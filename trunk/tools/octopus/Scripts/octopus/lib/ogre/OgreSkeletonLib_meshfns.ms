
----------------------------------------------------------------------------------------
-- ----------------------------------- WRITE MESH ----------------------------------- --
----------------------------------------------------------------------------------------
-- NOTA
-- 3dsmax starts ID from 1 although OGRE starts from 0.
-- -> a vertex 0 and a bone 0 will be added

-- flipNormal, scale, flipYZ, exportColours,exportUV : mesh parameters
-- firstFrmae, lastFrame,length,animName : animation parameters.
struct exportOptions (firstFrame, lastFrame, length, animName, scale, flipyz, flipNormal, exportColours, exportUV) ;



-----------------------------------------------------------------
-- Global 'variables'
-- There is an array with vertices datas for each submesh
-- There is also an array made up of faces.
-----------------------------------------------------------------

verticesArrays=#() ;
facesArrays=#() ;
boneAssignments=#() ;
submeshesId=#() ;
ogreVertIndex=#() ;
materialName = "" ;

-- verticesArrays[i] will describe the submesh number i and its elements will be ogreVertices where
--	ogreVert = #(x,y,z,nx,ny,nz,u,v,r,g,b,a) 
--   		     ----- -------- --- -------
--          	 Pos     Nmal   U,V   RGBA

-- facesArrays[i] consists in arrays like #(v1,v2,v3) which discribe submesh number i.

-- boneAssignments is made up of arrays like #(vertex,bone,weight)

-- In submeshesId, there are material Ids

-- in ogreVertIndex, there are the current index for submeshes

-- materialName is the name that Ogre will use


-----------------------------------------------------------------
-- There is no function to have the number of a bone with the 
-- Physique modifier, so before trying to export, the user
-- properties "number" is added to bones.
-----------------------------------------------------------------

function setBonesNumber phy =
(
	local i ;

	for i=1 to (physiqueOps.GetBoneCount $) do
	(
		b = (physiqueOps.getBones $)[i] ;
		setUserProp b "number" i ;
	)
)


	
-----------------------------------------------------------------
-- exploreMesh returns a string which is "OK" or an error message
-- if there are warnings, it displays message boxes.
-----------------------------------------------------------------

function exploreMesh pmesh exportUV exportColours=
(
	local material, answer, m ;
	
	answer = "" ;
	m = snapShotAsMesh pmesh ;
	
	-- first, is a material defined	?
	material = pmesh.material ;
	if (material == undefined) then
		answer = "This mesh doesn't have any material, please apply one\n" ;
	else
		-- then, are UVW coords set up ?
		if (exportUV and getnumtverts m == 0) then
			answer = "This mesh must have UVW coords in order to be exported" ;
		
	if (answer == "") then 
		answer = "OK" ;
	
	delete m ;
	answer ;
)

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

function tabEquals tab1 tab2 =
(
        if (tab1.count != tab2.count) then
                return false;

        for i=1 to tab1.count do
                if (tab1[i] != tab2[i]) then
                        return false;
                
        return true;
)

----------------------------------------------------------------------------
-- Retrieves al the datas which wil be used
-- flipyz = true if you want coords to fit in OGRE.
-- flipNormal = true if you want to flip normals
-- scale : the scale factor
-- exportUV = true if you want to export texture
-- exportColor = true if you want to export vertices colors
-- sk : the skin modifier (may be undefined)
--
-- stores datas in verticesArrays and facesArrays.
----------------------------------------------------------------------------

function getDatas tmesh flipyz scale flipNormal exportUV exportColours sk phy =
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
				vert = vert * scale ;
		
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

-------------------------------------------------
-- writes in the mesh.xml file
-- outName : filename
-- exportUV = true if you want to export texture
-- exportColor = true if you want to export vertices colors
--
-- Uses the arrays verticesArrays and facesArrays
-------------------------------------------------

