---------------
-- USER OPTIONS
---------------

-- this definition should be the same as the
-- global RCT_CHARACTER_ROOT as defined in rctxMacros.ms
global RCT_CHARACTER_ROOT = "Pelvis"


-- when using constraints which don't lose energy
-- it's a good idea to add a drag action to the scene
global RCT_ADD_DRAG_ACTION = false

---------------
-- END OPTIONS
---------------

global RCT_MV = maxVersion()

global RCT_MAX4_FLOATER_EXTENTION = 0

if (RCT_MV[1] < 5000 ) then
(
	RCT_MAX4_FLOATER_EXTENTION = 30
)

global RCT_NUM_HINGE_ERRORS = 0
global RCT_NUM_RAGDOLL_ERRORS = 0

global RCT_RAGDOLL_BIPED_SCRIPT_ACTIVE = 0

-- RCT_GROUPS_PRESENT
-- tracks whether any groups are present 
-- in the current source

global RCT_GROUPS_PRESENT = false

function rctCreatePart name length width height transform offsetTransform type:#box =
(
	case type of
	(
		#sphere :
		(
			radius = 1
			if( (length < width) and (length < height) ) then
				radius = length
			else
				if( width < height ) then
					radius = width
				else
					radius = height

			radius = radius
			
			numLength	= (length / radius) as integer
			numWidth	= (width / radius) as integer
			numHeight	= (height / radius) as integer
						
			modLength 	= length - (radius * numLength)
			modWidth 	= width - (radius * numWidth)
			modHeight 	= height - (radius * numHeight)
			
			stepLength	= radius + (modLength / numLength)
			stepWidth	= radius + (modWidth / numWidth)
			stepHeight	= radius + (modHeight / numHeight)

			if( 0 != modLength ) then 	
			(
				stepLength	-= radius / numLength
				numLength	+= 1
			)
			if( 0 != modWidth ) then	
			(
				stepWidth	-= radius / numWidth
				numWidth	+= 1
			)
			if( 0 != modHeight ) then
			(
				stepHeight	-= radius / numHeight
				numHeight	+= 1
			)
			
			numSpheres	 = numLength * numWidth * numHeight
			radius 		*= 0.5
			
			spheres = #()
			for l = 0 to (numLength - 1) do
			(


				y = (l * stepLength) + radius
				for w = 0 to (numWidth - 1) do
				(
					x = (w * stepWidth) + radius
 					for h = 0 to (numHeight - 1) do
					(
						z = (h * stepHeight) + radius 
						s = sphere()
						s.radius = radius
						s.position  = [x, y, z] * offsetTransform
						
						append spheres s
					)
				)
			)
			
			part = group spheres

			pivotOffset	 = offsetTransform.translationPart 
			pivotOffset	*= offsetTransform.rotationPart
			pivotOffset	*= offsetTransform.scalePart
			part.pivot	-= pivotOffset 

			part.transform = transform
			part.name = name
		)

		#box :
		(
			part = box()
			part.length = length 
			part.width	= width 
			part.height = height

			part.objectOffsetRot	 = offsetTransform.rotationPart
			part.objectOffsetScale	 = offsetTransform.scalePart

			pivotOffset	 = [0, 0, -height * 0.5]
			pivotOffset	+= offsetTransform.translationPart 
			pivotOffset	*= offsetTransform.rotationPart
			pivotOffset	*= offsetTransform.scalePart
			part.pivot	-= pivotOffset 

			part.transform = transform
			part.name = name
		)
	)

	return part
)

