plugin helper YappModel

name:"YappModel" 
category:"Assembly Heads"
classID:#(0x3407673a, 0x3a43ef3)
extends:Point
replaceUI:true 
invisible:false 

( 
	parameters main rollout:params
	( 
		isInstance	type:#boolean default:true ui:cvInstance
	) 

	rollout params "General" 
	( 
		checkbox cvInstance "Instance" type:#boolean

		on params open do
		(
		)

		on params close do
		(
		) 
	)
	
	on create do
	(
		size = 10
		cross = true
	)
	
)