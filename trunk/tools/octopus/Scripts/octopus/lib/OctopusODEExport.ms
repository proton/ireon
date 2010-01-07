XverticesArrays=#() ;XfacesArrays=#() ;
XsubmeshesId=#() ;
XogreVertIndex=#() ;
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
function makeTrimesh tmesh mtrx flipyz =
(
	XverticesArrays=#() ;
	XfacesArrays=#() ;
	XsubmeshesId = #() ;
	XogreVertIndex=#() ;

	matId = 1;
	XverticesArrays[matId] = #()
	XfacesArrays[matId] = #()
	
	for i = 1 to (getnumverts tmesh) do 
	(
		vert = getVert tmesh i;
		-- change scale
		-- vert = vert * scale ;
				
		localpoint = point3 vert[1] vert[2] vert[3]
		localpoint = localpoint * mtrx
		vert[1] = localpoint[1]
		vert[2] = localpoint[2]
		vert[3] = localpoint[3]
			
		-- flip axes		
		if (flipyz) then
		(
			vertTmp = copy vert ;
			vert[2] = vert[3] ;
			vert[3] = -vertTmp[2] ;
		)
				
		append XverticesArrays[matId] #(vert[1],vert[2],vert[3]) ;   -- Vertex is added to datas
	)

	
	for i = 1 to (getnumfaces tmesh) do
	(
		f = getface tmesh i ;
		append XfacesArrays[matId] #(f[1],f[2],f[3]);
	)
)
----------------------------------------------------------
-----------------------------------------------------------
-----------------------------------------------------------
function writeTrimesh objname tabstring unitsScale flipyz ScriptX =
(
 	matId = 1
	use32bitindexes = "false";
	if arrayLength XverticesArrays[matId] > 65535 then
		use32bitindexes = "true";

	format "%\t<trimesh>\n" tabstring to:ScriptX
	format "%\t\t<vertices>\n" tabstring to:ScriptX

		-- for each vertex
	for v in XverticesArrays[matId] do
	(	
--		v[1] = v[1] / unitsScale
--		v[2] = v[2] / unitsScale
--		v[3] = v[3] / unitsScale
		if (flipyz) then
		(
			vy = v[2];
			v[2] = v[3] ;
			v[3] = -vy ;
		)
		format "%\t\t\t<v x=\"%\" y=\"%\" z=\"%\" />\n" tabstring v[1] v[2] v[3] to:ScriptX 
	)	
	format "%\t\t</vertices>\n" tabstring to:ScriptX
	format "%\t\t<triangles>\n" tabstring to:ScriptX

		-- for each face
	for f in XfacesArrays[matId] do 
	(
		ia = ((int)f[1]) ;
		ib = ((int)f[2]) ;
		ic = ((int)f[3]) ;
		-- subtract 1, because here the indices are from 1..n while in XODE they're 0..n-1 !
		format "%\t\t\t<t ia=\"%\" ib=\"%\" ic=\"%\" />\n" tabstring (ia-1) (ib-1) (ic-1) to:ScriptX
	)
	format "%\t\t</triangles>\n" tabstring to:ScriptX

	format "%\t</trimesh>\n" tabstring to:ScriptX
)
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
function makeTrimeshWithSub tmesh mtrx flipyz =
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
	XverticesArrays=#() ;
	XfacesArrays=#() ;
	XsubmeshesId = #() ;
	XogreVertIndex=#() ;

	-- For each face
	for i = 1 to (getnumfaces tmesh) do -- For each face
	(		
		
		f = getface tmesh i ;
		matId = getFaceMatId tmesh i ;		
		
		-- For each vertex whose face is made up of.
		for j=1 to 3 do (			
		    a = (int) (f[j]) ;
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
				ogreVertNum = triplet[1] ;					
			)

			if (ogreVertNum == -1) then -- That is to say it has not been found. 
			(
				-- the vertex number for the current submesh must be updated
				
				-- if undefined, the array is created
				if (XogreVertIndex[matId] == undefined ) then
					XogreVertIndex[matId]=1 ;
				
				-- vertex number is increased.								
				ogreVertNum = XogreVertIndex[matId] ;
				XogreVertIndex[matId] = XogreVertIndex[matId] + 1 ;
				
				-- it is added to the search table.
				append searchTable[a] #(ogreVertNum) ;
				
				-- we retrieve data
				vert = getVert tmesh f[j] ;
				-- change scale
--				vert = vert * scale ;
				
				localpoint = point3 vert[1] vert[2] vert[3]
				localpoint = localpoint * mtrx
				vert[1] = localpoint[1]
				vert[2] = localpoint[2]
				vert[3] = localpoint[3]
		
				-- flip axes		
				if (flipyz) then
				(
					vertTmp = copy vert ;
					vert[2] = vertTmp[3] ;
					vert[3] = -vertTmp[2] ;
				)
				
				-- Maybe the material found defines a new submesh...
				if (XverticesArrays[matId] == undefined) then
				(
					format "- Submesh detected: material ID = %\n" matId
					XverticesArrays[matId] = #() ;
					boneAssignments[matId] = #() ;
					append XsubmeshesId matId ;
				)
				append XverticesArrays[matId] #(vert[1],vert[2],vert[3]) ;   -- Vertex is added to datas
			)
			ogreFace[j] = ogreVertNum ;	
		)
		
		-- flip normal capability