function rctCreateParts bip height numVertebra:1 type:#box hands:true feet:true linkParts:false =
(
	-- OK Lets make a character studio-like ragdoll.
	
	-- Everything is going to measured in Head
	-- Character to be 8 heads in height and 2 1/3 heads wide.
	-- 1 Hand is equivalent to 2/3 of a Head
	
	UnitHead = height / 8.0
	UnitHand = UnitHead * 2.0 / 3.0

	bipRArm = #()
	bipLArm = #()
	bipRLeg = #()
	bipLLeg = #()
	bipSpineParts = #()
	
	-- Pelvis 
	l = 0.75 * UnitHead
	w = 0.5 * UnitHead * 1.75
	h = 0.75 * UnitHead
	ot = matrix3 1
	ot.row1 = [0, 0, 1]
	ot.row2 = [0,-1, 0]
	ot.row3 = [1, 0, 0]
	ot.row4 = [0, 0, 0]
	t = matrix3 1
	t.row1 = [0, 0, 1]
	t.row2 = [0,-1, 0]
	t.row3 = [1, 0, 0]
	t.row4 = [0, 0, 4.25 * UnitHead]
	bipPelvis = rctCreatePart (bip + " Pelvis") l w h t ot type:type
	append bipSpineParts bipPelvis
	
	-- Spine
	lastVertebra = bipPelvis
	
	for i = 0 to numVertebra - 1 do
	(
		-- Vertebra
		l = 0.6 * UnitHead
		w = 0.5 * UnitHead * 2
		h = 1.75 * UnitHead / numVertebra
		ot = matrix3 1
		ot.row1 = [0, 0, 1]
		ot.row2 = [0,-1, 0]
		ot.row3 = [1, 0, 0]
		ot.row4 = [0, 0, 0.5 * h]
		t = matrix3 1
		t.row1 = [0, 0, 1]
		t.row2 = [0,-1, 0]
		t.row3 = [1, 0, 0]
		t.row4 = [0, 0, (4.75 * UnitHead) + (i * (2.0 * UnitHead / numVertebra))]
		vertebra = rctCreatePart (bip + " Spine") l w h t ot type:type
		if( i > 0 ) then
		(
			vertebra.name = vertebra.name + (i as string)
		)
		append bipSpineParts vertebra
		
		if (linkParts) then
		(
			vertebra.parent = lastVertebra
		)
		
		lastVertebra = vertebra
	)

	-- Head - 1 Head
	l = UnitHead
	w = UnitHand
	h = UnitHead
	ot = matrix3 1
	ot.row1 = [0, 0, 1]
	ot.row2 = [0,-1, 0]
	ot.row3 = [1, 0, 0]
	ot.row4 = [0, -0.1 * l, 0.5 * h]
	t = matrix3 1
	t.row1 = [0, 0, 1]
	t.row2 = [0,-1, 0]
	t.row3 = [1, 0, 0]
	t.row4 = [0, 0, 7 * UnitHead]
	bipHead = rctCreatePart (bip + " Head") l w h t ot type:type
	append bipSpineParts bipHead
	
	
	-- Left Thigh
	l = 0.75 * UnitHead
	w = 0.5 * UnitHead
	h = 2.0 * UnitHead
	ot = matrix3 1
	ot.row1 = [0, 0, -1]
	ot.row2 = [0, -1, 0]
	ot.row3 = [-1, 0, 0]
	ot.row4 = [0, 0, -0.5 * h]
	t = matrix3 1
	t.row1 = [0, 0, -1]
	t.row2 = [0, -1, 0]
	t.row3 = [-1, 0, 0]
	t.row4 = [0.75 * w, 0, 4.25 * UnitHead]
	bipLThigh = rctCreatePart (bip + " L Thigh") l w h t ot type:type
	append bipLLeg bipLThigh
	
	
	-- Right Thigh
	l = 0.75 * UnitHead
	w = 0.5 * UnitHead
	h = 2.0 * UnitHead
	ot = matrix3 1
	ot.row1 = [0, 0, -1]
	ot.row2 = [0, -1, 0]
	ot.row3 = [-1, 0, 0]
	ot.row4 = [0, 0, -0.5 * h]
	t = matrix3 1
	t.row1 = [0, 0, -1]
	t.row2 = [0, -1, 0]
	t.row3 = [-1, 0, 0]
	t.row4 = [-0.75 * w, 0, 4.25 * UnitHead]
	bipRThigh = rctCreatePart (bip + " R Thigh") l w h t ot type:type
	append bipRLeg bipRThigh

	-- Left Calf
	l = 0.6 * UnitHead
	w = 0.5 * UnitHead
	h = 1.75 * UnitHead
	if( not feet ) then h += 0.5 * UnitHead
	ot = matrix3 1
	ot.row1 = [0, 0, -1]
	ot.row2 = [0, -1, 0]
	ot.row3 = [-1, 0, 0]
	ot.row4 = [0, 0, -0.5 * h]
	t = matrix3 1
	t.row1 = [0, 0, -1]
	t.row2 = [0, -1, 0]
	t.row3 = [-1, 0, 0]
	t.row4 = [0.75 * w, 0, 2.25 * UnitHead]
	bipLCalf = rctCreatePart (bip + " L Calf") l w h t ot type:type
	append bipLLeg bipLCalf
	
	
	-- Right Calf
	l = 0.6 * UnitHead
	w = 0.5 * UnitHead
	h = 1.75 * UnitHead
	if( not feet ) then h += 0.5 * UnitHead
	ot = matrix3 1
	ot.row1 = [0, 0, -1]
	ot.row2 = [0, -1, 0]
	ot.row3 = [-1, 0, 0]
	ot.row4 = [0, 0, -0.5 * h]
	t = matrix3 1
	t.row1 = [0, 0, -1]
	t.row2 = [0, -1, 0]
	t.row3 = [-1, 0, 0]
	t.row4 = [-0.75 * w, 0, 2.25 * UnitHead]
	bipRCalf = rctCreatePart (bip + " R Calf") l w h t ot type:type
	append bipRLeg bipRCalf

	if (linkParts) then
	(
		bipHead.parent = lastVertebra
		bipLThigh.parent = bipPelvis
		bipRThigh.parent = bipPelvis
		bipLCalf.parent = bipLThigh
		bipRCalf.parent = bipRThigh
	)
	
	if( feet ) then
	(
		-- Left Foot
		l = 1.0 * UnitHead
		w = 0.5 * UnitHead
		h = 0.5 * UnitHead
		ot = matrix3 1
		ot.row1 = [0, 0, -1]
		ot.row2 = [0, -1, 0]
		ot.row3 = [-1, 0, 0]
		ot.row4 = [0, -0.5 * w, -0.5 * h]
		t = matrix3 1
		t.row1 = [0, 0, -1]
		t.row2 = [0, -1, 0]
		t.row3 = [-1, 0, 0]
		t.row4 = [0.75 * w, 0.0 * l, h]
		bipLFoot = rctCreatePart (bip + " L Foot") l w h t ot type:type
		append bipLLeg bipLFoot
			
		-- Right Foot
		l = 1.0 * UnitHead
		w = 0.5 * UnitHead
		h = 0.5 * UnitHead
		ot = matrix3 1
		ot.row1 = [0, 0, -1]
		ot.row2 = [0, -1, 0]
		ot.row3 = [-1, 0, 0]
		ot.row4 = [0, -0.5 * w, -0.5 * h]
		t = matrix3 1
		t.row1 = [0, 0, -1]
		t.row2 = [0, -1, 0]
		t.row3 = [-1, 0, 0]
		t.row4 = [-0.75 * w, 0.0 * l, h]
		bipRFoot = rctCreatePart (bip + " R Foot") l w h t ot type:type
		append bipRLeg bipRFoot	

		if (linkParts) then
		(
			bipLFoot.parent = bipLCalf
			bipRFoot.parent = bipRCalf
		)

	)
	
	-- Left Upperarm
	l = 0.5 * UnitHead
	w = 0.3 * UnitHead
	h = 1.25 * UnitHead
	ot = matrix3 1
	ot.row1 = [0, 0, 1]
	ot.row2 = [0, 1, 0]
	ot.row3 = [-1, 0, 0]
	ot.row4 = [0, 0, -0.5 * h]
	t = matrix3 1
	t.row1 = [1, 0, 0]
	t.row2 = [0, 1, 0]
	t.row3 = [0, 0, 1]
	t.row4 = [0.75 * UnitHead, 0, 6.75 * UnitHead]
	bipLUpperArm = rctCreatePart (bip + " L UpperArm") l w h t ot type:type
	append bipLArm bipLUpperArm	

	-- Right Upperarm
	l = 0.5 * UnitHead
	w = 0.3 * UnitHead
	h = 1.25 * UnitHead
	ot = matrix3 1
	ot.row1 = [0, 0, 1]
	ot.row2 = [0, 1, 0]
	ot.row3 = [-1, 0, 0]
	ot.row4 = [0, 0, -0.5 * h]
	t = matrix3 1
	t.row1 = [-1, 0, 0]
	t.row2 = [0, 1, 0]
	t.row3 = [0, 0, -1]
	t.row4 = [-0.75 * UnitHead, 0, 6.75 * UnitHead]
	bipRUpperArm = rctCreatePart (bip + " R UpperArm") l w h t ot type:type
	append bipRArm bipRUpperArm	

	-- Left Forearm
	l = 0.4 * UnitHead
	w = 0.3 * UnitHead
	h = 1.25 * UnitHead
	if( not hands ) then h += UnitHand
	ot = matrix3 1
	ot.row1 = [0, 0, 1]
	ot.row2 = [0, 1, 0]
	ot.row3 = [-1, 0, 0]
	ot.row4 = [0, 0, -0.5 * h]
	t = matrix3 1
	t.row1 = [1, 0, 0]
	t.row2 = [0, 1, 0]
	t.row3 = [0, 0, 1]
	t.row4 = [2.0 * UnitHead, 0, 6.75 * UnitHead]
	bipLForearm = rctCreatePart (bip + " L Forearm") l w h t ot type:type
	append bipLArm bipLForearm	

	-- Right Forearm
	l = 0.4 * UnitHead
	w = 0.3 * UnitHead
	h = 1.25 * UnitHead
	if( not hands ) then h += UnitHand
	ot = matrix3 1 	
	ot.row1 = [0, 0, 1]
	ot.row2 = [0, 1, 0]
	ot.row3 = [-1, 0, 0]
	ot.row4 = [0, 0, -0.5 * h]
	t = matrix3 1
	t.row1 = [-1, 0, 0]
	t.row2 = [0, 1, 0]
	t.row3 = [0, 0, -1]
	t.row4 = [-2.0 * UnitHead, 0, 6.75 * UnitHead]
	bipRForearm = rctCreatePart (bip + " R Forearm") l w h t ot type:type
	append bipRArm bipRForearm	

	if (linkParts) then
	(
		bipLUpperArm.parent = lastVertebra
		bipRUpperArm.parent = lastVertebra
		bipLForeArm.parent = bipLUpperArm
		bipRForeArm.parent = bipRUpperArm
	)
	
	if( hands ) then
	(
		-- Left Hand
		l = 0.4 * UnitHead
		w = 0.2 * UnitHead
		h = UnitHand
		ot = matrix3 1
		ot.row1 = [0, 1, 0]
		ot.row2 = [0, 0, -1]
		ot.row3 = [-1, 0, 0]
		ot.row4 = [0, 0, -0.5 * h]
		t = matrix3 1
		t.row1 = [1, 0, 0]
		t.row2 = [0, 0, -1]
		t.row3 = [0, 1, 0]
		t.row4 = [3.25 * UnitHead, 0, 6.75 * UnitHead]
		bipLHand = rctCreatePart (bip + " L Hand") l w h t ot type:type
		append bipLArm bipLHand

		-- Right Hand
		l = 0.4 * UnitHead
		w = 0.2 * UnitHead
		h = UnitHand
		ot = matrix3 1
		ot.row1 = [0, -1, 0]
		ot.row2 = [0, 0, 1]
		ot.row3 = [-1, 0, 0]
		ot.row4 = [0, 0, -0.5 * h]
		t = matrix3 1
		t.row1 = [-1, 0, 0]
		t.row2 = [0, 0, -1]
		t.row3 = [0, -1, 0]
		t.row4 = [-3.25 * UnitHead, 0, 6.75 * UnitHead]
		bipRHand = rctCreatePart (bip + " R Hand") l w h t ot type:type
		append bipRArm bipRHand
		
		if (linkParts) then
		(
			bipLHand.parent = bipLForeArm
			bipRHand.parent = bipRForeArm
		)
	)

	bipArms = #( bipLArm, bipRArm )
	bipLegs = #( bipLLeg, bipRLeg )

	bipParts = #( bipSpineParts, bipLegs, bipArms )
		
	return bipParts
)

