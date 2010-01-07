plugin SimpleMod OctopusMeshModifier
	name:"OctopusExport"
	category:"Octopus"
	classID:#(0x7704982d, 0x668a89f)
	version:2
	initialRollupState:0x02
(
	local a_names = #();
	
	parameters pcommon rollout:rcommon
    (
		exportThis type:#boolean ui:cbet default:true
    )

	parameters pmesh rollout:rmesh
	(
		scale type:#float ui:spscale default:1.0
		flipYZ type:#boolean ui:cbfyz default:true
		flipNormal type:#boolean ui:cbfn default:false
		exportUV type:#boolean ui:cbeuv default:true
		exportColours type:#boolean ui:cbec default:false
		castsShadow type:#boolean ui:cboeshadow default:false
    )
	
	parameters panim rollout:ranim
	(
		atype type:#integer ui:typeDL default:1
		anames type:#stringTab tabSizeVariable:true
		afframes type:#intTab tabSizeVariable:true
		alframes type:#intTab tabSizeVariable:true
		artimes type:#floatTab tabSizeVariable:true
		fframe type:#integer ui:fframeSP
		lframe type:#integer ui:lframeSP
		rtime type:#float ui:realtSP
	)
  
	rollout rcommon "Common Settings"
    (
		checkBox cbet "Export this entity"
	)

    rollout rmesh "Mesh Export Settings"
    (
		checkBox cboeshadow "Casts shadow"
		spinner spscale "Scale" type:#float
	    checkBox cbfyz "Flip Y/Z" 
	    checkBox cbfn "Flip Normals" 
	    checkBox cbeuv "Export texture coordinates" 
	    checkBox cbec "Export colour information" 
	)

    rollout ranim "Animation Export Settings"
    (
		dropdownlist typeDL "Skeleton Animation Type" aligh:#right items:#("Skin only","Biped")
		group "Animations"
		(
		button addButton "Add name to list" align:#left width:80 across:2
		button delButton "Delete" align:#right width:40
		combobox namesCB align:#left height:5 width:140 items:a_names
		label lab2234 "Frames" align:#left
		spinner fframeSP "from " align:#right fieldwidth:40 type:#integer across:2 range:[animationRange.start, animationRange.end, animationRange.start]
		spinner lframeSP "to " align:#right fieldwidth:40 type:#integer range:[animationRange.start, animationRange.end, animationRange.start]

		label lab1234 "Animation length" align:#left
		spinner realtSP "(in seconds) " align:#right fieldwidth:60 type:#float
		)
		
		on addButton pressed do
		(
			if (namesCB.text != "") then
			(
				append a_names namesCB.text
				append anames namesCB.text				append afframes fframe
				append alframes lframe
				append artimes rtime
				
				max motion mode
				max modify mode
			)
		)
		
		on delButton pressed do
		(
			if ((namesCB.items.count > 0) and (namesCB.selection > 0)) then
			(
				ind = namesCB.selection
				deleteItem a_names ind
				deleteItem anames ind
				deleteItem afframes ind
				deleteItem alframes ind
				deleteItem artimes ind
				
				max motion mode
				max modify mode
			)
		)
		
		on namesCB selected num do
		(
			if (namesCB.items.count >= num) then
			(
				fframe = afframes[num]
				lframe = alframes[num]
				rtime = artimes[num]
			)
		)

		on fframeSP changed val do
		(
			if ((namesCB.items.count > 0) and (namesCB.selection > 0)) then
			(
				afframes[namesCB.selection] = fframe
			)
		)

		on lframeSP changed val do
		(
			if ((namesCB.items.count > 0) and (namesCB.selection > 0)) then
			(
				alframes[namesCB.selection] = lframe
			)
		)

		on realtSP changed val do
		(
			if ((namesCB.items.count > 0) and (namesCB.selection > 0)) then
			(
				artimes[namesCB.selection] = rtime
			)
		)
	)

	on map i p do
	(
	
	)
	
	on load do
	(
		a_names = #();
		for str in anames do
		(
			append a_names str
		)
	)
)