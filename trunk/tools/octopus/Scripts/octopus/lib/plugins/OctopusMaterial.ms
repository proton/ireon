include "octopus/lib/OctopusMatExport.ms"

plugin material OctopusPassMaterial
    name:"OGRE (single/pass)"
    classID:#(0x7616301e, 0x34fb27dc)
    extends:Standard 
	replaceUI:true
	initialRollupState:0x03
(
	function remaptextures tunits ttypes =
	(
		if (tunits != undefined) then
		(
			local i_ons = #(0,0,0,0,0,0,0,0,0,0,0,0)
			
			for i = 1 to 4 do
			(
				if (tunits[i] != undefined) then
				(
					if ttypes[i] > 1 then i_ons[ttypes[i]] = i
				)
			)
			
			for i = 2 to 12 do
			(
				if (i_ons[i] > 0) then
				(
					delegate.maps[i] = tunits[i_ons[i]]
					delegate.mapEnables[i] = true
				)
				else
				(
					delegate.mapEnables[i] = false
					delegate.maps[i] = undefined
				)
			)
		)
	)

	parameters pshadertype rollout:shaderselect
	(
    	m_shadertype type:#integer default:2 ui:shaderDDL
		m_lighting type:#boolean default:true ui:lightingCB
		m_fogoverride type:#boolean ui:fogoverrideCB default:true
        
        on m_shadertype set val do
		(
			if (val == 1) then --flat
			(
				delegate.shaderType = 1
				delegate.faceted = true
			)
			else
			if (val == 2) then --gouraud
			(
				delegate.shaderType = 1
				delegate.faceted = false
			)
			if (val == 3) then --phong
			(
				delegate.shaderType = 5
				delegate.faceted = false
			)
		)
	)
	
	parameters pshaderopt rollout:shaderparams
	(
		m_ambcolor type:#color ui:ambientCP default:[255,255,255]
		m_diffcolor type:#color ui:diffuseCP default:[255,255,255]
		m_speccolor type:#color ui:specularCP default:[0,0,0]
		m_sicolor type:#color ui:selfillumCP default:[0,0,0]
		m_siamount type:#integer ui:selfillumSP default:0
		m_sichecked type:#boolean ui:selfillumCB default:false
		m_speclevel type:#integer ui:speclevelSP default:0
		m_specgloss type:#integer ui:specglossSP default:0
		
		on m_ambcolor set val do delegate.ambient = val
		on m_diffcolor set val do
		(
			delegate.diffuse = val
			if (sichecked == false) then
				delegate.self_Illum_color = val * ((m_siamount as float)/ 100.0)
		)
		on m_speccolor set val do
		(
			delegate.specular = val * (m_speclevel as float/ 100.0)
		)
		on m_sicolor set val do delegate.self_Illum_color = val
		on m_siamount set val do delegate.self_Illum_color = m_diffcolor * ((val as float)/ 100.0)
		on m_speclevel set val do
		(
			delegate.specular = m_speccolor * (val as float/ 100.0)
			delegate.specularLevel = val
		)
		on m_specgloss set val do
		(
			delegate.Glossiness = val
		)
	)
	
	parameters pmaps rollout:maps
	(
		TextureUnits type:#texturemapTab tabSize:4 ui:(tu1MB,tu2MB,tu3MB,tu4MB)
		m_tutypes type:#intTab tabSize:4 ui:(tu1DDL,tu2DDL,tu3DDL,tu4DDL) default:1
		
		on TextureUnits set newtexture do
		(
--			if ((newtexture != undefined) and (classof newtexture != OctopusTextureUnit)) then
--				MessageBox "\nThis texture class is not supported by OGRE\nPlease use \"OGRE Texture Unit\" instead\n" title:"Warning!"
			remaptextures TextureUnits m_tutypes
		)
		
		on m_tutypes set val do
		(
			local i_def = #(1,1,1,1)
			local j_def = 0
			for i = 1 to 4 do
			(
				if (m_tutypes[i] != 1) then
				(
					for j = 1 to j_def do
					(
						if (i_def[j] == m_tutypes[i]) then
						(
							m_tutypes[i] = 1
						)
					)
					if (m_tutypes[i] != 1) then 
					(
						i_def[i] = m_tutypes[i]
						j_def = j_def + 1
					)
				)
			)
			remaptextures TextureUnits m_tutypes
		)
	)
	
	parameters pogreoptions rollout:ogreparams
	(
		m_sceneblend_sel type:#integer ui:blendmodeRB default:1	-- 1:opaque 2:add 3:modulate 4:alpha_blend 5:custom
		m_sceneblend_src type:#integer ui:blendsrcDDL default:1	-- scene_blend source
		m_sceneblend_dest type:#integer ui:blenddestDDL default:2-- scene_blend dest
		m_depthcheck type:#boolean ui:depthcheckCB default:true	-- depth_check
		m_depthwrite type:#boolean ui:depthwriteCB default:true	-- depth_write
		m_depthfunc type:#integer ui:depthfuncDDL default:4
		m_depthbias type:#integer ui:depthbiasSP default:0
		m_cullhardware type:#integer ui:cullhardwareDDL default:1
		m_cullsoftware type:#integer ui:cullsoftwareDDL default:1
		m_colourwrite type:#boolean ui:colourwriteCB default:true
	)
    
	rollout shaderselect "Shader Basic Parameters"
	(
		dropdownlist shaderDDL items:#("Flat","Gouraud","Phong") align:#left offset:[-10,0] across:4
		checkBox lightingCB "Apply lighting" align:#right
		checkBox fogoverrideCB "Apply fog" align:#right
		button previewBT images:#("UI\\Icons\\SnapsControls_24i.bmp",undefined,32,9,9,9,9) tooltip:"Preview OGRE material" align:#right
		
		on previewBT pressed do
		(
			previewOctopusMaterial (this)
		)
	)
	
	rollout shaderparams "Shader Lighting Parameters"
	(
		groupBox gb01 width:160 height:110 offset:[-7,-6] 
		colorpicker ambientCP "Ambient:" aligh:#left fieldWidth:50 offset:[14,-95]
		colorpicker diffuseCP "Diffuse:" aligh:#left fieldWidth:50 offset:[20,0]
		colorpicker specularCP "Specular:" aligh:#left fieldWidth:50 offset:[9,0]
		
		groupBox gb02 "Self-Illumination" width:146 height:50 offset:[160,-95] 
		checkBox selfillumCB "Color" aligh:#left offset:[170,-35]
		spinner selfillumSP type:#integer fieldWidth:35 range:[0,100,0] align:#left offset:[230,-22]
		colorpicker selfillumCP  aligh:#left fieldWidth:50 offset:[230,-27]
		
		groupBox gb03 "Specular Highlights" aligh:#left width:314 height:50 offset:[-7,74]
		spinner speclevelSP "Specular " type:#integer fieldWidth:35 range:[0,100,0] align:#left offset:[20,-32]
		spinner specglossSP "Glossiness " type:#integer fieldWidth:35 range:[0,100,0] align:#left offset:[160,-24]
		
		on selfillumCB changed state do
		(
			if (state) then
			(
				selfillumSP.visible = false
				selfillumCP.visible = true
				delegate.self_Illum_color = m_sicolor
			)
			else
			(
				selfillumSP.visible = true
				selfillumCP.visible = false
				delegate.self_Illum_color = m_diffcolor * ((m_siamount as float)/ 100.0)
			)
		)
		
		on shaderparams open do
		(
			if (selfillumCB.checked) then
			(
				selfillumSP.visible = false
				selfillumCP.visible = true
			)
			else
			(
				selfillumSP.visible = true
				selfillumCP.visible = false
			)
		)	
	)
	
	rollout maps "Maps (texture units)"
	(
		label lab13 "Map 1:" width:50 align:#left across:3
		mapbutton tu1MB width:160 align:#right
		dropdownlist tu1DDL width:80 align:#right items:#("--hide--","Diffuse","Specular","Spec.Level","Glossiness","SelfIllum","Opacity","Filter","Bump","Reflection","Refraction","Displacement")

		label lab14 "Map 2:" width:50 align:#left across:3
		mapbutton tu2MB width:160 align:#right
		dropdownlist tu2DDL width:80 align:#right items:#("--hide--","Diffuse","Specular","Spec.Level","Glossiness","SelfIllum","Opacity","Filter","Bump","Reflection","Refraction","Displacement")

		label lab15 "Map 3:" width:50 align:#left across:3
		mapbutton tu3MB width:160 align:#right
		dropdownlist tu3DDL width:80 align:#right items:#("--hide--","Diffuse","Specular","Spec.Level","Glossiness","SelfIllum","Opacity","Filter","Bump","Reflection","Refraction","Displacement")

		label lab16 "Map 4:" width:50 align:#left across:3
		mapbutton tu4MB width:160 align:#right
		dropdownlist tu4DDL width:80 align:#right items:#("--hide--","Diffuse","Specular","Spec.Level","Glossiness","SelfIllum","Opacity","Filter","Bump","Reflection","Refraction","Displacement")
	)
	
	rollout ogreparams "OGRE Material Parameters"
	(
		Button setdefaultBT	"Set to Default" align:#right
		group "Blending"
		(
			radiobuttons blendmodeRB labels:#("Opaque  ","Add      ","Modulate","AlphaBlend","Custom") align:#left column:4 default:1
			dropdownlist blendsrcDDL "Source factor" items:#("one ","zero","DEST.colour","SRC.colour","1-(DEST.colour) ","1-(SRC.colour)","DEST.alpha","SRC.alpha","1-(DEST.alpha)","1-(SRC.alpha)") default:1 width:120 align:#left across:2
			dropdownlist blenddestDDL "Destination factor" items:#("one ","zero","DEST.colour","SRC.colour","1-(DEST.colour) ","1-(SRC.colour)","DEST.alpha","SRC.alpha","1-(DEST.alpha)","1-(SRC.alpha)") default:1 width:120 align:#left
		)
		
		group "Depth and Colour Buffers"
		(
			checkBox colourwriteCB "Write to colour buffer" align:#left width:120
			checkBox depthwriteCB "Write to depth buffer" align:#left width:120
			checkBox depthcheckCB "Check depth buffer" align:#left width:120
			label lab10 "Check function:" align:#left offset:[40,0]
			dropdownlist depthfuncDDL items:#("always fail","always pass","less","less or equal","equal","not equal","greater or equal","greater")  default:4 width:120 align:#left offset:[150,-24]
			spinner depthbiasSP "Depth bias " type:#integer fieldWidth:50 range:[0,16,0] align:#left
		)
		
		group "Culling"
		(
			label lab11 "Hardware culling mode:" align:#left
			dropdownlist cullhardwareDDL items:#("clockwise","anitclockwise","none") default:1 width:120 align:#left offset:[160,-24]
			label lab12 "Software culling mode:" align:#left
			dropdownlist cullsoftwareDDL items:#("back","front","none") default:1 width:120 align:#left offset:[160,-24]
		)
		
		on setdefaultBT pressed do
		(
			m_sceneblend_sel = 1
			m_sceneblend_src = 1
			m_sceneblend_dest = 2
			m_depthcheck = true
			m_depthwrite = true
			m_depthfunc = 4
			m_depthbias = 0
			m_cullhardware = 1
			m_cullsoftware = 1
			m_colourwrite = true
			
			blendsrcDDL.enabled = false
			blenddestDDL.enabled = false
			depthcheckCB.enabled = true
			depthfuncDDL.enabled = true
			lab10.enabled = true
		)
		
		on blendmodeRB changed state do
		(
			if (state == 1) then
			(
				m_sceneblend_src = 1
				m_sceneblend_dest = 2
				blendsrcDDL.enabled = false
				blenddestDDL.enabled = false
			)
			else
			if (state == 2) then
			(
				m_sceneblend_src = 1
				m_sceneblend_dest = 1
				blendsrcDDL.enabled = false
				blenddestDDL.enabled = false
			)
			else
			if (state == 3) then
			(
				m_sceneblend_src = 4
				m_sceneblend_dest = 6
				blendsrcDDL.enabled = false
				blenddestDDL.enabled = false
			)
			else
			if (state == 4) then
			(
				m_sceneblend_src = 8
				m_sceneblend_dest = 10
				blendsrcDDL.enabled = false
				blenddestDDL.enabled = false			)
			else
			if (state == 5) then
			(
				blendsrcDDL.enabled = true
				blenddestDDL.enabled = true
			)
		)
		
		on depthwriteCB changed state do
		(
			depthcheckCB.enabled = state
			depthfuncDDL.enabled = state and depthcheckCB.checked
			lab10.enabled = state and depthcheckCB.checked
		)

		on depthcheckCB changed state do
		(
			lab10.enabled = state
			depthfuncDDL.enabled = state
		)
		
		on ogreparams open do
		(
			if(blendmodeRB.state == 5) then
			(
				blendsrcDDL.enabled = true
				blenddestDDL.enabled = true
			)
			else
			(
				blendsrcDDL.enabled = false
				blenddestDDL.enabled = false
			)
		)
	)
	
	rollout vfprograms "Vertex and Fragment Programs"
	(
	)
	
	on create do 
	(
		delegate.useSelfIllumColor=true
		delegate.soften = 0.5
		m_sichecked = false;
	)
)
