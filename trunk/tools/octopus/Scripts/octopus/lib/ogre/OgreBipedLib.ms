function isPelvis bipObj =
(
	--format "isPelvis: % Vs %  --> % \n" bipObj.name (biped.getNode bipObj 12).name  ((biped.getNode bipObj 12) == bipObj);
	if (bipObj == undefined) then return false ;
	return ((biped.getNode bipObj 13) == bipObj) ;
)

function isFootStep bipObj =
(
	if (bipObj == undefined) then return false ;
	return ((biped.getNode bipObj 16) == bipObj) ;
)



-----------------------------------------------------------------------------
-- gets  keyframes from the controllers animation
-----------------------------------------------------------------------------
function getBipedTimeList obj firstframe lastframe =
(
	local list,rotContr,posContr,e  ;
	list = #(firstframe) ;
	list2 = #() ; -- this is the list which will be returned.
	
	-- Biped Bones and the root: Bip01 for example don't have the same controller
	
	-- Root
	if (isPelvis obj) then (
		-- vertical controller
		for e in obj.controller.vertical.controller.keys do (
			t =e.time ;
			if (t>firstFrame and t<=lastFrame) then (
				append list t ;
				print t ;	
			)
		)
		-- horizontal controller
		for e in obj.controller.horizontal.controller.keys do (
			t =e.time ;
			if (t>firstFrame and t<=lastFrame) then
				append list t ;
		)
		-- turn controller
		for e in obj.controller.turning.controller.keys do (
			t =e.time ;
			if (t>firstFrame and t<=lastFrame) then
				append list t ;
		)
		sort list ;
	)
	
	else (
		-- Biped Bones
		for e in obj.controller.keys do (
			t =e.time ;
			if (t>firstFrame and t<=lastFrame) then
				append list t ;
		)
	)
	
	-- if several keyframes have the same value, we keep just one
	keepLoneValues list list2 ;
	list2 ;
)


-----------------------------------------------------------------------------
-- write <track />
-- Selected keys belongs to [firstframe,lastFrame]
-- time = (key.time - firstFrame)*length/(lastFrame-firstFrame)
--   (e.g. first key has time 0.)
-----------------------------------------------------------------------------

function writeBipedTrack phy sk boneId firstframe lastframe length scale flipYZ outFile=
(
	local angle,timef,i,bname,d,mref,mparent ;
	
	-- displays information in the maxscript listener
	if (sk != undefined) then
		format "retrieving key information from skin for % ...\n" (skinOps.GetBoneName sk boneId 1) ;
	else
		format "retrieving key information from physique for % ...\n" ( (physiqueOps.GetBones $)[boneId] ) ;

	
	-- gets bone acording to the parameter boneId
	if (sk != undefined) then
		bname= skinOps.GetBoneName sk boneId 1 ;
	else -- physique
		bname = (physiqueOps.getBones $)[boneId].name ; 
	
	replaceSpaces bname ;
	d = execute ("$" + bname) ;
	
	-- gets keyframe list
	timelist = getBipedTimeList d firstframe lastframe ;
	
	-- track header
	format("\t\t\t\t<track bone = \"%\">\n") d.name to:outFile ;
	format("\t\t\t\t\t<keyframes>\n") to:outFile ;
	
	-- gets initial transform at frame 0f
	at time 0f (
		initTform = d.transform ;
		if (not isPelvis d) then (
			mparent = d.parent.transform ;
			initTform = initTform*inverse(mparent) ;
		)
		else if (flipYZ) then (
			format " - flipping pelvis track..." ;
			-- we add the bip Transform
			--initTform = initTform * d.controller.rootNode.transform ;
			initTform = flipYZTransform initTform ;
		)
	)
		
	-- for each frame in the list
	for i in timelist do
	(
		-- moves slider time and compute OGRE time
		at time i (
			timef = ((float) (i-firstFrame)*length)/(lastframe - firstframe ) ;
			
		-- First, rotation which depends on initial transformation
			Tform = d.transform ;
			-- if this is the pelvis
			if (isPelvis d) then (
				mparent = matrix3 1 ;
				-- if flipYZ == true
				if (flipYZ) then
					Tform = flipYZTransform Tform ;
			)			
			else
				mparent = d.parent.transform ; 
			
			-- computes rotation
			mref = initTform*mparent ;	
			Tform = Tform*inverse(mref) ;
			
			-- rotation part is saved.
			rot = Tform.rotation as angleaxis ;
			angle = - degToRad (rot.angle) ; -- don't know why there must be this minus :((((((
			
		-- Then, position which depends on parent			
			Tform=d.transform ;
			Tform=Tform*inverse(mparent) ;
			
			-- if this is the root bone and flipYZ == true
			if (isPelvis d and flipYZ) then (
				Tform = flipYZTransform Tform ;
			)
			
			-- substracts position of the initial transform
			Tform.pos -= initTform.pos ;
			Tform.pos = Tform.pos * scale ;
			
			pos = Tform.pos ;

				
			-- writes them !
			if (abs(pos.x)<1e-5) then pos.x = 0 ;
			if (abs(pos.y)<1e-5) then pos.y = 0 ;
			if (abs(pos.z)<1e-5) then pos.z = 0 ;
			
			format("\t\t\t\t\t\t<keyframe time=\"%\">\n") timef to: outFile ;
			format("\t\t\t\t\t\t\t<translate x=\"%\" y=\"%\" z=\"%\" />\n") pos.x pos.y pos.z to: outFile ;
			format("\t\t\t\t\t\t\t<rotate angle=\"%\">\n") angle to:outFile ; 
			format("\t\t\t\t\t\t\t\t<axis x=\"%\" y=\"%\" z=\"%\" />\n") (rot.axis.x) (rot.axis.y) (rot.axis.z) to:outFile ;
			format("\t\t\t\t\t\t\t</rotate>\n") to:outFile ;
			format("\t\t\t\t\t\t</keyframe>\n") to:outFile ;
		)
	)
	
	-- track end
	format("\t\t\t\t\t</keyframes>\n") to:outFile ;
	format("\t\t\t\t</track>\n") d.name to: outFile ;
)


