plugin helper Joint

name:"Joint" 
category:"Octopus"
classID:#(0x4801577f, 0x3043eb5)
extends:Dummy
replaceUI:true 
invisible:false 

( 
	parameters main rollout:jointparams 
	( 

		temp type:#integer default:0 ui:temp 

		on temp set val do
		( 

		)
	) 

	rollout jointparams "Joint Parameters" 
	( 
		spinner temp "Temp" type:#integer range:[-100, 100, 0]
		on jointparams open do
		(
		)

		on params close do
		(
		) 
	)
	
	on create do
	(
		boxsize = [5,5,5]
	)
	
)