
----------------------------------------------------------------
-- returns the correct normal for the vertex
-- according to smoothing groups
-----------------------------------------------------------------
function getVertexNormalAccordingToSmoothingGroups selectedMesh faceIndex vertexIndex =
(
        local normal;
        local faceSmoothingGroup = getFaceSmoothGroup selectedMesh faceIndex

        if faceSmoothingGroup == 0 then
        (
                -- if not smooth use face normal
                normal = getFaceNormal selectedMesh faceIndex
        )       
        else
        (
                                local vertexIndexes = getFace selectedMesh faceIndex;
                local vI = vertexIndexes[vertexIndex] 
                local n = [0, 0, 0] -- point3
                for adjoiningFaceIndex in (meshop.getFacesUsingVert selectedMesh vI) do
                (
                                        sg = getFaceSmoothGroup selectedMesh adjoiningFaceIndex;
                                        and_sg = bit.and sg faceSmoothingGroup ;
                                        if (and_sg != 0) then
                                                n += getFaceNormal selectedMesh adjoiningFaceIndex ;
                )
                normal = normalize n;
        )       

        normal;
)

function getDatas2 tmesh mtrx flipyz scale1 flipNormal exportUV exportColours sk phy =
(
    local num_tverts, num_faces,v,f,cf,face,nmal,vert,vertTmp,tvertTmp,faceTmp ;
	local couple,searchTable,ogreface ;
	local i,j,k,vwcount,bid,w,matId
	local rootId ;
	
	searchTable=#() ;
	-- In order to know if a #(vertex,tvertex) has already be found,
	-- just read in the searchtable[vertex] in order to find (or not) an element such as #(tvertex,cvertex,ogreVertNum)
	-- This algorithm was used because I think there is not lots of (tvertex,cvertex) linked to one vertex...
	-- Maybe I'm wrong, but I don't find anything else to perform a quick search and to save memory...
	searchTables=#() ;
	-- It is a searchTable array. (one searchTable for each submesh)
	
	num_tverts = tmesh.numtverts ;
	num_faces = tmesh.numfaces ;

	ogreFace=Point3 12 12 12 ;
	cmpt = 1 ; -- vertex index.

	-- Data arrays init.	
	verticesArrays=#() ;
	facesArrays=#() ;
	boneAssignments = #() ;
	submeshesId = #() ;
	ogreVertIndex=#() ;
	

	-- For each face
	for i = 1 to (getnumfaces tmesh) do -- For each face
	(		
		
		f = getface tmesh i ;
		matId = getFaceMatId tmesh i ;		
		
		-- we retrieve datas. it depends on options.
		-- UV face
		if (exportUV) then
			tvf = gettvface tmesh i ;
		else
			tvf = point3 1 1 1 ;
		
		-- Color Face
		if (exportColours) then
			cf = getVCFace tmesh i ;
		else
			cf = point3 1 1 1 ;
		
		-- For each vertex whose face is made up of.
		for j=1 to 3 do (			
		    a = (int) (f[j]) ;
			b = (int) (tvf[j]) ;
			c = (int) (cf[j]) ;

			-- we try to find the corresponding searchtable.											
			if (searchTables[matId] == undefined ) then
				searchTables[matId]=#() ;	
			
			searchTable = searchTables[matId] ;
			
			-- Maybe this triplet (vertex,tvertex,cvertex) has already been found.
			-- So we use searchTable.
			ogreVertNum = -1 ;
			
		    if (searchTable[a] == undefined ) then
				searchTable[a]=#() ;
			
			tab = searchTable[a] ;
			
			-- Now we can search in the array searchTable[a]
			for triplet in tab do
			(
				if (triplet[1] == b) then
					if (triplet[2] == c) then
						ogreVertNum = triplet[3] ;
			)

			if (ogreVertNum == -1) then -- That is to say it has not been found. 
			(
				-- the vertex number for the current submesh must be updated
				
				-- if undefined, the array is created
				if (ogreVertIndex[matId] == undefined ) then
					ogreVertIndex[matId]=1 ;
				
				-- vertex number is increased.								
				ogreVertNum = ogreVertIndex[matId] ;
				ogreVertIndex[matId] = ogreVertIndex[matId] + 1 ;
				
				-- it is added to the search table.
				append searchTable[a] #(b,c,ogreVertNum) ;
				
				-- we retrieve data
				vert = getVert tmesh f[j] ;
				
				format "% \n" vert ;
				-- nmal = getNormal tmesh f[j] ;
				nmal = getVertexNormalAccordingToSmoothingGroups tmesh i j
				tvert = point3 0 0 0 ;
				if (exportUV) then
					tvert = getTvert tmesh tvf[j] ;   
				cvert = color 0 0 0 1;
				if (exportColours) then
					cvert = getVertColor tmesh cf[j] ;
				
				-- change scale
				vert = vert * scale1 ;
				
				localpoint = point3 vert[1] vert[2] vert[3]
				localpoint = localpoint * mtrx
				vert[1] = (floor (localpoint[1]*10))/10 ;
				vert[2] = (floor (localpoint[2]*10))/10 ;
				vert[3] = (floor (localpoint[3]*10))/10 ;
		
				-- flip axes		
				if (flipyz) then
				(
					vertTmp = copy vert ;
					vert[2] = vertTmp[3] ;
					vert[3] = -vertTmp[2] ;
					
					nmalTmp = copy nmal ;
					nmal[2] = nmalTmp[3] ;
					nmal[3] = -nmalTmp[2] ;
				)
				
				-- Maybe the material found defines a new submesh...
				if (verticesArrays[matId] == undefined) then
				(
					format "- Submesh detected: material ID = %\n" matId
					verticesArrays[matId] = #() ;
					boneAssignments[matId] = #() ;
					append submeshesId matId ;
					-- A new vertex is created because OGRE starts with 0 although 3dsmax starts with 1
					append verticesArrays[matId] #(vert[1],vert[2],vert[3],nmal[1],nmal[2],nmal[3],tvert[1],tvert[2],cvert.r,cvert.g,cvert.b,cvert.a) ;   -- Vertex is added to datas
				)
				append verticesArrays[matId] #(vert[1],vert[2],vert[3],nmal[1],nmal[2],nmal[3],tvert[1],tvert[2],cvert.r,cvert.g,cvert.b,cvert.a) ;   -- Vertex is added to datas

				
				-- And now, bone assignments. (if a skin modifier is present)
				-- It seems that there are issues when just few vertices have bone assignments.
				-- So there is at least the root assignment.
				if (sk != undefined) then (
					vwcount = skinOps.getVertexWeightCount sk a ; -- a is the vertex number: "a = (int) (f[j]) ;"

					for k=1 to vwcount do
					(
						bid = skinOps.getVertexWeightBoneID sk a k ;
						w = skinOps.getVertexWeight sk a k ;
						
						append boneAssignments[matId] #(ogreVertNum,bid,w) ;
					)
					-- assignment to the root bone.
					if (vwcount==0) then (
						-- gets the root Id:
						rootId=getRootsId sk
						append boneAssignments[matId] #(ogreVertNum,rootId[1],1) ;
					)
				)
				-- same thing with physique modifier if defined
				if (phy != undefined) then (
					-- TODO: here the selection ($) is used because tmesh can't be used...
					-- it should be changed ti use pmesh (in the main function) for example.
					
					vwcount = physiqueOps.getVertexBoneCount $ a ; -- a is the vertex number: "a = (int) (f[j]) ;"

					for k=1 to vwcount do
					(
						bone = physiqueOps.getVertexBone $ a k
						w = physiqueOps.getVertexWeight $ a k
						
						-- search the bone number
						bid = getUserProp bone "number" ;
						
						--format "%,%\n%(%)\n" v weight bid.Name k ;
						append boneAssignments[matId] #(ogreVertNum,bid,w) ;
					)
					-- assignment to the root bone.
					if (vwcount==0) then (
						-- gets the root Id:
						--rootId=getRootsId sk
						--append boneAssignments[matId] #(ogreVertNum,rootId[1],1) ;
						print "No assignment" ;
					)
				)				

			)
		
			ogreFace[j] = ogreVertNum ;		
		)
		
		-- flip normal capability
		if (flipNormal) then
		(
			faceTmp = copy ogreFace ;
			ogreFace[2] = faceTmp[3] ;
			ogreFace[3] = faceTmp[2] ;
		)

		if (facesArrays[matId] == undefined) then
			facesArrays[matId] = #() ;		
		append facesArrays[matId] #(ogreFace[1],ogreFace[2],ogreFace[3]) ;   -- Face is added to datas		
	)
)