-------------------------------------------------------------------------------------------------
------------------------------------------- WRITE SKELETON --------------------------------------
-------------------------------------------------------------------------------------------------

-- Nota :
-- A vertex 0 and a bone 0 are added because 3dsmax starts with vertex 1 and bone 1.


------------------
-- write <bones />
------------------

function writeBipedB phy sk id scale flipYZ outFile =
(
	-- gets bone acording to the parameter boneId
	if (sk != undefined) then
		bname= skinOps.GetBoneName sk id 1 ;
	else --physique
		bname = (physiqueOps.getBones $)[id].name ;
	
	
	replaceSpaces bname ;
	d = execute ("$" + bname) ;
	
	-- gets initial transform at frame 0f
	format("\t\t<bone id=\"%\" name=\"%\">\n") id d.name to:outFile ;
	
	slidertime = 0f ;
	Tform = d.transform ;
	if (not isPelvis d) then (
		mparent = d.parent.transform ;
		Tform = Tform*inverse(mparent) ;
	)
	
	Tform.pos = Tform.pos * scale ;
		
	if ((isPelvis d) and flipYZ) then (
		format "- Flipping pelvis... \n" ;
		Tform = flipYZTransform Tform ;
	)
	
	pos = Tform.pos ;
	rot = Tform.rotation as angleaxis ;
	angle = - degToRad (rot.angle) ; -- don't know why there must be this minus :((((((
	
	
			if (abs(pos.x)<1e-5) then pos.x = 0 ;
			if (abs(pos.y)<1e-5) then pos.y = 0 ;
			if (abs(pos.z)<1e-5) then pos.z = 0 ;
			
			
	format("\t\t\t<position x=\"%\" y=\"%\" z=\"%\" />\n") pos.x pos.y pos.z to:outFile ;
    format("\t\t\t<rotation angle=\"%\">\n") angle to:outFile ;
    format("\t\t\t\t<axis x=\"%\" y=\"%\" z=\"%\" />\n") rot.axis.x rot.axis.y rot.axis.z to:outFile ;
   	format("\t\t\t</rotation>\n") to:outFile ;
	format("\t\t</bone>\n") to:outFile ;
)
-----------------------------
-- write Bones (using writeB)
-----------------------------

function writeBipedBones phy sk scale flipYZ outFile =
(
	local i ;
	
	format("\t<bones>\n") to:outFile;
	
	-- we create a virtual bone number 0.
	format("\t\t<bone id=\"0\" name=\"zzwxy\">\n") to:outFile ;
	format("\t\t\t<position x=\"0\" y=\"0\" z=\"0\" />\n") to:outFile ;
    format("\t\t\t<rotation angle=\"0\">\n") to:outFile ;
    format("\t\t\t\t<axis x=\"1\" y=\"0\" z=\"0\" />\n") to:outFile ;
    format("\t\t\t</rotation>\n") to:outFile ;
	format("\t\t</bone>\n") to:outFile ;
	i = 0 ;
	
	if (sk != undefined) then
		for i=1 to (skinOps.GetNumberBones sk) do
		(
			writeBipedB phy sk i scale flipYZ outFile ;
		)
	else -- physique
		for i=1 to (physiqueOps.GetBoneCount $) do
		(
			writeBipedB phy sk i scale flipYZ outFile ;
		)	
	
	
	format("\t</bones>\n") to:outFile;
)

--------------------------
-- write <bonehierarchy />
--------------------------

