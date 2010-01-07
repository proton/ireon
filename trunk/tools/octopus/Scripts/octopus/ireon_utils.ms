-----------------------------------------------------------------
-- Global 'variables'
-- There is an array with vertices datas for each submesh
-- There is also an array made up of faces.
-----------------------------------------------------------------

verticesArrays=#() ;
facesArrays=#() ; 
degeneratedFacesArray=#() ;
deletedFacesArray=#() ;

function getVectorLength vector =
(
	squareLength = vector[1]*vector[1] + vector[2]*vector[2] + vector[3]*vector[3] ;
	length = sqrt squareLength ;
	return length ;

)

function dotProduct vector1 vector2=
(
	result = vector1[1]*vector2[1] + vector1[2]*vector2[2] + vector1[3]*vector2[3] ;
	return result ;

)


function setCoordinatesPrecision obj precision =
(

	numVerts = getNumVerts obj ;
	for i = 1 to numVerts  do -- For each vertex
	(
		v = getvert obj i ;
		vfloor = [(floor (v[1]*10))/10, (floor (v[2]*10))/10, (floor (v[3]*10))/10 ] ;
		setvert obj i vfloor ;
	)
)


function checkAndDeleteDegenerateFacesInEditableMesh obj =
(
	triangleMesh = snapshotAsMesh obj ;
	count = 1 ;
	oldNumFaces = getnumfaces triangleMesh ;
	for i = 1 to (getnumfaces triangleMesh ) do -- For each face
	(
		face = getface triangleMesh i ;
		for j=0 to 2 do 
		(
			-- vert = getVert triangleMesh f[j] ;
			
			-- ccw orientation triangle
			vertA = getVert triangleMesh face[1+int( mod j 3)]
			vertB = getVert triangleMesh face[1+int( mod (j+1) 3)]
			vertC = getVert triangleMesh face[1+int( mod (j+2) 3)]
			
			edgeVector1 = vertB - vertA ;
			edgeVector2 = vertC - vertB ;
  			edgeVector3 = vertA - vertC ;
			
			if getVectorLength edgeVector1  == (getVectorLength edgeVector2 + getVectorLength edgeVector3)
			then
			(
				format "--------------------------------------- \n" ;
				format "[%] Triangle is degenerated \n" i ;
				format "  face: [%] \n" face;
				format "  coor: [ (%), (%), (%) ]\n" (getVert triangleMesh face[1]) (getVert triangleMesh face[2]) (getVert triangleMesh face[3]) ;
				
				degeneratedFacesArray[count] = i ;
				count = count + 1 ;
				
				break ;					
			)
		)
	)
	format "degenerate faces: %\n" degeneratedFacesArray 
	if count > 1 then
	(
		format "delete faces... OK\n"
		for i = 1 to (count - 1 ) do
		(	
			format "% % \n" i obj.faces[#{degeneratedFacesArray[i] - i + 1}] ;
			deleteface obj (degeneratedFacesArray[i] - i + 1) ;
		)
	)
	triangleMesh = snapshotAsMesh obj ;
	newNumFaces = getnumfaces triangleMesh ;
	format "\n\n before: %       after: %\n"  oldNumFaces 	newNumFaces ;
	
)

function checkAndDeleteNegativeNormalFacesInEditableMesh obj angle =
(
	triangleMesh = snapshotAsMesh obj ;
	count = 1 ;
	oldNumFaces = getnumfaces triangleMesh ;
	for i = 1 to (getnumfaces triangleMesh ) do -- For each face
	(
		face = getface triangleMesh i ;
		normal = getFaceNormal triangleMesh i ;
		if (dotProduct normal (point3 0 0 1)) <= (tan (float angle) ) 
			then
			(
				format "--------------------------------------- \n" ;
				format "[%] Triangle has downed normal\n" i ;
				format "  face: [%] \n" face;
				format "  coor: [ (%), (%), (%) ]\n" (getVert triangleMesh face[1]) (getVert triangleMesh face[2]) (getVert triangleMesh face[3]) ;
				format "normal: % \n" normal ;
				deletedFacesArray[count] = i ;
				count = count + 1 ;
				
				break ;					
			)
	)
	format "delete faces: %\n" degeneratedFacesArray 
	if count > 1 then
	(
		format "delete faces... OK\n"
		for i = 1 to (count - 1 ) do
		(	
			format "% % \n" i obj.faces[#{deletedFacesArray[i] - i + 1}] ;
			deleteface obj (deletedFacesArray[i] - i + 1) ;
		)
	)
	triangleMesh = snapshotAsMesh obj ;
	newNumFaces = getnumfaces triangleMesh ;
	format "\n\n before: %       after: %\n"  oldNumFaces 	newNumFaces ;
	
)
setCoordinatesPrecision $ 1
checkAndDeleteDegenerateFacesInEditableMesh $
-- angle in degrees. faces with normal . (0, 0, 1) < = angle   deleting...
dangle = 5.0 ;
checkAndDeleteNegativeNormalFacesInEditableMesh  $ dangle
