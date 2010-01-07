plugin helper ODEBoxMass
name:"Mass(Box)"
category:"Octopus(ODE)"
classID:#(0x1302532f, 0x3647e8a)
extends:BoxGizmo
replaceUI:false
invisible:false
--initialRollupState:0x01
( 
	parameters pbox rollout: boxparams
	( 
		m_density type:#float ui:densitySP default:1.0
	) 

	rollout boxparams "ODE Mass" 
	( 
		spinner densitySP "Density" type:#float
	)
)
----------------------------------------------------------
----------------------------------------------------------
plugin helper ODESphereMass
name:"Mass(Sphere)"
category:"Octopus(ODE)"
classID:#(0x2303342f, 0x3ab7eca)
extends:SphereGizmo
replaceUI:false
invisible:false 
--initialRollupState:0x01
( 
	parameters psphere rollout: sphereparams
	( 
		m_density type:#float ui:densitySP default:1.0
	) 

	rollout sphereparams"ODE Mass" 
	( 
		spinner densitySP "Density" type:#float
	)
)
----------------------------------------------------------
----------------------------------------------------------
plugin helper ODECylMass
name:"Mass(Cyl)"
category:"Octopus(ODE)"
classID:#(0x3a02012f, 0x3e46e8a)
extends:CylGizmo
replaceUI:false
invisible:false 
--initialRollupState:0x01
( 
	parameters pcyl rollout: cylparams
	( 
		m_density type:#float ui:densitySP default:1.0
	) 

	rollout cylparams "ODE Mass" 
	( 
		spinner densitySP "Density" type:#float
	)
)
----------------------------------------------------------
----------------------------------------------------------
plugin helper ODEStructMass
name:"Mass(Matrix)"
category:"Octopus(ODE)"
classID:#(0x3a02456b, 0x3f46a1b)
extends:Point
replaceUI:true
invisible:false 
--initialRollupState:0x01
( 
	parameters pcyl rollout: cylparams
	( 
		m_total type:#float ui:totalSP default:1.0

		m_mmx1 type:#floatTab tabsize:3 ui:(mmx11SP,mmx12SP,mmx13SP)
		m_mmx2 type:#floatTab tabsize:3 ui:(mmx21SP,mmx22SP,mmx23SP)
		m_mmx3 type:#floatTab tabsize:3 ui:(mmx31SP,mmx32SP,mmx33SP)
	) 

	rollout cylparams "ODE Mass" 
	( 
		spinner totalSP "Total mass" type:#float range:[-9999.9,9999.9,0.0]
		
		group "Mass matrix"
		(
	       	spinner mmx11SP type:#float range:[-9999.9,9999.9,0.0] fieldWidth:36 across:3
	       	spinner mmx12SP type:#float range:[-9999.9,9999.9,0.0] fieldWidth:36
	       	spinner mmx13SP type:#float range:[-9999.9,9999.9,0.0] fieldWidth:36
	       	spinner mmx21SP type:#float range:[-9999.9,9999.9,0.0] fieldWidth:36 across:3
	       	spinner mmx22SP type:#float range:[-9999.9,9999.9,0.0] fieldWidth:36
	       	spinner mmx23SP type:#float range:[-9999.9,9999.9,0.0] fieldWidth:36
	       	spinner mmx31SP type:#float range:[-9999.9,9999.9,0.0] fieldWidth:36 across:3
	       	spinner mmx32SP type:#float range:[-9999.9,9999.9,0.0] fieldWidth:36
	       	spinner mmx33SP type:#float range:[-9999.9,9999.9,0.0] fieldWidth:36
		)
	)
)
----------------------------------------------------------
----------------------------------------------------------
plugin helper OGRE_ODE_Link
name:"Link(Yapp)"
category:"Octopus(ODE)"
classID:#(0x428fab5c, 0x46e7a5b)
extends:Point
replaceUI:true
invisible:false
( 
	parameters pgeneral
	(
		m_isValid type:#boolean default:false
	)

	parameters pnodes rollout: linkparams
	( 
		m_node1 type:#node ui:node1PB
		m_valid1 type:#boolean default:false
		m_node2 type:#node ui:node2PB
		m_valid2 type:#boolean default:false
	)

	rollout linkparams "Link settings" 
	( 
		function pick_filter1 obj =
		(
			for mod in obj.modifiers do
			(
				if (mod.name == "ODE Body") do
				(
					if (mod.m_isStatic == false) then
					(
						return true
					)
					else
					return false
				)
			)
			return false
		)

		function pick_filter2 obj =
		(
			for mod in obj.modifiers do
			(
				if (mod.name == "ODE Body") do
					return false
			)
				
			if((Superclassof obj == GeometryClass) and (obj.isTarget == false)) then
			(
				return true
			)
			else if((Superclassof obj == Helper) and (classOf obj == Dummy)) then
			(
				return true
			)
			return false
		)
	
		group "Link Source Info"
		(
			label lb0101 "Picked: " align:#left across:2
			pickbutton node1PB "- none -" align:#right filter:pick_filter1
			label lb01_type "Type: unknown" align:#left
			label lb01_message "Error: must be ODE Body" align:#left
		)

		group "Link Destination Info"
		(
			label lb0102 "Picked: " align:#left across:2
			pickbutton node2PB "- none -" align:#right filter:pick_filter2
			label lb02_type "Type: unknown" align:#left
			label lb02_message "Error: must be Mesh or Dummy" align:#left
		)
		
		function checkNode1 obj =
		(
			this.m_valid1 = false
			if (obj != undefined) then
			(
				typeStr = "Type: unknown"
				capStr = "Error: must be ODE Body"
				node1PB.text = obj.name
				typeUnknown = true
				for mod in obj.modifiers do
				(
					if (mod.name == "ODE Body") do
					(
						typeUnknown = false
						if (mod.m_isStatic == false) then
						(
							this.m_valid1 = true
							typeStr = "Type: ODE Dynamic Body"
							capStr = ""
						)
						else
						(
							typeStr = "Type: ODE Static Body"
							capStr = "Error: Static not allowed"
						)
					)
				)
				
				lb01_type.caption = typeStr
				lb01_message.caption = capStr
			)
			else
			(
				node1PB.text = "- none -"
				lb01_type.caption = "Type: unknown"
				lb01_message.caption = "Error: must be ODE Body"
			)
		)
		
		function checkNode2 obj =
		(
			this.m_valid2 = false
			if (obj != undefined) then
			(
				typeStr = "Type: unknown"
				capStr = "Error: must be Mesh or Dummy"
				node2PB.text = obj.name
				typeUnknown = true
				for mod in obj.modifiers do
				(
					if (mod.name == "ODE Body") do
					(
						typeUnknown = false
						typeStr = "Type: ODE Body"
						capStr = "Error: ODE Body not allowed"
					)
				)
				
				if (typeUnknown) then
				(
					if((Superclassof obj == GeometryClass) and (obj.isTarget == false)) then
					(
						this.m_valid2 = true
						typeStr = "Type: OGRE Mesh"
						capStr = ""
					)
					else if((Superclassof obj == Helper) and (classOf obj == Dummy)) then
					(
						this.m_valid2 = true
						typeStr = "Type: Dummy"
						capStr = ""
					)
				)
				
				lb02_type.caption = typeStr
				lb02_message.caption = capStr
			)
			else
			(
				node2PB.text = "- none -"
				lb02_type.caption = "Type: unknown"
				lb02_message.caption = "Error: must be Mesh or Dummy"
			)
		)

		on node1PB picked obj do
		(
			checkNode1 obj
			m_isValid = (m_valid1 and m_valid2)
			if (delegate.box != m_isValid) do
				delegate.box = m_isValid
		)

		on node2PB picked obj do
		(
			checkNode2 obj
			m_isValid = (m_valid1 and m_valid2)
			if (delegate.box != m_isValid) do
				delegate.box = m_isValid
		)
		
		on linkparams open do
		(
			checkNode1 node1PB.object
			checkNode2 node2PB.object
			m_isValid = (m_valid1 and m_valid2)
			if (delegate.box != m_isValid) do
				delegate.box = m_isValid
		)
	)
	
	on load do
	(
		delegate.drawontop = true
		delegate.centermarker = true
		delegate.cross = true
		delegate.box = m_isValid
	)

	on create do
	(
		delegate.drawontop = true
		delegate.centermarker = true
		delegate.cross = true
		delegate.box = false
	)
)