plugin Modifier ODEBody
	name:"ODE Body"
	category:"ODE"
	classID:#(0x3415acb0, 0xd6da53b)
	version:1
	initialRollupState:0x01
(
    parameters pcommon rollout:rcommon
    (
        exportThis type:#boolean ui:cbet default:true
    )

    parameters pdynamic rollout:rdynamic
    (
        m_isStatic type:#boolean ui:cbstatic default:false
		m_bodyShape type:#integer ui:bodyShapeDDL
		m_adjust type:#float ui:totalSP
    )
	
	parameters pjoint rollout:rjoint
	(
		m_isJoint type:#boolean ui:jointCB default:false
		m_jointType type:#integer ui:jointTypeDDL default:1
		m_a1Params type:#floatTab tabSize:11 ui:(a1p1SP, a1p2SP, a1p3SP, a1p4SP, a1p5SP, a1p6SP, a1p7SP, a1p8SP, a1p9SP, a1p10SP, a1p11SP)
		m_a2Params type:#floatTab tabSize:11 ui:(a2p1SP, a2p2SP, a2p3SP, a2p4SP, a2p5SP, a2p6SP, a2p7SP, a2p8SP, a2p9SP, a2p10SP, a2p11SP)
		m_a3Params type:#floatTab tabSize:11 ui:(a3p1SP, a3p2SP, a3p3SP, a3p4SP, a3p5SP, a3p6SP, a3p7SP, a3p8SP, a3p9SP, a3p10SP, a3p11SP)
	)
  
    rollout rcommon "Common Settings"
    (
        checkBox cbet "Export this entity"
    )

    rollout rdynamic "Dynamic Options"
    (
        checkBox cbstatic "Static (geom only)"
		spinner totalSP "Mass adjust" type:#float range:[-9999.9,9999.9,0.0]

		label lab131 "Body shape:" align:#left
		dropdownlist bodyShapeDDL items:#("TriMesh","Box","Cylinder","Sphere","Cone","Plane") align:#left
		
--		on bodyShapeDDL selected sel do
--		(
--			case sel of
--			(
--				2: ( -- Box
--				)
--				3: ( -- Cylinder
--				)
--				4: ( -- Sphere
--				)
--				5: ( -- Cone
--				)
--				6: ( -- Plane
--				)
--			)
--		)
	)

	rollout rjoint "Joint options"
	(
		checkBox jointCB "This object has joint"
		dropdownlist jointTypeDDL items:#("AMotor (3)","Ball (0)","Fixed (0)","Hinge (1)","Hinge2 (2)","Slider (1)","Universal (2)") align:#left
		group "Axis #1 Params"
		(
	       	spinner a1p1SP "lowStop" type:#float range:[-9999.9,9999.9,0.0]
			spinner a1p2SP "hiStop" type:#float range:[-9999.9,9999.9,0.0]
			spinner a1p3SP "vel" type:#float range:[-9999.9,9999.9,0.0]
			spinner a1p4SP "fMax" type:#float range:[0.0,99999.9,0.0]
			spinner a1p5SP "fudgeFactor" type:#float range:[0.0,1.0,1.0]
			spinner a1p6SP "bounce" type:#float range:[0.0,1.0,0.0]
			spinner a1p7SP "CFM" type:#float range:[0.0,9999.9,0.0]
			spinner a1p8SP "stopERP" type:#float range:[0.0,1.0,0.2]
			spinner a1p9SP "stopCFM" type:#float range:[0.0,9999.9,0.0]
			spinner a1p10SP "suspens.ERP" type:#float range:[0.0,1.0,0.2]
			spinner a1p11SP "suspens.CFM" type:#float range:[0.0,9999.9,0.0]
		)
		group "Axis #2 Params"
		(
	       	spinner a2p1SP "lowStop" type:#float range:[-9999.9,9999.9,0.0]
			spinner a2p2SP "hiStop" type:#float range:[-9999.9,9999.9,0.0]
			spinner a2p3SP "vel" type:#float range:[-9999.9,9999.9,0.0]
			spinner a2p4SP "fMax" type:#float range:[0.0,99999.9,0.0]
			spinner a2p5SP "fudgeFactor" type:#float range:[0.0,1.0,1.0]
			spinner a2p6SP "bounce" type:#float range:[0.0,1.0,0.0]
			spinner a2p7SP "CFM" type:#float range:[0.0,9999.9,0.0]
			spinner a2p8SP "stopERP" type:#float range:[0.0,1.0,0.2]
			spinner a2p9SP "stopCFM" type:#float range:[0.0,9999.9,0.0]
			spinner a2p10SP "suspens.ERP" type:#float range:[0.0,1.0,0.2]
			spinner a2p11SP "suspens.CFM" type:#float range:[0.0,9999.9,0.0]
		)
		group "Axis #3 Params"
		(
	       	spinner a3p1SP "lowStop" type:#float range:[-9999.9,9999.9,0.0]
			spinner a3p2SP "hiStop" type:#float range:[-9999.9,9999.9,0.0]
			spinner a3p3SP "vel" type:#float range:[-9999.9,9999.9,0.0]
			spinner a3p4SP "fMax" type:#float range:[0.0,99999.9,0.0]
			spinner a3p5SP "fudgeFactor" type:#float range:[0.0,1.0,1.0]
			spinner a3p6SP "bounce" type:#float range:[0.0,1.0,0.0]
			spinner a3p7SP "CFM" type:#float range:[0.0,9999.9,0.0]
			spinner a3p8SP "stopERP" type:#float range:[0.0,1.0,0.2]
			spinner a3p9SP "stopCFM" type:#float range:[0.0,9999.9,0.0]
			spinner a3p10SP "suspens.ERP" type:#float range:[0.0,1.0,0.2]
			spinner a3p11SP "suspens.CFM" type:#float range:[0.0,9999.9,0.0]
		)
		
		function EnableAxis axis state =
		(
			if (axis == 1) then
			(
		       	a1p1SP.enabled = state
				a1p2SP.enabled = state
				a1p3SP.enabled = state
				a1p4SP.enabled = state
				a1p5SP.enabled = state
				a1p6SP.enabled = state
				a1p7SP.enabled = state
				a1p8SP.enabled = state
				a1p9SP.enabled = state
				a1p10SP.enabled = state
				a1p11SP.enabled = state
			)
			else
			if (axis == 2) then
			(
		       	a2p1SP.enabled = state
				a2p2SP.enabled = state
				a2p3SP.enabled = state
				a2p4SP.enabled = state
				a2p5SP.enabled = state
				a2p6SP.enabled = state
				a2p7SP.enabled = state
				a2p8SP.enabled = state
				a2p9SP.enabled = state
				a2p10SP.enabled = state
				a2p11SP.enabled = state
			)
			else
			if (axis == 3) then
			(
		       	a3p1SP.enabled = state
				a3p2SP.enabled = state
				a3p3SP.enabled = state
				a3p4SP.enabled = state
				a3p5SP.enabled = state
				a3p6SP.enabled = state
				a3p7SP.enabled = state
				a3p8SP.enabled = state
				a3p9SP.enabled = state
				a3p10SP.enabled = state
				a3p11SP.enabled = state
			)
		)
		
		function SetAxisState type =
		(
			case type of
			(
				1: ( -- amotor
				EnableAxis 1 true
				EnableAxis 2 true
				EnableAxis 3 true
				)
				2: ( -- ball
				EnableAxis 1 false; 
				EnableAxis 2 false; 
				EnableAxis 3 false;
				)
				3: ( -- fixed
				EnableAxis 1 false; 
				EnableAxis 2 false; 
				EnableAxis 3 false;
				)
				4: ( -- hinge
				EnableAxis 1 true
				EnableAxis 2 false
				EnableAxis 3 false
				)
				5: ( -- hinge2
				EnableAxis 1 true; 
				EnableAxis 2 true; 
				EnableAxis 3 false;
				)
				6: ( -- slider
				EnableAxis 1 true; 
				EnableAxis 2 false; 
				EnableAxis 3 false;
				)
				7: ( -- universal
				EnableAxis 1 true; 
				EnableAxis 2 true; 
				EnableAxis 3 false;
				)
			)
		)
		
		on jointCB changed val do
		(
			jointTypeDDL.enabled = val
			if ((val == true) and (jointTypeDDL.selection != undefined)) then
			(
				SetAxisState (jointTypeDDL.selection)
			)
			else
			(
				SetAxisState 2
			)
		)
		
		on jointTypeDDL selected sel do
		(
			SetAxisState sel
		)
		
		on rjoint open do
		(
			jointTypeDDL.enabled = m_isJoint
			if ((m_isJoint == true) and (jointTypeDDL.selection != undefined)) then
			(
				SetAxisState (jointTypeDDL.selection)
			)
			else
			(
				SetAxisState 2
			)
		)
	)
	
	on attachedToNode theNode do 
	(
		if (classOf theNode.baseObject == box) then
		(
			m_bodyShape = 2
		)
		else
		if (classOf theNode.baseObject == cylinder) then
		(
			m_bodyShape = 3
		)
		else
		if ((classOf theNode.baseObject == sphere) or (classOf theNode.baseObject == geosphere))then
		(
			m_bodyShape = 4
		)
		else
		if (classOf theNode.baseObject == cone) then
		(
			m_bodyShape = 5
			theNode.radius2 = 0.0
		)
		else
		if (classOf theNode.baseObject == plane) then
		(
			m_bodyShape = 6
		)
		else
		(
			m_bodyShape = 1
		)
		theNode.wireColor = red
	)
)