--
-- rctGetPart 
-- given a root name and an extension
-- getpart attempts to select that part
-- and if necessary it creates a snapshot
-- of same

function rctGetPart bip part createSnapshot:false linkToOriginal:false =
(
	
	bipPart = execute( "$" + bip + part )
		
	if( (undefined != bipPart) and createSnapshot ) then
	(
		-- create a snapshot
		new_part = snapshot bipPart
		
		if (linkToOriginal) then
		(
			new_part.parent = bipPart
		)
		else
		(
			new_part.parent = Undefined
		)
		
		-- if the part is a group head, then also snapshot
		-- the children

		if isGroupHead bipPart then
		(		
			RCT_GROUPS_PRESENT = true
			
			setGroupHead new_part true
			
			setGroupOpen new_part true
			
			for c in bipPart.children do
			(			
				-- we do not wish to confuse the children
				-- which should be a member of the group
				-- with the child that is the next link 
				-- in the hierarchy

				if isGroupMember c then
				(
					new_c = snapshot c
					new_c.parent = new_part
					setGroupMember new_c true
				)
			)
		)	

		return new_part
	)
	
	return bipPart
)

function rctGetParts	bip postfix:"" createSnapshots:false \
						numVertebra:-1 hands:false feet:false linkToOriginal:false=
(
	--	parts == #(	spineParts( Pelvis, N Vertebra, Head ), 
	--				legs( 	leftLeg( thigh, calf, foot ), 
	--						rightLeg( thigh, calf, foot ) )
	--				arms( 	leftArm( upperArm, forearm, hand ),
	--						rightArm( upperArm, forearm, hand ) ) )

	-- bip should be the common name of all parts
	-- function assumes naming convention of a character studio biped

	bipRUpperarm = rctGetPart bip ("_R_UpperArm" + postfix) createSnapshot:createSnapshots linkToOriginal:linkToOriginal
	bipRForearm = rctGetPart bip ("_R_ForeArm" + postfix) createSnapshot:createSnapshots linkToOriginal:linkToOriginal
	bipRArm = #( bipRUpperarm, bipRForearm )
	if( hands ) then
	(
		bipRHand = rctGetPart bip ("_R_Hand" + postfix) createSnapshot:createSnapshots linkToOriginal:linkToOriginal
		append bipRArm bipRHand
	)

	bipLUpperarm = rctGetPart bip ("_L_UpperArm" + postfix) createSnapshot:createSnapshots linkToOriginal:linkToOriginal
	bipLForearm = rctGetPart bip ("_L_ForeArm" + postfix) createSnapshot:createSnapshots linkToOriginal:linkToOriginal
	bipLArm = #( bipLUpperarm, bipLForearm )
	if( hands ) then
	(
		bipLHand = rctGetPart bip ("_L_Hand" + postfix) createSnapshot:createSnapshots linkToOriginal:linkToOriginal
		append bipLArm bipLHand
	)

	bipRThigh = rctGetPart bip ("_R_Thigh" + postfix) createSnapshot:createSnapshots linkToOriginal:linkToOriginal
	bipRCalf = rctGetPart bip ("_R_Calf" + postfix) createSnapshot:createSnapshots linkToOriginal:linkToOriginal
	bipRLeg = #( bipRThigh, bipRCalf )
	if( feet ) then
	(
		bipRFoot = rctGetPart bip ("_R_Foot" + postfix) createSnapshot:createSnapshots linkToOriginal:linkToOriginal
		append bipRLeg bipRFoot
	)

	bipLThigh = rctGetPart bip ("_L_Thigh" + postfix) createSnapshot:createSnapshots linkToOriginal:linkToOriginal
	bipLCalf = rctGetPart bip ("_L_Calf" + postfix) createSnapshot:createSnapshots linkToOriginal:linkToOriginal
	bipLLeg = #( bipLThigh, bipLCalf )
	if( feet ) then
	(
		bipLFoot = rctGetPart bip ("_L_Foot" + postfix) createSnapshot:createSnapshots linkToOriginal:linkToOriginal
		append bipLLeg bipLFoot
	)
	
	bipPelvis = rctGetPart bip ("_Pelvis" + postfix) createSnapshot:createSnapshots  linkToOriginal:linkToOriginal
		
	bipSpineParts = #( bipPelvis )
	bipSpine = rctGetPart bip ("_Spine" + postfix) createSnapshot:createSnapshots linkToOriginal:linkToOriginal

	index = 1
	do
	(
		append bipSpineParts bipSpine
		bipSpine = undefined
		if( (index < numVertebra) or (numVertebra < 0) ) then
		(
			bipSpine = rctGetPart bip ("_Spine" + (index as string) + postfix) createSnapshot:createSnapshots linkToOriginal:linkToOriginal
			index = index + 1
		)
	)
	while( undefined != bipSpine ) 
	
	bipHead = rctGetPart bip ("_Head" + postfix) createSnapshot:createSnapshots linkToOriginal:linkToOriginal
	append bipSpineParts bipHead
	
	bipArms = #( bipLArm, bipRArm )
	bipLegs = #( bipLLeg, bipRLeg )

	bipParts = #( bipSpineParts, bipLegs, bipArms )
		
	return bipParts
)


