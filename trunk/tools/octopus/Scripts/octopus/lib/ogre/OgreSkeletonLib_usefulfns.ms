----------------------------------------------------------------------------------------
-- ------------------------------ SOME USEFUL FUNCTIONS ----------------------------- --
----------------------------------------------------------------------------------------


--------------------------------------------------------------------
-- compute the transform, if you want to flip Y and Z axis,
-- because the axis which 'defines height' in OGRE is the Y axis,
-- whereas 3dsmax uses the Z one.
--------------------------------------------------------------------
function flipYZTransform Tform = (
	local axis1,axis2,axis3,t,m
	
	-- computes the matrix
	axis1 = point3 1 0 0 ;
	axis2 = point3 0 0 1 ;
	axis3 = point3 0 -1 0 ;
	t = point3 0 0 0 ;
	m=matrix3 axis1 axis2 axis3 t ;
	
	-- multiplies by the inverse
	Tform = Tform*inverse(m) ;

	return Tform ;
)


--------------------------------------------------------------------
-- returns if the bone is the root or not,
-- ia if its parent is undefined or is not a bone.
--------------------------------------------------------------------
function isRoot b = (
	if (b.parent==undefined or not (iskindof b.parent BoneGeometry or iskindOf b.parent Biped_Object) ) then
		return true ;
	else
		return false ;
	
)

--------------------------------------------------------------------
-- creates a new array (which must be set up as an array before
-- calling this function) in which there isn't the same element.
-- Moreover, the array is sorted.
--------------------------------------------------------------------

function keepLoneValues a b= (
	local e, last_e ;
	sort a ;
	last_e = undefined ;
	for e in a do (
		if (e!=last_e) then 
			append b e ;
		last_e = e ;
	)
)

---------------------------------------------------------------------
-- replaces " " by "_" in a string.
-- when a name is for example Left Biceps max knows it at Left_Biceps
-- and execute fonction will not work if you don't use this function
---------------------------------------------------------------------
function replaceSpaces s =
(
	for i=1 to s.count do
	(
		if (s[i] == " ") then
			s[i] = "_" ;
	)
	s ;
)


-- point3[i] don't exist in 3dsmax3 so here is a function which get x,y or z
function p3get p i =
(
	if i==1 then p.x ;
	else if i==2 then p.y ;
	else p.z ;
)

-- point3[i] don't exist in 3dsmax3 so here is a function which set x,y or z
function p3set p i j=
(
	if i==1 then p.x = j ;
	else if i==2 then p.y = j ;
	else p.z = j;
)
--------------------------------
-- return the length of an array
--------------------------------
function arrayLength a = 
(
	local i ;
	i = 1 ;
	while (a[i] != undefined) do
		i = i + 1 ;	
	i-1 ;
)
-----------------------------------------------------------------------------
-- return the skin modifier or undefined if object don't have a skin modifier
-----------------------------------------------------------------------------

function getSkin obj =
(
	local s,i ;
	s = undefined ;
	if obj != undefined then
		for i in obj.modifiers do
		(
			if iskindof i Skin do
				s = i ;
		)
	s ;
)


-----------------------------------------------------------------------------
-- return the physique modifier or undefined if object don't have it
-----------------------------------------------------------------------------

function getPhysique obj =
(
	local s,i ;
	s = undefined ;
	if obj != undefined then
		for i in obj.modifiers do
		(
			if iskindof i Physique do
				s = i ;
		)
	s ;
)

--------------------------------------------------
-- return an Array with the root bones of the skin
--------------------------------------------------

function getRoots skin =
(
	local rootstab,n,i,c,d ;
	rootstab = #() ;
	n = skinOps.GetNumberBones skin ;
	for i = 1 to n do
	(
		c= skinOps.GetBoneName skin i 1 ;
		replaceSpaces c ;
		d = execute ("$" + c) ;
		if (isRoot d) then
			append rootstab d ;
	)
	rootstab ;
)

--------------------------------------------------------
-- return an Array with the ID of root bones of the skin
--------------------------------------------------------

function getRootsId skin =
(
	local rootstab,n,i,c,d ;
	rootstab = #() ;
	n = skinOps.GetNumberBones skin ;
	for i = 1 to n do
	(
		c= skinOps.GetBoneName skin i 1 ;
		replaceSpaces c ;
		d = execute ("$" + c) ;
		if (isRoot d) then
			append rootstab i ;
	)
	rootstab ;
)

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