function writeM exportUV exportColours material outName =
(
	local a,v,f,matname,hasSkeleton,outFile,numTexCoord,hasColours,matId ;
	
	hasSkeleton = false ;
	texCoordString = "" ;
	if (exportUV) then 
		texCoordString = "texture_coords=\"1\" texture_coords_dimensions_0=\"2\" "
	hasColours = false ;	
	if (exportColours) then
		hasColours = true ;		
	

	-- the final file is created
	outFile = createfile (outName + ".mesh.xml") ;

	-- writes the header
	format("<mesh>\n") to:outFile ;

	-- submeshes start
	format("\t<submeshes>\n") to:outFile ;
	
	-- for each submesh
	for matId in submeshesId do (
		-- Do we need 32-bit indexes?
		use32bitindexes = "false";
		if arrayLength verticesArrays[matId] > 65535 then
			use32bitindexes = "true";
		
		-- get the name of the sub material if needed.
		submatname = replacespaces material.name ;
		
		if (classof material == MultiMaterial) then
		    submatname  += "/" + replacespaces material.materiallist[matId].name ;
			
									-- HELLO ! --
			---------------------------------------------------------------------------------
			-- IF YOU COME HERE BECAUSE YOUR OBJECT FAILED EXPORTING, MAYBE THAT'S BECAUSE --
			-- YOU USE MATERIAL ID IN YOUR MESH THAT DOESN'T EXIST  IN YOUR MULTIMATERIAL  --
			---------------------------------------------------------------------------------
		
		format("\t\t<submesh material = \"%\" usesharedvertices=\"false\" use32bitindexes=\"%\">\n") submatname use32bitindexes to:outFile ;
	
		format "- writing faces...\n"		
		format("\t\t\t<faces count=\"%\">\n") (arraylength facesArrays[matId]) to:outFile;
		
		-- for each face
		for f in facesArrays[matId] do (
			format("\t\t\t\t<face ") to:outFile ;
			format("v1=\"%\" v2=\"%\" v3=\"%\" />\n") ((int)f[1]) ((int)f[2]) ((int)f[3]) to:outFile ;
		)
		format("\t\t\t</faces>\n") to:outFile ;

		format "- writing vertices...\n"	
		format("\t\t\t<geometry vertexcount=\"%\">\n") (arrayLength verticesArrays[matId] ) to:outFile;
		format("\t\t\t\t<vertexbuffer positions=\"true\" normals=\"true\" colours_diffuse=\"%\" %>\n") hasColours TexCoordString to:outFile ;		
		
		-- for each vertex
		for v in verticesArrays[matId] do
		(	
	
			format("\t\t\t\t\t<vertex>\n") to:outFile ;
			format("\t\t\t\t\t\t<position x=\"%\" y=\"%\" z=\"%\" />\n") v[1] v[2] v[3] to:outFile ;
			format("\t\t\t\t\t\t<normal x=\"%\" y=\"%\" z=\"%\" />\n") v[4] v[5] v[6] to:outFile ;
			if (exportUV) then
				format("\t\t\t\t\t\t<texcoord u=\"%\" v=\"%\" />\n") v[7] (1 - v[8]) to:outFile ;		
			if (exportColours) then (
				cv = (float) 255*(int)v[12] ;   -- a
				cv = 255*(cv + (int)v[11]) ; -- b
				cv = 255*(cv + (int)v[10]) ; -- g
				cv = cv+ (int)v[12] ;        -- r
				cv_part2 = (int) (mod cv 1000000) ;
				cv_part1 = (int) (cv/1000000);
				format("\t\t\t\t\t\t<colour_diffuse value=\"%\" />\n") cv_part1 cv_part2 to:outFile ;
			)
			format("\t\t\t\t\t</vertex>\n") to:outFile ;
		)	
		format("\t\t\t\t</vertexbuffer>\n") to:outFile ;
		format("\t\t\t</geometry>\n") to:outFile ;
					
	
		-- and now bone assignments (and skeleton), if there is at least one element in boneAssignments array.		
		if ((arrayLength boneAssignments[matId]) != 0) then 
		(
			hasSkeleton = true ;
			format "- writing bone assignments...\n"	
			format("\t\t\t<boneassignments>\n") to:outFile ;
				
			for a in boneAssignments[matId] do (
				format("\t\t\t\t<vertexboneassignment vertexindex=\"%\" boneindex=\"%\" weight=\"%\" />\n") a[1] a[2] a[3] to:outFile ;
			)
			format("\t\t\t</boneassignments>\n") to:outFile ;
		)
	
		-- submesh end
		format("\t\t</submesh>\n") to:outFile ;
	)
	
	-- submeshes end
	format("\t</submeshes>\n") to:outFile ;

	-- Skeleton link if there is at least one bone assignement.	
	if (hasSkeleton) then 
	(
		t = filterstring outName "\\" ;
		format ("\t<skeletonlink name=\"%\"/>\n") (t[arrayLength t] + ".skeleton") to:outFile ;				
	)
				
	format("</mesh>\n") to: outFile ;
	close outFile ;
)


---------------------------------
-- writes the mesh: main function
---------------------------------

function writeMesh pmesh exportOptions out_name =
(
	local m,sk,outFile,message,phy ;

	m = snapshotAsMesh pmesh ;
		
	-- tries to find errors
	message = exploreMesh pmesh exportOptions.exportUV exportOptions.exportColours;
	if (message != "OK") then
	(
		MessageBox ("\n There is a problem with your mesh:\n" + message + "\n\nOperation aborted") ;
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
		getDatas m exportOptions.flipYZ exportOptions.scale exportOptions.flipNormal exportOptions.exportUV exportOptions.exportColours sk phy;
		
		-- writing in the file
		format "Writing in file %.mesh.xml : \n" out_name ;
		WriteM exportOptions.exportUV exportOptions.exportColours pmesh.material out_name ;
		
		format "Export successful.\n"		
		
		delete m ;		
		
		format "-----                                -----\n"
		format "----------          END          ---------\n"
		format "------------------------------------------\n"
		
		MessageBox ("Exporting mesh successful !") ;
		
	)
	return true ;
)