struct RagdollJointLimit ( twistMin, twistMax, coneMin, coneMax, planeMin, planeMax )
struct RagdollJointTransform ( twistAxis, twistPerpendicularAxis, planeAxis )
struct RagdollJoint ( attachedBody, referenceBody, limit, attachedTransform, referenceTransform, jointname ) 
function rctMakeRagdollJoint joint = 
(
	if (undefined == joint.attachedBody or undefined == joint.referenceBody) then
	(
		RCT_NUM_RAGDOLL_ERRORS += 1
	)	
	
	if (undefined == joint.attachedBody) then
	(
		format "no attached body found for this ragdoll joint: % \n" joint.jointname
		return 0
	)

	if (undefined == joint.referenceBody) then
	(
		format "no reference body found for this ragdoll joint: % \n" joint.jointname
		return 0
	)
	

	
	-- create constraint
	rd = rctRagdoll()
	
	-- set basic properties
	rd.childBody = joint.attachedBody
	rd.hasParent = undefined != joint.referenceBody
	rd.parentBody = joint.referenceBody
	rd.twistMin = joint.limit.twistMin
	rd.twistMax = joint.limit.twistMax
	rd.coneMin = joint.limit.coneMin
	rd.coneMax = joint.limit.coneMax
	rd.planeMin = joint.limit.planeMin
	rd.planeMax = joint.limit.planeMax
	
	-- set attached transform
	(
		t = matrix3 1
		t.row1 = joint.attachedTransform.twistAxis
		t.row2 = joint.attachedTransform.twistPerpendicularAxis
		t.row3 = cross t.row1 t.row2
		t.row4 = [0,0,0]
	
		rd.childTransform = t 
	)
	
	-- set reference transform
	(
		t = matrix3 1
		if( rd.hasParent ) then
		(
			t.row1 = joint.referenceTransform.twistAxis
			t.row2 = joint.referenceTransform.twistPerpendicularAxis
			t.row3 = cross t.row1 t.row2
			t.row4 = rd.childBody.transform.position * (inverse rd.parentBody.transform)
		)
	
		rd.parentTransform = t 
	)
	
	-- set plane offset transform
	(
		
		t.row1 = joint.referenceTransform.planeAxis
		t.row2 = [t.row1.y, t.row1.z, t.row1.x]
		t.row3 = cross t.row1 t.row2
		t.row4 = [0,0,0]
		
		-- transform need to be in reference space
		m = rd.parentTransform
		m.row4 = [0,0,0]
		t = t * (inverse m) 
		
		rd.planeOffsetTransform = t
	)
	
	rd.iconSize = 1

	-- disable collisions between both objects
	reactor.disableCollisions joint.attachedBody joint.referenceBody	

	return rd
)

struct HingeJointLimit ( isLimited, minAngle, maxAngle )
struct HingeJointTransform ( axis, perpendicularAxis )
struct HingeJoint ( attachedBody, referenceBody, limit, attachedTransform, referenceTransform, jointname ) 
function rctMakeHingeJoint joint = 
(
	if (undefined == joint.attachedBody or undefined == joint.referenceBody) then
	(
		RCT_NUM_HINGE_ERRORS += 1
	)
	
	if (undefined == joint.attachedBody) then
	(
		format "no attached body found for this hinge joint: % \n" joint.jointname
		return 0
	)
	
	if (undefined == joint.referenceBody) then
	(
		format "no reference body found for this hinge joint: % \n" joint.jointname
		return 0
	)
	
	-- create constraint
	hn = rctHinge()
	
	-- set basic properties
	hn.childBody = joint.attachedBody
	hn.hasParent = undefined != joint.referenceBody
	hn.parentBody = joint.referenceBody

	hn.isLimited = joint.limit.isLimited
	hn.limitMinAngle = joint.limit.minAngle
	hn.limitMaxAngle = joint.limit.maxAngle
	
	-- set attached transform
	(
		t = matrix3 1
		t.row1 = joint.attachedTransform.axis
		t.row2 = joint.attachedTransform.perpendicularAxis
		t.row3 = cross t.row1 t.row2
		t.row4 = [0,0,0]
	
		hn.childTransform = t 
	)
	
	-- set reference transform
	(
		t = matrix3 1
		if( hn.hasParent ) then
		(
			t.row1 = joint.referenceTransform.axis
			t.row2 = joint.referenceTransform.perpendicularAxis
			t.row3 = cross t.row1 t.row2
			t.row4 = hn.childBody.transform.position * (inverse hn.parentBody.transform)
		)
	
		hn.parentTransform = t 
	)
		
	hn.iconSize = 1

	-- disable collisions between both objects
	reactor.disableCollisions joint.attachedBody joint.referenceBody	
	
	return hn
)

