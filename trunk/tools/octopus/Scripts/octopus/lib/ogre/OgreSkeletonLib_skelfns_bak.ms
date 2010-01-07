-----------------------------------------------------------------
-- getParentId returns the id of the parent according to the skin
-----------------------------------------------------------------

function getParentIndex sk bid =
(
	local c,d,par,parName,bname,n,i,parentId ;
	
	parentId = 0 ;
	c= skinOps.GetBoneName sk bid 1 ;
	replaceSpaces c ;
	d = execute ("$" + c) ;
	par = d.parent ;
		
	if (par != undefined) then
	(
		parName = par.name ;
		n = skinOps.GetNumberBones sk ;
		for i=1 to n do
		(
			bname = skinOps.GetBoneName sk i 1;
			replaceSpaces bName ;
			if (parName==bname) then parentId = i ;
		)
	)
	parentId;
)

---------------------------------------------------------------------------------------
-- exploreKeys returns an array #(message1, boneName1, message2, boneName2, ... )
-- where message =
-- OK if when a parameter (posX,posY,posZ or rotX,rotY,rotZ) has a key,
--       all the others have also a key.
-- an error message else.
-- in fact you have to test the first element. if it's OK, good!, else look at messages
---------------------------------------------------------------------------------------

function exploreKeys sk =
(
	local i,d,boneName,rotContr,posContr,subRotContr,subPosContr ;
	local nRotKeys1,nRotKeys2,nRotKeys3,nPosKeys1,nPosKeys2,nPosKeys3,time1,time2,time3 ;
	local answer ;
	
	answer = #() ;
	
	for i=1 to (skinOps.getNumberBones sk) do
	(		
		boneName= skinOps.GetBoneName sk i 1 ;

		--format "verifying key information for % ...\n" boneName ;
		replaceSpaces boneName ;
		d = execute ("$" + boneName) ;
		rotContr = d.rotation.controller ;
		posContr = d.pos.controller ;
		
		if (not ((IsKindOf rotContr Euler_XYZ) and (IsKindOf posContr Position_XYZ) )) then
		(
			append answer "Your bones must have Euler_XYZ and Position_XYZ controllers (rotation and position)" ;
			append answer boneName ;
		)
		
		else (
		
		--3dsmax5 :
		subRotContr = getXYZControllers rotContr ;
		subPosContr = getXYZControllers posContr ;
		--3dsmax3
		--subRotContr = #(rotContr.x_rotation.controller,rotContr.y_rotation.controller,rotContr.z_rotation.controller) ;
		--subPosContr = #(posContr.x_position.controller,posContr.y_position.controller,posContr.z_position.controller) ;
		
		nRotKeys1 = numKeys subRotContr[1] ;
		nRotKeys2 = numKeys subRotContr[2] ;
		nRotKeys3 = numKeys subRotContr[3] ;
		
		nPosKeys1 = numKeys subPosContr[1] ;
		nPosKeys2 = numKeys subPosContr[2] ;
		nPosKeys3 = numKeys subPosContr[3] ;
		
		if (nRotKeys1 == 0 or nRotKeys2 == 0 or nRotKeys3 == 0 or nPosKeys1 == 0 or nPosKeys2 == 0 or nPosKeys3 == 0) then
		(
			append answer "There is no keyframe in a track" ;
			append answer boneName ;
		)
		else
			if (nRotKeys1 != nRotKeys2 or nRotKeys1 != nRotKeys3 or nPosKeys1 != nPosKeys2 or nPosKeys2 != nPosKeys3 or nPosKeys1 != nRotKeys1) then
			(
				append answer "Numbers of keys are not the same for all tracks.\nCheck rotation AND position or use the AddMissingKeys Tools" ;
				append answer boneName ;
			)
			else
				if ((getKey subRotContr[1] 1).time != 0f or (getKey subRotContr[2] 1).time != 0f or (getKey subRotContr[3] 1).time != 0f) then
				(
					append answer "There must be a keyframe for rotations at frame 0." ;
					append answer boneName ;
				)
				else
					if ((getKey subPosContr[1] 1).time != 0f or (getKey subPosContr[2] 1).time != 0f or (getKey subPosContr[3] 1).time != 0f) then
					(
						append answer "There must be a keyframe for positions at frame 0." ;
						append answer boneName ;
					)
					else
						for j=1 to nRotKeys1 do
						(
							time1 = (getKey subRotContr[1] j).time ;
							time2 = (getKey subRotContr[2] j).time ;
							time3 = (getKey subRotContr[3] j).time ;
							if (time1 != time2 or time1 != time3) then
							(
								append answer "At a frame, all tracks don't have a key, use the AddMissingKeys Tools" ;
								append answer boneName ;
							)
						)
		)
	)
	append answer "OK" ;
	--format "results of verification: % \n" answer ;
	answer ;
)