--		if (flipNormal) then
--		(
--			faceTmp = copy ogreFace ;
--			ogreFace[2] = faceTmp[3] ;
--			ogreFace[3] = faceTmp[2] ;
--		)

		if (XfacesArrays[matId] == undefined) then
			XfacesArrays[matId] = #() ;		
		append XfacesArrays[matId] #(ogreFace[1],ogreFace[2],ogreFace[3]) ;   -- Face is added to datas
	)
)
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
function writeTrimeshWithSub objname tabstring ScriptX =
(
	for matId in XsubmeshesId do 
	(
		use32bitindexes = "false";
		if arrayLength XverticesArrays[matId] > 65535 then
			use32bitindexes = "true";
	
		format "%<geom name=\"%.geom.%\">\n" tabstring objname matId to:ScriptX
		format "%\t<trimesh>\n" tabstring to:ScriptX
		format "%\t\t<vertices>\n" tabstring to:ScriptX

		-- for each vertex
		for v in XverticesArrays[matId] do
		(	
			format "%\t\t\t<v x=\"%\" y=\"%\" z=\"%\" />\n" tabstring v[1] v[2] v[3] to:ScriptX 
		)	
		format "%\t\t</vertices>\n" tabstring to:ScriptX
		format "%\t\t<triangles>\n" tabstring to:ScriptX

		-- for each face
		for f in XfacesArrays[matId] do 
		(
			format "%\t\t\t<t ia=\"%\" ib=\"%\" ic=\"%\" />\n" ((int)f[1]) ((int)f[2]) ((int)f[3]) tabstring to:ScriptX
		)
		format "%\t\t</triangles>\n" tabstring to:ScriptX

		format "%\t</trimesh>\n" tabstring to:ScriptX
		format "%</geom>\n" tabstring to:ScriptX
	)
)
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
function writeJoint obj mod unitsScale ScriptX =
(
	tabstring = "\t\t\t\t"
	local str_jointtype = #("amotor","ball","fixed","hinge","hinge2","slider","universal")
	local str_paramnames = #("lowStop","hiStop","vel","fMax","fudgeFactor","bounce","CFM","stopERP","stopCFM","suspensionERP","suspensionCFM")
	
	if (mod.m_isJoint == true) then
	(
		lpos = in coordsys parent obj.pos
		lpos = lpos / unitsScale
		lquat = in coordsys parent obj.rotation
		lscale = in coordsys parent obj.scale

		format "%<joint name=\"%.joint\">\n" tabstring obj.name to:ScriptX
		format "%\t<transform>\n" tabstring to:ScriptX
		format "%\t\t<position x=\"%\" y=\"%\" z=\"%\" />\n" tabstring lpos.x lpos.y lpos.z to:ScriptX
		format "%\t\t<rotation>\n" tabstring to:ScriptX
		format "%\t\t\t<quaternion x=\"%\" y=\"%\" z=\"%\" w=\"%\" />\n" tabstring lquat.x lquat.y lquat.z lquat.w to:ScriptX
		format "%\t\t</rotation>\n" tabstring to:ScriptX
		format "%\t\t<scale x=\"%\" y=\"%\" z=\"%\" />\n" tabstring lscale.x lscale.y lscale.z to:ScriptX
		format "%\t</transform>\n" tabstring to:ScriptX
        
		format "%\t<link2 body=\"%\" />\n" tabstring obj.name to:ScriptX
		
		format "%\t<%>\n" tabstring str_jointtype[mod.m_jointType] to:ScriptX
		
		if ((mod.m_jointType == 2) or (mod.m_jointType == 4) or (mod.m_jointType == 5)) do
		(
			format "%\t\t<anchor x=\"0.0\" y=\"0.0\" z=\"0.0\" absolute=\"false\" />\n" tabstring to:ScriptX
		)
		
		if ((mod.m_jointType == 1) or (mod.m_jointType > 4)) do
		(
        	format "%\t\t<axis x=\"0\" y=\"0\" z=\"1\"" tabstring to:ScriptX
			for i = 1 to 11 do
			(
				format "\n%\t\t\t%=\"%\"" tabstring str_paramnames[i] mod.m_a1Params[i] to:ScriptX
			)
			format " >\n" to:ScriptX
			format "%\t\t</axis>\n" tabstring to:ScriptX
		)
		
		if ((mod.m_jointType == 1) or (mod.m_jointType == 5) or (mod.m_jointType == 7)) do
		(
        	format "%\t\t<axis x=\"0\" y=\"1\" z=\"0\"" tabstring to:ScriptX
			for i = 1 to 11 do
			(
				format "\n%\t\t\t%=\"%\"" tabstring str_paramnames[i] mod.m_a2Params[i] to:ScriptX
			)
			format " >\n" to:ScriptX
			format "%\t\t</axis>\n" tabstring to:ScriptX
		)
		
		if (mod.m_jointType == 1) do
		(
        	format "%\t\t<axis x=\"1\" y=\"0\" z=\"0\"" tabstring to:ScriptX
			for i = 1 to 11 do
			(
				format "\n%\t\t\t%=\"%\"" tabstring str_paramnames[i] mod.m_a3Params[i] to:ScriptX
			)
			format " >\n" to:ScriptX
			format "%\t\t</axis>\n" tabstring to:ScriptX
		)
		
		format "%\t</%>\n" tabstring str_jointtype[mod.m_jointType] to:ScriptX
                
	    format "%</joint>\n" tabstring to:ScriptX
	)
)
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
function writeMassMatrix obj tabstring unitsScale ScriptX =
(
	lpos = in coordsys parent obj.pos
	lpos = lpos / unitsScale

	format "%<mass_struct total=\"%\">\n" tabstring obj.m_total to:ScriptX
	format "%\t<centerGravity x=\"%\" y=\"%\" z=\"%\" />\n" tabstring lpos.x lpos.y lpos.z to:ScriptX
	format "%\t<imatrix\n" tabstring to:ScriptX
	format "%\t\tm00=\"%\" m01=\"%\" m02=\"%\"\n" tabstring obj.m_mmx1[1] obj.m_mmx1[2] obj.m_mmx1[3] to:ScriptX
	format "%\t\tm10=\"%\" m11=\"%\" m12=\"%\"\n" tabstring obj.m_mmx2[1] obj.m_mmx2[2] obj.m_mmx2[3] to:ScriptX
	format "%\t\tm20=\"%\" m21=\"%\" m22=\"%\"\n" tabstring obj.m_mmx3[1] obj.m_mmx3[2] obj.m_mmx3[3] to:ScriptX
	format "%\t/>\n" tabstring to:ScriptX
	format "%</mass_struct>\n" tabstring to:ScriptX
) 
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
function writeMassBox obj tabstring unitsScale ScriptX =
(
	format "%<mass_shape density=\"%\">\n" tabstring obj.m_density to:ScriptX
	format "%\t<box\n" tabstring to:ScriptX
	format "%\t\tsizex=\"%\"\n" tabstring (obj.delegate.width/unitsScale) to:ScriptX
	format "%\t\tsizey=\"%\"\n" tabstring (obj.delegate.length/unitsScale) to:ScriptX
	format "%\t\tsizez=\"%\"\n" tabstring (obj.delegate.height/unitsScale) to:ScriptX
	format "%\t/>\n" tabstring to:ScriptX
	format "%</mass_shape>\n" tabstring to:ScriptX
) 
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
function writeMassSphere obj tabstring unitsScale ScriptX =
(
	format "%<mass_shape density=\"%\">\n" tabstring obj.m_density to:ScriptX
	format "%\t<sphere\n" tabstring to:ScriptX
	format "%\t\tradius=\"%\"\n" tabstring (obj.delegate.radius/unitsScale) to:ScriptX
	format "%\t/>\n" tabstring to:ScriptX
	format "%</mass_shape>\n" tabstring to:ScriptX
) 
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
function writeMassCyl obj tabstring unitsScale ScriptX =
(
	format "%<mass_shape density=\"%\">\n" tabstring obj.m_density to:ScriptX
	format "%\t<cylinder\n" tabstring to:ScriptX
	format "%\t\tradius=\"%\"\n" tabstring (obj.delegate.radius/unitsScale) to:ScriptX
	format "%\t\tlength=\"%\"\n" tabstring (obj.delegate.height/unitsScale) to:ScriptX
	format "%\t/>\n" tabstring to:ScriptX
	format "%</mass_shape>\n" tabstring to:ScriptX
)
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
function exportPhysHeader sceneName ScriptX =
(
	format "<xode version=\"1.0r22\" name=\"%\">\n" sceneName to:ScriptX
	format "\t<world>\n" to:ScriptX
	format "\t\t<space>\n\n" to:ScriptX
)
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
function exportPhysFooter ScriptX =
(
	format "\t\t</space>\n" to:ScriptX
	format "\t</world>\n" to:ScriptX
	format "</xode>\n" to:ScriptX
)
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
function exportGeom obj mod staticGeom unitsScale flipyz ScriptX =
(
	if (staticGeom) then
		tabstring = "\t\t\t"
	else
		tabstring = "\t\t\t\t"
		
	format "%<geom name=\"%.geom\">\n" tabstring obj.name to:ScriptX
	
	staticScaleX = 1.0
	staticScaleY = 1.0
	staticScaleZ = 1.0
	isPlane = false
	if ((mod.m_bodyShape == 6) and (classOf obj.BaseObject == plane)) then
		isPlane = true


	if (staticGeom and not isPlane) do
	(
		lpos = in coordsys world obj.pos
		lpos = lpos/unitsScale
		lquat = in coordsys world obj.rotation
		lscale = in coordsys world obj.scale
		
		if (flipyz) then
		(
			lposTmp = copy lpos ;
			lpos[2] = lpos[3];
			lpos[3] = -lposTmp[2];
		)
	
		format "%\t<transform>\n" tabstring to:ScriptX
		format "%\t\t<position x=\"%\" y=\"%\" z=\"%\" />\n" tabstring lpos.x lpos.y lpos.z to:ScriptX
		format "%\t\t<rotation>\n" tabstring to:ScriptX
		format "%\t\t\t<quaternion x=\"%\" y=\"%\" z=\"%\" w=\"%\" />\n" tabstring lquat.x lquat.y lquat.z lquat.w to:ScriptX
		format "%\t\t</rotation>\n" tabstring to:ScriptX
    	format "%\t\t<scale x=\"%\" y=\"%\" z=\"%\" />\n" tabstring lscale.x lscale.y lscale.z to:ScriptX	
		format "%\t</transform>\n" tabstring to:ScriptX
		
		staticScaleX = lscale.x
		staticScaleY = lscale.y
		staticScaleZ = lscale.z
	)
	--TODO: Think about transform !!!
	
	if ((mod.m_bodyShape == 2) and (classOf obj.BaseObject == box)) then
	(
		x = ((obj.width*staticScaleX)/unitsScale) ;
		y = ((obj.length*staticScaleY)/unitsScale) ;
		z = ((obj.height*staticScaleZ)/unitsScale) ;
		if (flipyz) then
		(
			tmp = copy y;
			y = z;
			z = tmp;
		)
		format "%\t<box\n" tabstring to:ScriptX
		format "%\t\tsizex=\"%\"\n" tabstring x to:ScriptX
		format "%\t\tsizey=\"%\"\n" tabstring y to:ScriptX
		format "%\t\tsizez=\"%\"\n" tabstring z to:ScriptX
		format "%\t/>\n" tabstring to:ScriptX
	)
	else
	if ((mod.m_bodyShape == 3) and (classOf obj.BaseObject == cylinder)) then
	(
		format "%\t<cylinder\n" tabstring to:ScriptX
		format "%\t\tradius=\"%\"\n" tabstring ((obj.radius*staticScaleX)/unitsScale) to:ScriptX
		format "%\t\tlength=\"%\"\n" tabstring ((obj.height*staticScaleZ)/unitsScale) to:ScriptX
		format "%\t/>\n" tabstring to:ScriptX
	)
	else
	if ( (mod.m_bodyShape == 4) and ((classOf obj.BaseObject == sphere) or (classOf obj.BaseObject == geosphere)) )then
	(
		format "%\t<sphere\n" tabstring to:ScriptX
		format "%\t\tradius=\"%\"\n" tabstring ((obj.radius*staticScaleX)/unitsScale) to:ScriptX
		format "%\t/>\n" tabstring to:ScriptX
	)
	else
	if ((mod.m_bodyShape == 5) and (classOf obj.BaseObject == cone)) then
	(
		format "%\t<cone\n" tabstring to:ScriptX
		format "%\t\tradius=\"%\"\n" tabstring ((obj.radius1*staticScaleX)/unitsScale) to:ScriptX
		format "%\t\tlength=\"%\"\n" tabstring ((obj.height*staticScaleZ)/unitsScale) to:ScriptX
		format "%\t/>\n" tabstring to:ScriptX
	)
	else
	if ((mod.m_bodyShape == 6) and (classOf obj.BaseObject == plane)) then
	(
		format "%\t<plane\n" tabstring to:ScriptX
		format "%\t\ta=\"0\" b=\"1\" c=\"0\" d=\"-3\"\n" tabstring to:ScriptX
		format "%\t/>\n" tabstring to:ScriptX
	)
	else
	(
		m = snapshotAsMesh obj;	
--		wmatrix = inverse obj.transform
		tmatrix = obj.transform
		translate tmatrix (obj.transform.translationpart * -1.0)
		translate tmatrix (obj.transform.translationpart / unitsScale )
		wmatrix = inverse tmatrix
--		scale wmatrix obj.transform.scalepart true
		makeTrimesh m wmatrix false
		writeTrimesh obj.name tabstring unitsScale flipyz ScriptX
	)

	format "%</geom>\n" tabstring to:ScriptX
)
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
function isMassObject obj =
(
	if ((classOf obj) == ODEBoxMass) then return true
	else
	if ((classOf obj) == ODESphereMass) then return true
	else
	if ((classOf obj) == ODECylMass) then return true
	else
	if ((classOf obj) == ODEStructMass) then return true
	else
		return false
)
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------