function rctMakeRagdollFromParts parts = 
(
	ragdollJoints = #()
	hingeJoints = #()
	
	--	parts == #(	spineParts( Pelvis, N Vertebra, Head ), 
	--				legs( 	leftLeg( thigh, calf, foot ), 
	--						rightLeg( thigh, calf, foot ) )
	--				arms( 	leftArm( upperArm, forearm, hand ),
	--						rightArm( upperArm, forearm, hand ) ) )

	-- RagdollJointTransform = ( twistAxis, twistPerpendicularAxis, planeAxis )
	-- struct HingeJointTransform ( Axis, PerpendicularAxis )

	-- Reactor 2.1, issue RCT-390 - Specialized handling of CS Biped
	local usingCOM = false
	
	bipPelvis = parts[1][1]
	if (bipPelvis != undefined and bipPelvis.controller.classId[1] == 37204 and bipPelvis.controller.classId[2]==0 ) then
	(
		-- This is a CS Biped, use the COM as the root and not the Pelvis
		parts[1][1] = bipPelvis.parent
		usingCOM = true		
	)

	neck = RagdollJoint parts[1][parts[1].count]
	neck.jointname = "neck"  
	neck.referenceBody = parts[1][parts[1].count-1]	
	neck.limit = RagdollJointLimit twistMin:-5 twistMax:5 coneMin:-45 coneMax:10 planeMin:-15 planeMax:15
	neck.attachedTransform = RagdollJointTransform [1,0,0] [0,1,0] 
	neck.referenceTransform = RagdollJointTransform [1,0,0] [0,1,0] [0,0,1]
	append ragdollJoints neck

	numVertebra = parts[1].count - 2	
	for i = 2 to parts[1].count - 1 do
	(
		vertebra = RagdollJoint parts[1][i]
		vertebra.referenceBody = parts[1][i - 1]
		vertebra.limit = RagdollJointLimit		twistMin:(-5 / numVertebra) \
												twistMax:(5 / numVertebra) \
												coneMin:(-25 / numVertebra) \
												coneMax:(25 / numVertebra) \
												planeMin:(-20 / numVertebra) \
												planeMax:(10 / numVertebra)
		vertebra.attachedTransform = RagdollJointTransform [1,0,0] [0,1,0]
		vertebra.referenceTransform = RagdollJointTransform [1,0,0] [0,1,0] [0,1,0]
		
		-- RCT-390 : CS COM has a different local transform than the Pelvis
		if (usingCOM and i==2) then
		(
			--  twistAxis, twistPerpendicularAxis, planeAxis )
			vertebra.referenceTransform = RagdollJointTransform [0,0,1] [1,0,0] [1,0,0]
		)
			
		append ragdollJoints vertebra
	)

	--
	-- Left refers to the bipeds left
	--
	
	leftShoulder = RagdollJoint parts[3][1][1] 
	leftShoulder.jointname = "leftShoulder" 
	leftShoulder.referenceBody = parts[1][parts[1].count-1]
	leftShoulder.limit = RagdollJointLimit twistMin:-5 twistMax:5 coneMin:-85 coneMax:75 planeMin:-65 planeMax:5
	leftShoulder.attachedTransform = RagdollJointTransform [1,0,0] [0,0,1]
	leftShoulder.referenceTransform = RagdollJointTransform [0,0,1] [1,0,0] [0,1,0]
	append ragdollJoints leftShoulder
	
	leftElbow = HingeJoint parts[3][1][2] 
	leftElbow.jointname = "leftElbow" 
	leftElbow.referenceBody = parts[3][1][1]
	leftElbow.limit = HingeJointLimit isLimited:true minAngle:10.0 maxAngle:80.0
	leftElbow.attachedTransform = HingeJointTransform [1,0,0] [0,-1,0]
	leftElbow.referenceTransform = HingeJointTransform [1,0,0] [0,-1,0] 
	append hingeJoints leftElbow
	
	if( undefined != parts[3][1][3] ) then
	(
		leftWrist = HingeJoint parts[3][1][3] 
		leftWrist.jointname = "leftWrist" 
		leftWrist.referenceBody = parts[3][1][2]
		leftWrist.limit = HingeJointLimit isLimited:true minAngle:-10.0 maxAngle:20.0
		leftWrist.attachedTransform = HingeJointTransform [1,0,0] [0,1,0]
		leftWrist.referenceTransform = HingeJointTransform [1,0,0] [0,0,-1] 
		append hingeJoints leftWrist
	)

	leftHip = RagdollJoint parts[2][1][1]
	leftHip.jointname = "leftHip" 
	leftHip.referenceBody = parts[1][1]
	leftHip.limit = RagdollJointLimit twistMin:-5 twistMax:5 coneMin:-25 coneMax:85 planeMin:-45 planeMax:2.5
	leftHip.attachedTransform = RagdollJointTransform [1,0,0] [0,-1,0]
	leftHip.referenceTransform = RagdollJointTransform [-1,0,0] [0,-1,0] [0,0,1]	
	-- RCT-390 : CS COM has a different local transform than the Pelvis
	if (usingCOM) then
	(
		--  twistAxis, twistPerpendicularAxis, planeAxis )
		leftHip.referenceTransform = RagdollJointTransform [0,0,-1] [-1,0,0] [0,1,0]
	)
	append ragdollJoints leftHip

	leftKnee = HingeJoint parts[2][1][2]
	leftKnee.jointname = "leftKnee" 
	leftKnee.referenceBody = parts[2][1][1]
	leftKnee.limit = HingeJointLimit isLimited:true minAngle:10.0 maxAngle:80.0
	leftKnee.attachedTransform = HingeJointTransform [1,0,0] [0,-1,0] 
	leftKnee.referenceTransform = HingeJointTransform [1,0,0] [0,-1,0] 	
	append hingeJoints leftKnee
	
	if( undefined != parts[2][1][3] ) then
	(
		leftAnkle = HingeJoint parts[2][1][3] 
		leftAnkle.jointname = "leftAnkle" 
		leftAnkle.referenceBody = parts[2][1][2]
		leftAnkle.limit = HingeJointLimit isLimited:true minAngle:-15.0 maxAngle:35.0
		leftAnkle.attachedTransform = HingeJointTransform [1,0,0] [0,-1,0]
		leftAnkle.referenceTransform = HingeJointTransform [1,0,0] [0,-1,0] 
		append hingeJoints leftAnkle
	)

	rightShoulder = RagdollJoint parts[3][2][1] 
	rightShoulder.jointname = "rightShoulder" 
	rightShoulder.referenceBody = parts[1][parts[1].count-1]
	rightShoulder.limit = RagdollJointLimit twistMin:-5 twistMax:5 coneMin:-75 coneMax:85 planeMin:-65 planeMax:5
	rightShoulder.attachedTransform = RagdollJointTransform [1,0,0] [0,0,-1]
	rightShoulder.referenceTransform = RagdollJointTransform [0,0,-1] [1,0,0] [0,1,0]
	append ragdollJoints rightShoulder

	rightElbow = HingeJoint parts[3][2][2] 
	rightElbow.jointname = "rightElbow" 
	rightElbow.referenceBody = parts[3][2][1]
	rightElbow.limit = HingeJointLimit isLimited:true minAngle:10.0 maxAngle:80.0
	rightElbow.attachedTransform = HingeJointTransform [1,0,0] [0,-1,0]
	rightElbow.referenceTransform = HingeJointTransform [1,0,0] [0,-1,0]
	append hingeJoints rightElbow

	if( undefined != parts[3][2][3] ) then
	(
		rightWrist = HingeJoint parts[3][2][3] 
		rightWrist.jointname = "rightWrist" 
		rightWrist.referenceBody = parts[3][2][2]
		rightWrist.limit = HingeJointLimit isLimited:true minAngle:-10.0 maxAngle:20.0
		rightWrist.attachedTransform = HingeJointTransform [1,0,0] [0,1,0]
		rightWrist.referenceTransform = HingeJointTransform [1,0,0] [0,0,1] 
		append hingeJoints rightWrist
	)

	rightHip = RagdollJoint parts[2][2][1]
	rightHip.jointname = "rightHip" 
	rightHip.referenceBody = parts[1][1]
	rightHip.limit = RagdollJointLimit twistMin:-5 twistMax:5 coneMin:-85 coneMax:25 planeMin:-45 planeMax:2.5
	rightHip.attachedTransform = RagdollJointTransform [1,0,0] [0,1,0]
	rightHip.referenceTransform = RagdollJointTransform [-1,0,0] [0,1,0] [0,0,-1]	
	-- RCT-390 : CS COM has a different local transform than the Pelvis
	if (usingCOM) then
	(
		--  twistAxis, twistPerpendicularAxis, planeAxis )
		rightHip.referenceTransform = RagdollJointTransform [0,0,-1] [1,0,0] [0,-1,0]
	)
	append ragdollJoints rightHip

	rightKnee = HingeJoint parts[2][2][2]
	rightKnee.jointname = "rightKnee" 
	rightKnee.referenceBody = parts[2][2][1]
	rightKnee.limit = HingeJointLimit isLimited:true minAngle:10.0 maxAngle:80.0
	rightKnee.attachedTransform = HingeJointTransform [1,0,0] [0,-1,0] 
	rightKnee.referenceTransform = HingeJointTransform [1,0,0] [0,-1,0] 
	append hingeJoints rightKnee

	if( undefined != parts[2][2][3] ) then
	(
		rightAnkle = HingeJoint parts[2][2][3] 
		rightAnkle.jointname = "rightAnkle" 
		rightAnkle.referenceBody = parts[2][2][2]
		rightAnkle.limit = HingeJointLimit isLimited:true minAngle:-15.0 maxAngle:35.0
		rightAnkle.attachedTransform = HingeJointTransform [1,0,0] [0,-1,0]
		rightAnkle.referenceTransform = HingeJointTransform [1,0,0] [0,-1,0] 
		append hingeJoints rightAnkle
	)

	joints = #()
	
	for joint in ragdollJoints do
	(	
		next_rd = rctMakeRagdollJoint joint

		if (next_rd != 0) then
		(
			append joints (next_rd)
		)
	)
	
	for joint in hingeJoints do
	(
		next_hn = rctMakeHingeJoint joint
		
		if (next_hn != 0) then
		(
			append joints (next_hn)
		)
	)
		
	return joints
)