-----------------------------------------------------------------------------
-- get allkeys return an array of #(time (int), axis (point3), pos (point3)).
-- Selected keys belongs to [firstframe,lastFrame]
-- time = (key.time - firstFrame)*length/(lastFrame-firstFrame)
-- nb: so first key has time 0.
-----------------------------------------------------------------------------

function getAllKeys sk boneId firstFrame lastFrame length scale =
(
	local angle,ax,timef,nKeys ,i,j,tmp;
	local bname,d,rotContr,posContr,subRotContr,subPosContr,rotValues,posValues ;
	
	-- format "retrieving key information for % ...\n" (skinOps.GetBoneName sk boneId 1) ;
	
	a = #() ;
	ax = #() ;
	angle = 0 ;
	timef = 0 ;
	
	bname= skinOps.GetBoneName sk boneId 1 ;
	replaceSpaces bname ;
	d = execute ("$" + bname) ;
	
	rotContr = d.rotation.controller ;
	posContr = d.pos.controller ;
	--3dsmax5 :
	subRotContr = getXYZControllers rotContr ;
	subPosContr = getXYZControllers posContr ;
	
	nkeys = numKeys subRotContr[1] ;
	-- NOTA !!!
	-- there must be all the keys : position X,Y,Z and rotation X,Y,Z
	-- exploreKeys verifies this...
	
	for i = 1 to nkeys do
	(
		--3dsmax3: timef = ((getKey subRotContr[1] i).time as float)/160 ;
		--3dsmax5
		timef = (float) ((getKey subRotContr[1] i).time) ;
		
		if (timef >=firstFrame and timef<=lastFrame) then
		(
			timef = (timef-firstFrame)*length/(lastFrame-firstFrame) ;
			rotValues = [0,0,0] ;
			posValues = [0,0,0] ;
		
			-- enregistrement des valeurs des controllers
			for j = 1 to 3 do
			(
				tmp = (radToDeg ((getKey subRotContr[j] i).value)) ;
				
				rotValues[j] = tmp ;

				tmp = (getKey subPosContr[j] i).value ;
				
				posValues[j] = tmp ;
			)
			
			-- change scale 
			posValues = posValues*scale ;
						
			b = #(timef,rotValues,posValues)  ;
			append a b ;
		)
	)
	if (a[1] == undefined) then -- if there is no key between firstFrame and lastFrame we create a virtual key.
	(
		rotValues = [0,0,0] ;
		posValues = [0,0,0] ;
		
		-- enregistrement des valeurs des controllers
		for j = 1 to 3 do
		(
			tmp = (radToDeg ((getKey subRotContr[j] 1).value)) ;
			rotValues[j] = tmp ;
			
			tmp = (getKey subPosContr[j] 1).value ;
			posValues[j] = tmp ;
			)

		append a #(0,rotValues,posValues) ;
	)
	
	a ;
)


-------------------------------------------------------------------------------------------------
------------------------------------------- WRITE SKELETON --------------------------------------
-------------------------------------------------------------------------------------------------


-- Remarque !
-- 3dsmax commence tous ses ID a 1 alors que Ogre commence à 0.
-- -> on va rajouter un vertex 0 et un bone 0 pour assurer la compatibilté.

-- A vertex 0 and a bone 0 are added because 3dsmax starts with vertex 1 and bone 1.


------------------
-- write <bones />
------------------

function writeB sk id scale flipYZ outFile =
(
	local c,d,k,tmp,aax,aay,aaz,aa,mx,my,mz,angle,parentId,rotAngle,keys ;
	
	c= skinOps.GetBoneName sk id 1 ;
	replaceSpaces c ;
	d = execute ("$" + c) ;
	format("\t\t<bone id=\"%\" name=\"%\">\n") id d.name to:outFile ;
	
	keys = getAllKeys sk id 0 0 1 scale ; -- retrieving the first key.

	k = keys[1] ;	
	
	aaX = Angleaxis (k[2].x) (point3 1 0 0) ;
	aaY = Angleaxis (k[2].y) (point3 0 1 0) ;
	aaZ = Angleaxis (k[2].z) (point3 0 0 1) ;
			
	mX = aaX as matrix3 ;
	mY = aaY as matrix3 ;
	mZ = aaZ as matrix3 ;

	m = mZ*mY*mX ;


	if ((d.parent == undefined) and flipYZ) then  -- FLIP Y/Z: root bone
	(
		format "- Flipping root bone... \n" ;
		-- rotate
		aaX = Angleaxis (-90) (point3 1 0 0) ;
		aaY =AngleAxis 0 (point3 0 1 0) ;
		aaZ = Angleaxis 0 (point3 0 0 1) ;
		mX = aaX as matrix3 ;
		mY = aaY as matrix3 ;
		mZ = aaZ as matrix3 ;
		m2 = mZ*mY*mX ;		
		m = m2*m ;
		-- change position
		tmp = k[3].z ;
		k[3].z = -k[3].y ;
		k[3].y = tmp ;
		
	)

	aa = m as angleaxis ;
	angle = aa.angle ;
	angle = degToRad(angle) ;
	
	format("\t\t\t<position x=\"%\" y=\"%\" z=\"%\" />\n") k[3].x k[3].y k[3].z to:outFile ;
    format("\t\t\t<rotation angle=\"%\">\n") angle to:outFile ;
    format("\t\t\t\t<axis x=\"%\" y=\"%\" z=\"%\" />\n") aa.axis.x aa.axis.y aa.axis.z to:outFile ;
   	format("\t\t\t</rotation>\n") to:outFile ;
	format("\t\t</bone>\n") to:outFile ;
)
-----------------------------
-- write Bones (using writeB)
-----------------------------

