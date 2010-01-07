-------------------------------------------------------
---- 3dsmax4: 
---- 3dsmax5: place this in the plugins\Havok directory
-------------------------------------------------------

------------------------------------------------------------------------------
-- rctUtilityFunctions.ms
-- Helper functions for the macroscript "rctMacros.ms"
-- Havok 2003
------------------------------------------------------------------------------

-- Fix for localized versions, gets a float user property regardless of "." or "," used for decimals

fn rct_getFloatUserProp node propName =
(
	value = (getUserProp node propName) as string
	
	commaPos = findString value ","
	
	if (commaPos != Undefined) then
	(
		value = replace value commaPos 1 "."
	)
	
	return value as float
)

-- Filters the given node list so only rigid bodies remain
fn rct_filterOnlyRigidBodies objectList =
(
	filteredList = #()
	
	for object in objectList do
	(
		if (rct_isRigidBody object) then
		(
			append filteredList object
		)
	)
	
	return filteredList
)

-- Given a list of objects, it creates contraints among those that are parent-child
-- It returns a list of the constraints created
fn rct_constrainLinkedObjects objectList constraintType disableCollisions =
(	
	constraintList = #()
	
	for childObject in objectList do
	(
		parentObject = childObject.parent
		if findItem objectList parentObject != 0 then
		(
			newConstraint = constraintType()
			newConstraint.parentBody = parentObject
			newConstraint.childBody = childObject
			newConstraint.hasParent = true
			append constraintList newConstraint
			if (disableCollisions) then
			(
				reactor.disableCollisions parentObject childObject
			)
		)
	)
	
	return constraintList
)	


-- returns true if the object is a valid rigid body
fn rct_isRigidBody obj = 
(
	---------------------------------------------------------------------------------
	-- Spotlight targets and camera targets etc. are also GeometryClass's in max
	-- but are not valid rigidbodies so we filter them out of the selection
	---------------------------------------------------------------------------------
	
	if ((findString obj.name ".Target") != undefined) then return false
	
	if isKindOf obj GeometryClass and not isGroupMember obj then return true

	if (classOf obj == PlaneHelper) then return true

	---------------------------------------------------------------------------------
	-- If the selection is actually a grouphead then it is also a valid rigidbody:
	---------------------------------------------------------------------------------
	
	if isGroupHead obj then 
	(
		if obj.children.count < 1 then return false
		for obj_child in obj.children do
		(
			if isKindOf obj_child GeometryClass and isGroupMember obj_child then return true
		) 
	)
	return false
)

------------------------------------------------------------------
---- sort the valid RigidBodies in order w.r.t the chosen direction
------------------------------------------------------------------

fn rct_sortAlongPrincipalAxis objectList =
(
	--------------------------------------------------------------------------
	---- find the required direction in which to chain the selected rigidbodies
	--------------------------------------------------------------------------
	
	local chain_direction = rct_getLongestEdgeOfSelection()

	-------------------------------------------------
	---- remove any non-rigidbodies from the selection 
	-------------------------------------------------
		
	if chain_direction == 1 then
		qsort objectList rct_compareGeomsXPosFN 
	else if chain_direction == 2 then
		qsort objectList rct_compareGeomsYPosFN 
	else
		qsort objectList rct_compareGeomsZPosFN 
	
	----------------------------------
	---- return the sorted&pruned list
	----------------------------------
	return objectList
)
	
fn rct_sortSelectionAlongPrincipleAxis =
(
	--------------------------------------------------------------------------
	---- find the required direction in which to chain the selected rigidbodies
	--------------------------------------------------------------------------
	
	local chain_direction = rct_getLongestEdgeOfSelection()

	local current_sel = getCurrentSelection() 

	-------------------------------------------------
	---- remove any non-rigidbodies from the selection 
	-------------------------------------------------
	
	local filtered_bodies = #()
	for i = 1 to current_sel.count do
	(	
		if rct_isRigidBody current_sel[i] then
		(
			append filtered_bodies current_sel[i]
		)
	)
	
	if chain_direction == 1 then
		qsort filtered_bodies rct_compareGeomsXPosFN 
	else if chain_direction == 2 then
		qsort filtered_bodies rct_compareGeomsYPosFN 
	else
		qsort filtered_bodies rct_compareGeomsZPosFN 
	
	----------------------------------
	---- return the sorted&pruned list
	----------------------------------
	
	filtered_bodies 	
)

fn rct_getLongestEdgeOfThisSelection geoms =
(
	if undefined == geoms or geoms.count <= 0 then return 1
 
	local x_values = #()
	local y_values = #()
	local z_values = #()
	
	for g in geoms do
	(
		if (not undefined == g) and (not isDeleted g) then
		(
			append x_values g.pos.x
			append y_values g.pos.y
			append z_values g.pos.z
		)
	)
	
	format "gg %\n" geoms
	
	-- return a default direction of 1 if we have 
	-- no x_values - this is a rare case
	if  x_values.count <= 0 then return 1

	format "..\n"
	local x_range = amax x_values - amin x_values
	local y_range = amax y_values - amin y_values
	local z_range = amax z_values - amin z_values
	format "...\n"
 	---------------------------------------------------------------------
 	---- determine the most logical direction in which to apply the helper
 	---------------------------------------------------------------------
 	local longest_direction = 1
	if y_range > x_range then longest_direction = 2
	if z_range > y_range and z_range > x_range then longest_direction = 3
	format "....\n"
	-----------------------------------------------
	---- return the longest edge of BB of selection
	-----------------------------------------------
	longest_direction
)