function rctChangeNameRoot obj oldsrcNameRoot newNameRoot removePostfix:true = 
(
	if (undefined != obj) then
	(
		groupname = obj.name
		
		-- replace spaces with underscores
		for i = 1 to groupname.count do 
		(
			if " " == groupname[i] then groupname[i] = "_"
		)
		
		-- format name correctly
		groupname = execute( "$" + groupname )
			
		-- if this obj is a group we don't rename 
		-- its internal parts using the new prefix,
		-- as this makes it harder to select just the groups 
		-- in the 'select by name' floater.
		
		index = (filterString obj.name " _")[1].count

		newName = substring obj.name (index + 1) obj.name.count
		newName = newNameRoot + newName
		
		-- assumes a postfix exists!	
		if( removePostfix ) then
		(
			newName = substring newName 1 (newName.count-2)
		)

		obj.name = newName
	)
)

function rctRenameSpinePieces srcNameRoot postfix:"" =
(
	index = 1
	vertebra = undefined

	-- search for spinal parts _Spine0 to _Spine9:
	while( (undefined == vertebra) and (index < 10) ) do
	(
		vertebra = rctGetPart srcNameRoot ("_Spine0" + (index as string))
		index = index + 1
	)
	
	if( index < 10 ) then
	(
		index = index - 1
		first = true

		newIndex = 1
		while( undefined != vertebra ) do
		(
			if( first ) then
			(
				vertebra.name = srcNameRoot + " Spine" + postfix
				first = false
			)
			else
			(
				vertebra.name = srcNameRoot + " Spine" + (newIndex as string) + postfix
					newIndex = newIndex + 1
			)
			index = index + 1
				
			vertebra = rctGetPart srcNameRoot ("_Spine0" + (index as string))			
		)
		
		-- biped has at maximum 5 spine pieces
		vertebra = rctGetPart srcNameRoot "_Spine10"
		if( undefined != vertebra ) then
		(
			vertebra.name = srcNameRoot + " Spine5" + postfix
		)
	)
)

function rctMakeIndependentHierarchyFromParts parts postfix:"01" =
(
	return
	
	-- assume naming character studio naming convention 
	
	--	parts == #(	spineParts( Pelvis, N Spines, Head ), 
	--				legs( 	leftLeg( thigh, calf, foot ), 
	--						rightLeg( thigh, calf, foot ) )
	--				arms( 	leftArm( upperArm, forearm, hand ),
	--						rightArm( upperArm, forearm, hand ) ) )

	parts[1][1].parent = undefined
	for i = 2 to parts[1].count do
	(
		parts[1][i].parent = parts[1][i-1]
	)
	
	for j = 1 to 2 do
	(
		parts[2][j][1].parent = parts[1][2]
		for i = 2 to parts[2][j].count do
		(
			parts[2][j][i].parent = parts[2][j][i-1]
		)
	)
	
	for j = 1 to 2 do
	(
		parts[3][j][1].parent = parts[1][parts[1].count-1]
		for i = 2 to parts[3][j].count do
		(
			parts[3][j][i].parent = parts[3][j][i-1]
		)
	)
)

function rctMakeIndependentHierarchyFromRoot root postfix:"01" numVertebra:1 hands:false feet:false =
(
	
	return
	
	-- get parts again but this time don't create any snapshots
	parts = rctGetParts root postfix:postfix createSnapshots:false  \
								numVertebra:numVertebra hands:hands feet:feet
	
	-- assume naming character studio naming convention 
	
	--	parts == #(	spineParts( Pelvis, N Spines, Head ), 
	--				legs( 	leftLeg( thigh, calf, foot ), 
	--						rightLeg( thigh, calf, foot ) )
	--				arms( 	leftArm( upperArm, forearm, hand ),
	--						rightArm( upperArm, forearm, hand ) ) )

	-- defines
	local thigh = 1
	local calf = 2
	local foot = 3
	
	local upperArm = 1
	local foreArm = 2
	local hand = 3
	
	if (parts[1][1] != undefined) then
	(
		parts[1][1].parent = undefined
	)
	
	for spinePart = 2 to parts[1].count do
	(
		if ((undefined != parts[1][spinePart]) and (undefined != parts[1][spinePart-1])) then
		(
			-- parent each spine part to the previous spine part
			parts[1][spinePart].parent = parts[1][spinePart-1]
		)
	)
	
	-- for each leg
	for legPart = 1 to 2 do
	(
		if (undefined != parts[2][legPart][1]) and (undefined != parts[1][1]) then
		(
			-- parent leg to pelvis
			parts[2][legPart][1].parent = parts[1][1]
		)	
					
		if (undefined != parts[2][legPart][calf]) and (undefined != parts[2][legPart][thigh]) then
		(
			-- parent calf to thigh
			parts[2][legPart][calf].parent = parts[2][legPart][thigh]
		)
		
		if (undefined != parts[2][legPart][calf]) and (undefined != parts[2][legPart][foot]) then
		(
			-- parent foot to calf
			parts[2][legPart][foot].parent = parts[2][legPart][calf]
		)
	)
	
	-- for each arm
	for armPart = 1 to 2 do
	(
		if (undefined != parts[3][armPart][1]) and (undefined != parts[1][parts[1].count-1]) then
		(
			parts[3][armPart][1].parent = parts[1][parts[1].count-1]
		)
		
		if (undefined != parts[3][armPart][upperArm]) and (undefined != parts[3][armPart][foreArm]) then
		(		
			parts[3][armPart][foreArm].parent = parts[3][armPart][upperArm]
		)

		if (undefined != parts[3][armPart][hand]) and (undefined != parts[3][armPart][foreArm]) then
		(		
			parts[3][armPart][hand].parent = parts[3][armPart][foreArm]
		)
	)
)

function rctMakeRagdoll bipName createRBCollection createCSolver snapshotParts linkToOriginal\
						changeNameRoot ragdollNameRoot \
						numVertebra:-1 hands:false feet:false = 