function writeBipedHierarchy phy sk outFile =
(
	local bname,pelvis
	format("\t<bonehierarchy>\n") to:outFile ;

	if (sk != undefined) then
		bname= skinOps.GetBoneName sk 1 1 ;
	else --physique
		bname = (physiqueOps.getBones $)[1].name ;
		
	replaceSpaces bname ;
	d = execute ("$" + bname) ;
	pelvis = biped.getNode d 12 ;
	
	-- ecriture de la track pour le bone 0.
	format("\t\t<boneparent bone=\"zzwxy\" parent=\"%\" />\n") pelvis.name to:outFile ;

	if (sk != undefined) then
		for i=1 to (skinOps.GetNumberBones sk) do
		(
			-- gets bone:
			bname= skinOps.GetBoneName sk i 1 ;
			replaceSpaces bname ;
			d = execute ("$" + bname) ;
			if ((not isPelvis d) and (not isFootStep d) ) then
			(
				p = d.parent ;
				format("\t\t<boneparent bone=\"%\" parent=\"%\" />\n") d.name p.name to:outFile ;
			)
		)		
	else -- physique
		for i=1 to (physiqueOps.GetBoneCount $) do
		(
			-- gets bone:
			bname = (physiqueOps.getBones $)[i].name ;
			replaceSpaces bname ;
			d = execute ("$" + bname) ;
			if ((not isPelvis d) and (not isFootStep d) ) then
			(
				p = d.parent ;
				format("\t\t<boneparent bone=\"%\" parent=\"%\" />\n") d.name p.name to:outFile ;
			)
		)
	
	
	
	
	format("\t</bonehierarchy>\n") to:outFile ;
)

-----------------------
-- write <animations />
-----------------------
function writeBipedAnim phy sk firstFrame lastFrame length scale flipYZ name outFile =
(
	local i,n ;
	
	format("\t<animations>\n") to: outFile ;
    format("\t\t<animation name=\"%\" length=\"%\">\n") name length to:outFile ;
	format("\t\t\t<tracks>\n") to:outFile
	
	-- ecriture track 0
	format("\t\t\t\t<track bone = \"zzwxy\">\n") to: outFile ;
	format("\t\t\t\t\t<keyframes>\n") to:outFile ;
	format("\t\t\t\t\t\t<keyframe  time=\"0\">\n") to:outFile ;
	format("\t\t\t\t\t\t\t<translate x=\"0\" y=\"0\" z=\"0\" />\n") to: outFile ;
	format("\t\t\t\t\t\t\t<rotate angle=\"0\">\n")  to:outFile ; 
	format("\t\t\t\t\t\t\t\t<axis x=\"1\" y=\"0\" z=\"0\" />\n")  to:outFile ;
	format("\t\t\t\t\t\t\t</rotate>\n") to:outFile ;
	format("\t\t\t\t\t\t</keyframe>\n") to:outFile ;
	format("\t\t\t\t\t</keyframes>\n") to:outFile ;
	format("\t\t\t\t</track>\n") to: outFile ;
	
	if (sk != undefined) then
		n = skinOps.GetNumberBones sk ;
	else
		n = physiqueOps.GetBoneCount $ ;
	
	for i = 1 to n do
		writeBipedTrack phy sk i firstframe lastframe length scale flipYZ outFile ;

	format("\t\t\t</tracks>\n") to:outFile					
	format("\t\t</animation>\n") to: outFile ;
	format("\t</animations>\n") to: outFile ;
)

-------------------------------------------------------------
-- write <skeleton /> main function
-- write the animation in the file out_name + ".skeleton.xml"
-- between the frame firstFrame and lastFrame 
-- and scale time according to length
-------------------------------------------------------------

function writeBiped pmesh exportOptions out_name = 
(
	local sk,n,keys,initialKeys,messages,phy ;
	
	sk = getSkin pmesh ;
	phy = getPhysique pmesh ;
	
	if (sk == undefined and phy == undefined) then
	(
		MessageBox "There is no skin or physique modifier for this object" ;
	)
	else
	(	
		-- in order to perform, skin should be opened
		max modify mode ;
		if (sk != undefined) then
			modPanel.setCurrentObject pmesh.modifiers[#Skin] ;
		else -- physique
			modPanel.setCurrentObject pmesh.modifiers[#Physique] ;

		
			format "------------------------------------------\n"
			format "------ OGRE skeleton Exporter Log   ------\n"
			format "------------------------------------------\n"
		
			format "Exporter options :\n"
			format "- firstFrame: % \n- lastFrame: %\n" exportOptions.firstFrame exportOptions.lastFrame ;
			-- creates the output file
			outFile = createfile (out_name + ".skeleton.xml") ;

				-- writes header
				format("<skeleton>\n") to:outFile ;	
	
				format "Writing bones :\n" ;
				writeBipedBones phy sk exportOptions.scale exportOptions.flipYZ outFile ;
				format "Writing bone hierarchy.\n" ;
				writeBipedHierarchy phy sk outFile ;
				format "Writing bone tracks.\n" ;
				writeBipedAnim phy sk exportOptions.firstFrame exportOptions.lastFrame exportOptions.length exportOptions.scale exportOptions.flipYZ exportOptions.animName outFile ;

				-- ecriture, fin des balises
				format("</skeleton>\n") to: outFile ;
				
			format "------------------------------------------\n"
			format "----------          END          ---------\n"
			format "------------------------------------------\n"
		
				close outFile ;
				messageBox "Exporting skeleton successful !"
	)
	
)