function writeMesh2 pmesh exportOptions unitsScale out_name =
(
	local m,sk,outFile,message,phy ;

	m = snapshotAsMesh pmesh ;

	-- tries to find errors
	message = exploreMesh pmesh exportOptions.exportUV exportOptions.exportColours;
	if (message != "OK") then
	(
		MessageBox ("\nMesh export error:\nA problem occured while exporting file\n\"" + out_name + "\"\n-" + message + "\nOperation aborted") ;
		delete m ;
		return false ;
	)
	else
	(			
		format "\n\n"
		format "------------------------------------------\n"
		format "-------   OGRE Mesh Exporter Log   -------\n"
		format "-----                                -----\n"
		
		-- get the skin modifier ( may be undefined )
		-- and physique modifier
		phy = getPhysique pmesh ;
		sk = getSkin pmesh;
		
		if (sk != undefined) then
			format "Skin modifier detected.\n"
			
		if (phy != undefined) then
			format "Physique modifier detected.\n"
			
		-- if not undefined, skin modifier is selected in modifier tab. Or there will be an error.
		if (sk != undefined) then
		(
			-- in order to perform, skin should be opened
			max modify mode ;
			modPanel.setCurrentObject pmesh.modifiers[#Skin] ;		
		)
		
		-- physique
		if (phy != undefined) then
		(
			-- in order to perform, skin should be opened
			max modify mode ;
			modPanel.setCurrentObject pmesh.modifiers[#Physique] ;
			setBonesNumber phy ;
			--physiqueOps.setInitialPose pmesh true ;
		)
		
		-- retieving material
		if (pmesh.material != undefined) then (
			materialName = pmesh.material.name ;
			replaceSpaces materialName ;
		)
		else materialName = pmesh.name + "Material" ;
		
		format "Material name exported : \n- %/*** \n" materialName ;
		
		-- retrieving all datas
		format "Retrieving vertices and faces data : \n" ;

		tmatrix = pmesh.transform
		translate tmatrix (pmesh.transform.translationpart * -1.0)
		translate tmatrix (pmesh.transform.translationpart / unitsScale )
		wmatrix = inverse tmatrix
--		scale wmatrix pmesh.transform.scalepart true
		getDatas2 m wmatrix exportOptions.flipyz exportOptions.scale exportOptions.flipNormal exportOptions.exportUV exportOptions.exportColours sk phy;
		
		-- writing in the file
		format "Writing in file %.mesh.xml : \n" out_name ;
		
--		wmatrix = inverse pmesh.transform
--		scale wmatrix pmesh.transform.scalepart true
--		WriteM2 wmatrix exportOptions.exportUV exportOptions.exportColours pmesh.material out_name ;
		WriteM exportOptions.exportUV exportOptions.exportColours pmesh.material out_name ;		
		format "Export successful.\n"		
		
		delete m ;
		
		format "-----                                -----\n"
		format "----------          END          ---------\n"
		format "------------------------------------------\n"
		
		print ("Exporting mesh successful !") ;
		
	)
	return true ;
)

function writeBipedAnim2 phy sk firstFrame lastFrame length scale flipYZ name outFile =
(
	local i,n ;
	
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
)

function writeBiped2 pmesh exportOptions mod out_name = 
(
	local sk,n,keys,initialKeys,messages,phy ;
	
	sk = getSkin pmesh ;
	phy = getPhysique pmesh ;
	
	if (sk == undefined and phy == undefined) then
	(
		MessageBox ("\nSkeleton export error:\nNo skin or physique modifier found for object:\n" + pmesh.name + "\n\nOperation aborted");
		return false
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
	
		-- creates the output file
		outFile = createfile (out_name + ".skeleton.xml") ;

			-- writes header
			format("<skeleton>\n") to:outFile ;	

			format "Writing bones :\n" ;
			writeBipedBones phy sk exportOptions.scale exportOptions.flipYZ outFile ;
			format "Writing bone hierarchy.\n" ;
			writeBipedHierarchy phy sk outFile ;
			format "Writing bone tracks.\n" ;
			format("\t<animations>\n") to: outFile ;
			local anumber = 1
			for aname in mod.anames do
			(
				format "Exporter options :\n"
				format "- firstFrame: % \n- lastFrame: %\n" mod.afframes[anumber] mod.alframes[anumber] ;

				writeBipedAnim2 phy sk exportOptions.mod.afframes[anumber] mod.alframes[anumber] mod.artimes[anumber] exportOptions.scale exportOptions.flipYZ aname outFile ;
				anumber = anumber + 1
			)
			format("\t</animations>\n") to: outFile ;
			-- ecriture, fin des balises
			format("</skeleton>\n") to: outFile ;
			
		format "------------------------------------------\n"
		format "----------          END          ---------\n"
		format "------------------------------------------\n"
	
		close outFile ;
		return true;
	) 	
)

function writeAnim2 sk firstFrame lastFrame length scale flipYZ name outFile =
(
	local i,n ;
	
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
	liste = #(0,20,30,40) ;
	for i = 1 to n do
		writeTrack sk i firstframe lastframe length scale flipYZ outFile ;

	format("\t\t\t</tracks>\n") to:outFile
	format("\t\t</animation>\n") to: outFile ;
)

function writeSkeleton2 pmesh exportOptions mod out_name = 
(
	local sk,n,keys,initialKeys,messages ;
	sk = getSkin pmesh ;
	
	if (sk == undefined) then
	(
		MessageBox ("\nSkeleton export error:\nNo skin modifier found for object:\n" + pmesh.name + "\n\nOperation aborted");
		return false;
	)
	else
	(	
		-- in order to perform, skin should be opened
		max modify mode ;
		modPanel.setCurrentObject pmesh.modifiers[#Skin] ;
		
		format "------------------------------------------\n"
		format "------ OGRE skeleton Exporter Log   ------\n"
		format "------------------------------------------\n"
	
		-- creates the output file
		outFile = createfile (out_name + ".skeleton.xml") ;

			-- writes header
			format("<skeleton>\n") to:outFile ;	

			format "Writing bones :\n" ;
			writeBones sk exportOptions.scale exportOptions.flipYZ outFile ;
			format "Writing bone hierarchy.\n" ;
			writeHierarchy sk outFile ;
			format "Writing bone tracks.\n" ;
			format("\t<animations>\n") to: outFile ;
			local anumber = 1
			for aname in mod.anames do
			(
				format "Exporter options :\n"
				format "- firstFrame: % \n- lastFrame: %\n" mod.afframes[anumber] mod.alframes[anumber] ;

				writeAnim2 sk mod.afframes[anumber] mod.alframes[anumber] mod.artimes[anumber] exportOptions.scale exportOptions.flipYZ aname outFile ;
				anumber = anumber + 1
			)
			format("\t</animations>\n") to: outFile ;

			-- ecriture, fin des balises
			format("</skeleton>\n") to: outFile ;
			
		format "------------------------------------------\n"
		format "----------          END          ---------\n"
		format "------------------------------------------\n"
	
		close outFile ;
		return true;
	)
)