function writeBones sk scale flipYZ outFile =
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
	for i=1 to (skinOps.GetNumberBones sk) do
	(
		writeB sk i scale flipYZ outFile ;
	)
	format("\t</bones>\n") to:outFile;
)

-----------------------------------
-- recursive fun to write hierarchy
-----------------------------------

function writeH b outFile =
(
	if (b.parent != undefined) do
	(
		p = b.parent ;
		format("\t\t<boneparent bone=\"%\" parent=\"%\" />\n") b.name p.name to:outFile ;
	)
	childrenArray = b.children ;
	for i=1 to childrenArray.count do
	(
		writeH childrenArray[i] outFile ;
	)
)

--------------------------
-- write <bonehierarchy />
--------------------------

function writeHierarchy sk outFile =
(
	format("\t<bonehierarchy>\n") to:outFile ;
	--3dsmax3 :	roots = #(execute("$" + (skinops.getBoneName sk 2 1))) ;
	--3dsmax5 : 
	roots = getRoots sk ;
	
	-- ecriture de la track pour le bone 0.
	format("\t\t<boneparent bone=\"zzwxy\" parent=\"%\" />\n") roots[1].name to:outFile ;
	for b in roots do
	(
		writeH b outFile;
	)
	format("\t</bonehierarchy>\n") to:outFile ;
)

--------------------------------------------------------------------------------------------------------------------------------------
-- write <track />
-- initialKeys are the keys for the frame 0, because we usually create mesh and bones at frame 0.
--   But animation we want to export can start at another frame number. And initial Rotation are expected in order to exportation work.
--   Use getAllkeys with firstFrame=0 and lastFrame = 0
-- keys is the array returned by getAllKeys
---------------------------------------------------------------------------------------------------------------------------------------

function writeTrack sk boneId firstFrame lastFrame length outFile =
(
	local bname,d,k1,k,trans,rot,m0,m1,m2,m3,mFinal,mX,mY,mZ,initialKey,keys ;
	
	bname= skinOps.GetBoneName sk boneId 1 ;
	replaceSpaces bname ;
	d = execute ("$" + bname) ;
	
	format("\t\t\t\t<track bone = \"%\">\n") d.name to: outFile ;
	format("\t\t\t\t\t<keyframes>\n") to:outFile ;
	
	initialKey = getAllkeys sk boneId 0 0 length 1 ;
	keys = getAllkeys sk boneId firstFrame lastFrame length 1 ;
	
	k1 = initialKey[1] ;
	
	for k in keys do
	(	
		trans = k[3] - k1[3] ;
		rot = k[2] - k1[2] ;
		
		aaX = Angleaxis (k1[2].x) (point3 1 0 0) ;
		aaY = Angleaxis (k1[2].y) (point3 0 1 0) ;
		aaZ = Angleaxis (k1[2].z) (point3 0 0 1) ;
			
		mX = aaX as matrix3 ;
		mY = aaY as matrix3 ;
		mZ = aaZ as matrix3 ;
		
		m0 = mZ*mY*mX ;

		aaX = Angleaxis (k[2].x) (point3 1 0 0) ;
		aaY = Angleaxis (k[2].y) (point3 0 1 0) ;
		aaZ = Angleaxis (k[2].z) (point3 0 0 1) ;
		
		mX = aaX as matrix3 ;
		mY = aaY as matrix3 ;
		mZ = aaZ as matrix3 ;		
		
		m1 = mZ*mY*mX ;	
		m = m1*(inverse m0) --> Matrix in Parent coords (!) (dans la base du pere).
		
		-- change coords (changement de repere)
		
		m2 = m0 ;
		m3 = inverse m2 ; 
		
		mFinal = m3*m*m2 ;
		
		aa = mFinal as angleaxis ;
		angle = aa.angle ;
		angle = degToRad(angle) ;

		format("\t\t\t\t\t\t<keyframe time=\"%\">\n") k[1] to: outFile ;
		format("\t\t\t\t\t\t\t<translate x=\"%\" y=\"%\" z=\"%\" />\n") trans.x trans.y trans.z to: outFile ;
		format("\t\t\t\t\t\t\t<rotate angle=\"%\">\n") angle to:outFile ; 
		format("\t\t\t\t\t\t\t\t<axis x=\"%\" y=\"%\" z=\"%\" />\n") (aa.axis.x) (aa.axis.y) (aa.axis.z) to:outFile ;
		format("\t\t\t\t\t\t\t</rotate>\n") to:outFile ;
		format("\t\t\t\t\t\t</keyframe>\n") to:outFile ;
	)
	format("\t\t\t\t\t</keyframes>\n") to:outFile ;
	format("\t\t\t\t</track>\n") d.name to: outFile ;

)