--------------------------------------------------------------------------
---- sort the USER SUPPLIED Geometries in order w.r.t the chosen direction
--------------------------------------------------------------------------

fn rct_sortThisSelectionAlongPrincipleAxis geoms =
(
	if geoms.count <= 1 then
	(
		return geoms
	)
	
	--------------------------------------------------------------------------
	---- it is necessary to scan the list for <deleted scene node>s so that
	---- we do not generate exceptions
	--------------------------------------------------------------------------
	local clean_geoms = #()
	for g in geoms do
	(
		if not isDeleted g then append clean_geoms g
	)
		
	--------------------------------------------------------------------------
	---- find the required direction in which to chain the selected geometries
	--------------------------------------------------------------------------
	local chain_direction = getLongestEdgeOfThisSelection(clean_geoms)
	
	local sorted_geoms = #()
	
	for i = 1 to clean_geoms.count do
	(	
		append sorted_geoms clean_geoms[i]
	)
	
	case of
	(
		(chain_direction == 1): qsort sorted_geoms compareGeomsXPosFN 
		(chain_direction == 2): qsort sorted_geoms compareGeomsYPosFN 
		(chain_direction == 3): qsort sorted_geoms compareGeomsZPosFN 
	)
	
	----------------------------------
	---- return the sorted list
	----------------------------------

	sorted_geoms	
)


fn rct_getLongestEdgeOfSelection =
(
	local longest_direction = 1
	local p3_selection_range = selection.max - selection.min
	
	---------------------------
	---- remove any minus signs	 
	---------------------------
	
	if p3_selection_range.x	< 0 then p3_selection_range.x == -p3_selection_range.x
	if p3_selection_range.y	< 0 then p3_selection_range.y == -p3_selection_range.y
	if p3_selection_range.z	< 0 then p3_selection_range.z == -p3_selection_range.z
 
 	---------------------------------------------------------------------
 	---- determine the most logical direction in which to apply the helper
 	---------------------------------------------------------------------
 	
	if p3_selection_range.y > p3_selection_range.x then longest_direction = 2
	if p3_selection_range.z > p3_selection_range.y then 
	(
		if p3_selection_range.z > p3_selection_range.x then longest_direction = 3
	)
	
	-----------------------------------------------
	---- return the longest edge of BB of selection
	-----------------------------------------------
	
	longest_direction
)



---------------------------------------------
---- utility functions for sorting Geometries 
---- w.r.t. their position along the 
---- globally defined chain_direction
---------------------------------------------

fn rct_compareGeomsXPosFN v1 v2 = 
( 
	local d = (v1.pos[1]) - (v2.pos[1])
	
	case of 
	( 
		(d < 0.): -1	-- swap 
		(d >= 0.): 1 	-- leave
		default: 0 	-- leave
	) 
)

fn rct_compareGeomsYPosFN v1 v2 = 
( 
	local d = (v1.pos[2]) - (v2.pos[2])
	
	case of 
	( 
		(d < 0.): -1 
		(d >= 0.): 1 
		default: 0 
	) 
)

fn rct_compareGeomsZPosFN v1 v2 = 
( 
	local d = (v1.pos[3]) - (v2.pos[3])
	
	case of 
	( 
		(d < 0.): -1 
		(d >= 0.): 1 
		default: 0 
	) 
)



---------------------------------------------------------------------------------
-- isPrimitive: 
---------------------------------------------------------------------------------

fn rct_isPrimitive obj = 
(
	---------------------------------------------------------------------------------
	-- Spotlight targets and camera targets etc. are also GeometryClass's in max
	-- but are not valid rigidbodies so we filter them out of the selection
	---------------------------------------------------------------------------------
	if (obj.name == undefined) then return false
	if ((findString obj.name ".Target") != undefined) then return false
	
	if (classOf obj == PlaneHelper) then return true

	if isGroupMember obj and isKindOf obj GeometryClass then 
	(
		return true
	)
	else
	(
		return false
	)
)


fn rct_extractRigidBodies bodyList =
(
	local rigidBodyList = #()
	for o in bodyList do
	(
		if (isRigidBody o) then 
		(
			append rigidBodyList o
		)
	)
	rigidBodyList
)

fn rct_getGroupFromMember member =
(
	local search = true
	while search == true do 		
	(
		if isGroupHead member then
		(
			return member
		)
		else
		(
			member = member.parent
			search = false
		)
	)
	
	member
)

fn rct_averagePositions nodeList =
(
	local total = [0,0,0]
	
	if (nodeList.count==0) then
	(
		return [0,0,0]
	)
	else
	(
		for n in nodeList do
		(
			total = total + n.center
		)
		
		return (total / nodeList.count)
	)
)