function WriteAnyMass obj tabstring unitsScale ScriptX =
(
	lpos = in coordsys parent obj.pos
	lpos = lpos / unitsScale
	lquat = in coordsys parent obj.rotation

	format "%<mass>\n" tabstring to:ScriptX
	writeTransform = false
	
	if ((classOf obj) == ODEBoxMass) then
	(
		writeTransform = true
		writeMassBox obj (tabstring+"\t") unitsScale ScriptX
	)
	else
	if ((classOf obj) == ODESphereMass) then
	(
		writeTransform = true
		writeMassSphere obj (tabstring+"\t") unitsScale ScriptX
	)
	else
	if ((classOf obj) == ODECylMass) then
	(
		writeTransform = true
		writeMassCyl obj (tabstring+"\t") unitsScale ScriptX
	)
	else
	if ((classOf obj) == ODEStructMass) then
	(
		writeTransform = false
		writeMassMatrix obj (tabstring+"\t") unitsScale ScriptX
	)
	
	if (writeTransform) do
	(
		format "%\t<transform>\n" tabstring to:ScriptX
		format "%\t\t<position x=\"%\" y=\"%\" z=\"%\" />\n" tabstring lpos.x lpos.y lpos.z to:ScriptX
		format "%\t\t<rotation>\n" tabstring to:ScriptX
		format "%\t\t\t<quaternion x=\"%\" y=\"%\" z=\"%\" w=\"%\" />\n" tabstring lquat.x lquat.y lquat.z lquat.w to:ScriptX
		format "%\t\t</rotation>\n" tabstring to:ScriptX
		format "%\t</transform>\n" tabstring to:ScriptX
	)
	
	for chi in obj.children do
	(
		if (chi.parent == obj) then -- only the first level children !
		(
			if (isMassObject chi) then
			(
				WriteAnyMass chi (tabstring+"\t") ScriptX
			)
		)
	) --ENDOF for chi in obj.children do
	
	format "%</mass>\n" tabstring to:ScriptX
)
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
function exportPhysics obj mod unitsScale flipyz ScriptX =
(
	lpos = in coordsys world obj.pos
	lpos = lpos / unitsScale
	lquat = in coordsys world obj.rotation
	lscale = in coordsys world obj.scale
    tabstring = "\t\t\t"
	
	if (mod.m_isStatic) then
	(
		exportGeom obj mod true unitsScale flipyz ScriptX
	)
	else
	(
	
		format "%<body name=\"%\">\n" tabstring obj.name to:ScriptX

		outPos = copy lpos ;	
		if (flipyz) then
		(
			outPos[2] = lpos[3];
			outPos[3] = -lpos[2];
		)

		format "%\t<transform>\n" tabstring to:ScriptX
		format "%\t\t<position x=\"%\" y=\"%\" z=\"%\" />\n" tabstring outPos.x outPos.y outPos.z to:ScriptX
		format "%\t\t<rotation>\n" tabstring to:ScriptX
		format "%\t\t\t<quaternion x=\"%\" y=\"%\" z=\"%\" w=\"%\" />\n" tabstring lquat.x lquat.y lquat.z lquat.w to:ScriptX
		format "%\t\t</rotation>\n" tabstring to:ScriptX
	    format "%\t\t<scale x=\"%\" y=\"%\" z=\"%\" />\n" tabstring lscale.x lscale.y lscale.z to:ScriptX	
		format "%\t</transform>\n" tabstring to:ScriptX
	
		exportGeom obj mod false unitsScale flipyz ScriptX
		
		countMass = 0
		for chi in obj.children do
		(
			if (chi.parent == obj) then -- only the first level children !
			(
				if (isMassObject chi) do
					countMass = countMass + 1
			)
		)
		
		if (countMass == 1) then
		(
			for chi in obj.children do
			(
				if (chi.parent == obj) then -- only the first level children !
				(
					if (isMassObject chi) then
					(
						WriteAnyMass chi (tabstring+"\t") unitsScale ScriptX
					)
				)
			) --ENDOF for chi in obj.children do
		)
		else
		(
			format "%\t<mass>\n" tabstring to:ScriptX
		
			usesAjust = true
			for chi in obj.children do
			(
				if (chi.parent == obj) then -- only the first level children !
				(
					if (isMassObject chi) then
					(
						WriteAnyMass chi (tabstring+"\t\t") unitsScale ScriptX
						usesAjust = false
					)
				)
			) --ENDOF for chi in obj.children do
			
			if (usesAjust) do
			(
				format "%\t\t<adjust total=\"%\" />\n" tabstring mod.m_adjust to:ScriptX
			)
	
			format "%\t</mass>\n" tabstring to:ScriptX
		)
	
		for chi in obj.children do
		(
			for mod in chi.modifiers do
			(
				if (mod.name == "ODE Body") then
				(
					if (chi.parent == obj) then -- only the first level children !
					(
						writeJoint chi mod lscale ScriptX
					)
				)
			) --ENDOF for mod in obj.modifiers do
		) --ENDOF for chi in obj.children do
	
		format "%</body>\n\n" tabstring to:ScriptX
	)
)