-----------------------
-- write <animations />
-----------------------

function writeAnim sk firstFrame lastFrame length name outFile =
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
	
	n = skinOps.GetNumberBones sk ;
	for i = 1 to n do
		writeTrack sk i firstFrame lastFrame length outFile ;

	format("\t\t\t</tracks>\n") to:outFile					
	format("\t\t</animation>\n") to: outFile ;
	format("\t</animations>\n") to: outFile ;
)

------------
-- flip Axes
------------
function flipYZ sk keys initialKeys =
(
	local i,k,ktmp ;
	
	--print "flip axes" ;
	for i in (getRootsId sk) do
	(
		--format "i:%\n" i ;
		-- flip <=> rotate the root
		for k in (keys[i]) do
		(
			ktmp = copy k[2] ;
			
			k[2][1] = ktmp[1]+90 ;
			k[2][3] = -ktmp[2] ;
			k[2][2] = ktmp[3] ;
		)
		
		for k in (initialKeys[i]) do
		(
			ktmp = copy k[2] ;
			--format "ktmp: % , k[2]: %\n" ktmp (k[2]) ;
			
			k[2][1] = ktmp[1]+90 ;
			k[2][3] = -ktmp[2] ;
			k[2][2] = ktmp[3] ;
		)
	)
)

-------------------------------------------------------------
-- write <skeleton /> main function
-- write the animation in the file out_name + ".skeleton.xml"
-- between the frame firstFrame and lastFrame 
-- and scale time according to length
-------------------------------------------------------------

function writeSkeleton pmesh exportOptions out_name = 
(
	local sk,n,keys,initialKeys,messages ;
	sk = getSkin pmesh ;
	
	if (sk == undefined) then
	(
		close outFile ;
		MessageBox "There is no skin modifier for this object" ;
	)
	else
	(	
		-- in order to perform, skin should be opened
		max modify mode ;
		modPanel.setCurrentObject pmesh.modifiers[#Skin] ;
		
		-- trying to find errors
		messages = exploreKeys sk ;
		if (messages[1] != "OK") then
		(
			MessageBox ("\n There is a problem with bone: " + messages[2] + "\n\nHere is the found error:\n" + messages[1] + "\n\nOperation aborted") ;
		)
		else
		(
			format "------------------------------------------\n"
			format "------ OGRE skeleton Exporter Log   ------\n"
			format "------------------------------------------\n"
		
			format "Exporter options :\n"
			format "- firstFrame: % \n- lastFrame: %\n" exportOptions.firstFrame exportOptions.lastFrame ;
			-- create the output file
			outFile = createfile (out_name + ".skeleton.xml") ;

				-- ecriture de l'entete
				format("<skeleton>\n") to:outFile ;	
	
				--if (exportOptions.yAxis==3 and exportOptions.zAxis==2) then
				--	flipYZ sk keys initialKeys ;
				format "Writing bones :\n" ;
				writeBones sk exportOptions.scale exportOptions.flipYZ outFile ;
				format "Writing bone hierarchy.\n" ;
				writeHierarchy sk outFile ;
				format "Writing bone tracks.\n" ;
				writeAnim sk exportOptions.firstFrame exportOptions.lastFrame exportOptions.length exportOptions.animName outFile ;

				-- ecriture, fin des balises
				format("</skeleton>\n") to: outFile ;
				
			format "------------------------------------------\n"
			format "----------          END          ---------\n"
			format "------------------------------------------\n"
		
				close outFile ;
				messageBox "Exporting skeleton successful !"
		)
	)
	
)