(
	-- reset globals
	RCT_GROUPS_PRESENT = false
	
	srcNameRoot = bipName
		
	rigidbodies = rctGetParts	srcNameRoot createSnapshots:snapshotParts  \
								numVertebra:numVertebra hands:hands feet:feet linkToOriginal:linkToOriginal

		
	constraints = rctMakeRagdollFromParts rigidbodies 

	if( changeNameRoot ) then
	(
		newNameRoot = ragdollNameRoot
	)
	else
	(
		newNameRoot = srcNameRoot
	)

	rbs = #()
	join rbs rigidbodies[1]		-- head/spine/pelvis
	join rbs rigidbodies[2][1]  
	join rbs rigidbodies[2][2]
	join rbs rigidbodies[3][1]
	join rbs rigidbodies[3][2]

	filtered_rbs = #()
		
	for rb in rbs do
	(
		if (undefined != rb) then
		(		
			append filtered_rbs rb
		)
	)
		
	for rb in filtered_rbs do
	(
		if isGroupMember rb and not isGroupHead rb then
		(
			---------------------------
			-- set primitive properties:
			---------------------------

			setUserProp rb "Mass" 10

			-- disable collisions between the limbs
			setUserProp rb "Collision_Groups" RCT_CURRENT_COLLISIONGROUP_VALUE

			-- use bounding boxes
			setUserProp rb "Simulation_Geometry" 0

			-- hide the display children 
			setUserProp rb "Display_Children" 0

		)
		else if isGroupHead rb then
		(
			---------------------------
			-- set RB properties
			---------------------------

			setUserProp rb "Friction" 0.3

			setUserProp rb "Ellasticity" 0.3

			-- SHOW the display children 
			setUserProp rb "Display_Children" 1

			-- iterate through the children:
			for c in rb.children do 
			(
				
				---------------------------
				-- set primitive properties:
				---------------------------
				
				local prim_mass = 10.0f 
				prim_mass = prim_mass / rb.children.count
				
				-- distribute mass
				setUserProp c "Mass" prim_mass

				-- disable collisions between the limbs
				setUserProp c "Collision_Groups" RCT_CURRENT_COLLISIONGROUP_VALUE

				-- use bounding boxes
				setUserProp c "Simulation_Geometry" 0

				-- HIDE the display children 
				setUserProp c "Display_Children" 0

				setUserProp c "Proxy_Geometry" 		"<NONE>"
				setUserProp c "Use_Display_Proxy" 	0
				setUserProp c "Disable_Collisions" 	0
				setUserProp c "Inactive" 			0
				setUserProp c "Display_Proxy" 		"<NONE>"

			)

		)
		else if not isGroupMember rb and not isGroupHead rb then
		(
			----------------------------------
			-- set RB and primitive properties 
			----------------------------------

			setUserProp rb "Mass" 10

			-- use bounding boxes
			setUserProp rb "Simulation_Geometry" 0

			-- hide the display children 
			setUserProp rb "Display_Children" 0

			setUserProp rb "Friction" 0.3
			setUserProp rb "Ellasticity" 0.3	

			setUserProp rb "Proxy_Geometry" 		"<NONE>"
			setUserProp rb "Use_Display_Proxy" 	0
			setUserProp rb "Disable_Collisions" 	0
			setUserProp rb "Inactive" 			0
			setUserProp rb "Display_Proxy" 		"<NONE>"

		)

	)

	
	rctRenameSpinePieces srcNameRoot postfix:"01"	

	rctMakeIndependentHierarchyFromRoot srcNameRoot postfix:"01" hands:hands feet:feet
	
	if( changeNameRoot ) then
	(
		for rb in filtered_rbs do
		(
			rctchangeNameRoot rb srcNameRoot ragdollNameRoot
		)
	)
	
	-- select the ragdoll:
	select filtered_rbs
	
	local rbc
	local cs
	
	if( createRBCollection ) then
	(
		--- this gets a "nice" position for the new collection and solver relative to the current viewport
	--	local constructionPlaneToWorld = getCPTM()
	--	local viewSize = getViewSize()
	--	local collectionX = viewSize.x * 0.9
	--	local solverX = viewSize.x * 0.8
	--	local objectY = viewSize.y * 0.1
	--	local collectionPos = mapScreenToCP(Point2 collectionX objectY) * constructionPlaneToWorld
	--	local solverPos = mapScreenToCP(Point2 solverX objectY) * constructionPlaneToWorld
		
		rbc = rctRBCollection()
		rbc.rigidBodies = filtered_rbs
		rbcPosition = Point3 0 0 0 --this and minX are built up to be the top left and right of the bounding box
		minX = 0 --of the bodies respectively and are then used for the collection and solver positions relatively
		for o in filtered_rbs do
		(
			if o.max.x > rbcPosition.x then rbcPosition.x = o.max.x
			if o.min.x < minX then minX = o.min.x			
			if o.max.y > rbcPosition.y then rbcPosition.y = o.max.y
			if o.max.z > rbcPosition.z then rbcPosition.z = o.max.z	
		)
		
		
		rbc.name = newNameRoot + "RBCollection"
		rbc.position = rbcPosition

		-- add a drag action to the RBC as we're using constraints:
		if (RCT_ADD_DRAG_ACTION == true) then
		(
			reactor.useDragAction = true
		)
		
		selectMore rbc
		
		if( createCSolver ) then
		(	
			cs = rctConstraintSolver()
			cs.constraints = constraints
			cs.rbCollection = rbc

			cs.name = newNameRoot + "CSolver"

			for c in constraints do
			(
				c.name = newNameRoot + "Constraint " + c.name
			)
			rbcPosition.x = minX
			cs.position = rbcPosition
			selectmore cs
		)
	)
	
	----------------------------------
	---- Create / Append to  
	---- Selection Sets
	----------------------------------
	
	local rct_ragdoll = #()
	
	if( createRBCollection ) then
	(
		append rct_ragdoll rbc

		if( createCSolver ) then
		(
			append rct_ragdoll cs
		)
	)
	
	--check for biped root object
	bipedRoot = execute( "$" + srcNameRoot )
	if ( (bipedRoot != undefined) and (isKindOf bipedRoot Biped_Object) ) then --add biped object to body set if we haven't already
	(
		append rct_ragdoll bipedRoot 
	)

	-- add a selection set containing all the constraints in this new character:
	selectionSets["rct_"+newNameRoot+"_constraints"] = constraints
	
	-- add a selection set containing all the constraints and body parts in this new character:
	selectionSets["rct_"+newNameRoot+"_ragdoll"] = rct_ragdoll + filtered_rbs 
	
	-- add a selection set containing all the constraints in this new character:
	local rct_all = #()
	
	-- if the selection set was previously created
	-- copy the elements from it and then delete it.
	-- this is a roundabout way of doing an append
	if (selectionSets["rct_ragdolls"] != undefined) then
	(
		for s in selectionSets["rct_ragdolls"] do
		(
			append rct_all s
		)
		
		deleteItem selectionSets "rct_ragdolls"	
	)

	join rct_ragdoll constraints
	
	join rct_all rct_ragdoll
	
	selectionSets["rct_ragdolls"] = rct_all
	
	return true
)

--
-- global fns
--

function isCharacter p = 
(
	pos = findString p RCT_CHARACTER_ROOT
	if ( (undefined != pos) and ((pos + (RCT_CHARACTER_ROOT.count-1))==p.count) ) then
	(
		
		return true
	)

	return false
)

function addBipedOrCharacter bips g = 
(
	-- add the node if it is part of a biped system

	-- and it has no parents that are also part of that system
	if isKindOf g Biped_Object then
	(
		if (undefined == g.parent) or (not isKindOf g.parent Biped_Object) then
		(
			found = false
			for b in bips do
			(
				if (b == g.name) then found = true			

			)

			if (found == false) then
			(
				append bips g.name
			)
		)
	)
	-- or if it contains the substring RCT_CHARACTER_ROOT
	else if (isCharacter g.name) then
	(
		len = findString g.name RCT_CHARACTER_ROOT
		prefix = substring g.name 1 (len-2)

		found = false
		for b in bips do
		(
			if (b == prefix) then found = true			

		)

		if (found == false) then
		(
			append bips prefix
		)
	)
	
	return bips
)

function getListOfBipeds =
(	
	-- clear bips array
	bips = #()

	-- scan geometries, this is an objectSet maintained by max of all the geometries in the scene
	for g in geometry do
	(
		bips = addBipedOrCharacter bips g
	)

	for h in helpers do
	(
		if ( isGroupHead h ) then
		(
			bips = addBipedOrCharacter bips h
		)
	)

	return bips
)
	
rollout rctCreateRagdollBipedRollout "Create Humanoid"
(
	local ws = reactor.worldScale
	
	editText bipName "Name Root" enabled:true text:"Ragdoll"
	spinner bipHeight "Height" enabled:true range:[1, 10 * ws, 2 * ws] type:#worldunits scale:(0.1 * ws) fieldWidth:50
	spinner numVertebra "Vertebra" enabled:true range:[1, 5, 3] type:#integer scale:1 fieldWidth:50
	checkbox addHands "Add Hands" enabled:true checked:true
	checkbox addFeet "Add Feet" enabled:true checked:true
	checkbox linkParts "Link Parts" enabled:true checked:true
	button goButton "Create Humanoid" enabled:true	
	on goButton pressed do
	(
		undo on
		(
					
			bip_in_scene = getListOfBipeds()
			
			for b in bip_in_scene do 
			(
				if (bipName.text == b) then
				(
					-- new name is already used
					messageBox "The new root name is already in use as a root name" title:"ragdoll script" 

					-- returing false undo's anything that the script has done.
					return false
				)

			)
			
			parts = rctCreateParts	bipName.text bipHeight.value numVertebra:numVertebra.value \
									type:#box hands:addHands.checked feet:addFeet.checked linkParts:linkParts.checked

			-- print parts
			
			rctMakeIndependentHierarchyFromParts parts postfix:""
			
			name = bipName.text + " Pelvis"
			for i = 1 to name.count do if( " " == name[i] ) then name[i] = "_"
			execute ("select $" + name)
		)
	)	
)

rollout rctCreateRagdollFromBipedRollout "Constrain Humanoid" 
(
	local bips = #()
	
	listBox bipList "Humanoids"
	spinner numVertebra "Vertebra" enabled:true range:[1, 5, 3] type:#integer scale:1 fieldWidth:50
	checkbox createRBCollection "Create RBCollection" enabled:true checked:true
	checkbox createCSolver "Create CSolver" enabled:true checked:true
	checkbox addHands "Add Hands" enabled:true checked:true
	checkbox addFeet "Add Feet" enabled:true checked:true
	checkbox snapshotParts "Snapshot Parts" enabled:true checked:false
	checkbox linkToOriginal "Link to Original" enabled:false checked:false 
	checkbox changeNameRoot "Change Name" enabled:false checked:false
	editText ragdollNameRoot "New Name" enabled:false text:"Ragdoll"
	button goButton "Constrain Humanoid" enabled:false
	
	function updateRollout =
	(
		if (RCT_RAGDOLL_BIPED_SCRIPT_ACTIVE == 1) then
		(
			local currentSelection = bipList.selected
			bipList.items = getListOfBipeds()
			
			--set back original selection
			if (currentSelection != undefined) then
			(
				for i = 1 to bipList.items.count do
				(
					if (bipList.items[i] == currentSelection) then
					(
						bipList.selection = i
						goButton.enabled = true
						exit
					)
				)
			)
			else if (bipList.items.count > 0) then
			(
				bipList.selection = 1
				goButton.enabled = true
			)
		)
	)
	
	on rctCreateRagdollFromBipedRollout open do
	(
		RCT_RAGDOLL_BIPED_SCRIPT_ACTIVE = 1
		updateRollout()
	)
	
	on rctCreateRagdollFromBipedRollout close do
	( 
		RCT_RAGDOLL_BIPED_SCRIPT_ACTIVE = 0
	)

	on bipList selected sel do
	(
		updateRollout()
	)
	
	on snapshotParts changed state do
	(
		linkToOriginal.enabled = snapshotParts.checked
		changeNameRoot.enabled = snapshotParts.checked
		ragdollNameRoot.enabled = snapshotParts.checked
	)
	
	on changeNameRoot changed state do
	(
		ragdollNameRoot.enabled = changeNameRoot.checked
	)

	on createRBCollection changed state do
	(
		createCSolver.enabled = createRBCollection.checked
	)
	
	on createCSolver changed state do
	(
	)
	
	on goButton pressed do
	(
		undo on
		(
			if( bipList.enabled ) then
			(
				srcNameRoot = bipList.items[bipList.selection]
			)
			else
			(
				return false
			)
			
			if (changeNameRoot.enabled and changeNameRoot.checked) then
			(
				if (ragdollNameRoot.text == srcNameRoot) then
				(
					-- new name and source name are the same
					messageBox "The new root name is the same as the source root name" title:"ragdoll script" 
					
					-- returing false undo's anything that the script has done.
					return false
				)
				
				for b in bips do 
				(
					if (ragdollNameRoot.text == b) then
					(
						-- new name is already used
						messageBox "The new root name is already in use as a root name" title:"ragdoll script" 
										
						-- returing false undo's anything that the script has done
						return false
					)
					
				)
			)
			
			res = rctMakeRagdoll srcNameRoot \
							(createRBCollection.enabled and createRBCollection.checked) \
							(createCSolver.enabled and createCSolver.checked) \
							snapshotParts.checked \
							(linkToOriginal.enabled and linkToOriginal.checked) \
							(changeNameRoot.enabled and changeNameRoot.checked) \
							ragdollNameRoot.text \
							numVertebra:numVertebra.value \
							hands:addHands.checked \
							feet:addFeet.checked

			
			if (res == false) then return false
		)
	)
)

function rctMakeRagdollUI = 
(
	if undefined != rctFloater then closeRolloutFloater rctFloater
	rctFloater = newRolloutFloater "Ragdoll" 190 (415+RCT_MAX4_FLOATER_EXTENTION)
	addRollout rctCreateRagdollBipedRollout rctFloater rolledup:true
	addRollout rctCreateRagdollFromBipedRollout rctFloater
	
	callbacks.removeScripts #selectedNodesPostDelete id:#havok
	callbacks.removeScripts #selectionSetChanged id:#havok
	callbacks.addScript #selectedNodesPostDelete "rctCreateRagdollFromBipedRollout.updateRollout()" id:#havok
	callbacks.addScript #selectionSetChanged "rctCreateRagdollFromBipedRollout.updateRollout()" id:#havok
)


rctMakeRagdollUI()
