plugin textureMap OctopusTextureUnit
    name:"OGRE Texture Unit"
    classID:#(0x7661301e, 0x3f42b7dc)
    extends:Bitmaptexture
    replaceUI:true
(
	parameters pcoords rollout:coordparams
	(
		m_coordset type:#integer ui:coordsetSP default:1	-- OGRE: tex_coord_set 		MAX: mapChannel
		m_offsetx type:#float ui:offsetxSP default:0.0 		-- OGRE: scroll 			MAX: U_Offset
		m_offsety type:#float ui:offsetySP default:0.0		-- OGRE: scroll 			MAX: V_Offset
		m_tilingx type:#float ui:tilingxSP default:1.0		-- OGRE: scale 				MAX: U_Tiling
		m_tilingy type:#float ui:tilingySP default:1.0		-- OGRE: scale 				MAX: V_Tiling
		m_mirror type:#boolean ui:mirrorCB default:false	-- OGRE: tex_address_mode 	MAX: U_Mirror,V_Mirror
		m_tile type:#boolean ui:tileCB default:true			-- OGRE: tex_address_mode 	MAX: U_Tile,V_Tile
		m_angle type:#float ui:angleSP default:0.0			-- OGRE: rotate				MAX: W_Angle
		
		on m_coordset set val do
			delegate.coordinates.mapChannel = val

		on m_offsetx set val do
			delegate.coordinates.U_Offset  = val

		on m_offsety set val do
			delegate.coordinates.V_Offset  = val

		on m_tilingx set val do
		(
			if (val == 0.0) then
				delegate.coordinates.U_Tiling = 0.01
			else
				delegate.coordinates.U_Tiling = val
		)

		on m_tilingy set val do
		(
			if (val == 0.0) then
				delegate.coordinates.V_Tiling = 0.01
			else
				delegate.coordinates.V_Tiling = val
		)

		on m_mirror set val do
		(
			m_tile = ( (not val) and m_tile )
			
			delegate.coordinates.U_Mirror = val
			delegate.coordinates.V_Mirror = val
			delegate.coordinates.U_Tile = m_tile
			delegate.coordinates.V_Tile = m_tile
		)

		on m_tile set val do
		(
			m_mirror = ( (not val) and m_mirror )

			delegate.coordinates.U_Mirror = m_mirror
			delegate.coordinates.V_Mirror = m_mirror
			delegate.coordinates.U_Tile = val
			delegate.coordinates.V_Tile = val
		)
		
		on m_angle set val do
			delegate.coordinates.W_Angle = val
	)

	parameters pbitmap rollout:bitmapparams
	(
		m_texttype type:#integer ui:texttypeRB default:1		--!OGRE: texture,anim_texture,cubic_texture MAX: 
		m_combsep type:#integer ui:combsepRB default:1			--!OGRE: cubic_texture	MAX:
		m_text1d2d type:#integer ui:text1d2dRB default:2		--!OGRE: texture 		MAX:
		m_nframes type:#integer ui:numframesSB default:0		--!OGRE: anim_texture	MAX:
		m_duration type:#float ui:durationSB default:0.0		--!OGRE: anim_texture	MAX:
		m_bitmapname type:#string default:""					-- OGRE: texture,anim_texture,cubic_texture MAX: filename
		m_filtering type:#integer ui:filteringRB default:2		-- OGRE: filtering		MAX: filtering
		m_filtermin type:#integer ui:filterminDDL default:2		-- OGRE: filtering		MAX: filtering
		m_filtermag type:#integer ui:filtermagDDL default:2		-- OGRE: filtering		MAX: filtering
		m_filtermip type:#integer ui:filtermipDDL default:2		-- OGRE: filtering		MAX: filtering
		m_maxanisotropy type:#integer ui:maxanisSP default:1	--!OGRE: filtering		MAX: -
		m_envmap type:#boolean ui:envmapCB default:false		--!OGRE: env_map		MAX:
		m_mapping2D type:#integer ui:mapping1DDL default:1		--!OGRE: env_map		MAX:
		m_mappingCubic type:#integer ui:mapping2DDL default:1	--!OGRE: env_map		MAX:
		
		on m_bitmapname set val do
		(
			if ((val != undefined) and (val != "")) then
			(
				delegate.filename = val
			)
		)
		
		on m_texttype set val do
		(
			if ((val == 2) or ((val == 1) and (m_text1d2d == 1)) or ((val == 3) and (m_combsep == 2))) then m_envmap = false
		)
		
		on m_text1d2d set val do
		(
			if (val == 1) then m_envmap = false
		)

		on m_combsep set val do
		(
			if (val == 2) then m_envmap = false
		)
		
		on m_filtermin set val do
		(
			if ((val == 1) or (m_filtermag == 1)) then -- point
			(
				delegate.filtering = 2 --none
			)
			else
			if ((val == 2) or (m_filtermag == 2)) then -- linear
			(
				delegate.filtering = 0 -- pyramidal
			)
			else
			if ((val == 3) or (m_filtermag == 1)) then -- anisotropic
			(
				delegate.filtering = 1 -- summed area
			)
		)

		on m_filtermag set val do
		(
			if ((val == 1) or (m_filtermin == 1)) then -- point
			(
				delegate.filtering = 2 --none
			)
			else
			if ((val == 2) or (m_filtermin == 2)) then -- linear
			(
				delegate.filtering = 0 -- pyramidal
			)
			else
			if ((val == 3) or (m_filtermin == 1)) then -- anisotropic
			(
				delegate.filtering = 1 -- summed area
			)
		)
	)

	parameters pblend rollout:blendings
	(
		m_colourop type:#integer ui:colouropDDL default:3
		
		m_colouropex_type ui:operDDL type:#integer default:3
		m_colouropex_s1 ui:src1DDL type:#integer default:1
		m_colouropex_s2 ui:src2DDL type:#integer default:2
		m_colouropex_mf ui:manfSP type:#float default:0.0
		m_colouropex_m1 ui:mansrc1CP type:#color default:[0,0,0]
		m_colouropex_m2 ui:mansrc2CP type:#color default:[0,0,0]
		
		m_fallback ui:fallbackCB type:#boolean default:false
		m_fallback_s ui:fallbacksDDL type:#integer default:1
		m_fallback_d ui:fallbackdDDL type:#integer default:2
		
		m_alphaopex_type ui:alphaoperDDL type:#integer default:3
		m_alphaopex_s1 ui:alphasrc1DDL type:#integer default:1
		m_alphaopex_s2 ui:alphasrc2DDL type:#integer default:2
		m_alphaopex_mf ui:alphamanfSP type:#float default:0.0
		m_alphaopex_m1 ui:alphamans1SP type:#float default:0.0
		m_alphaopex_m2 ui:alphamans2SP type:#float default:0.0
	)
	
	parameters panim rollout:animations
	(
		m_doscroll type:#boolean ui:scrollanimCB default:false
		m_dorotate type:#boolean ui:rotateanimCB default:false
		m_scrollanimx type:#float ui:scrollanimxSP default:0.0
		m_scrollanimy type:#float ui:scrollanimySP default:0.0
		m_rotateanim type:#float ui:rotateanimSP default:0.0
		m_xformtype type:#intTab tabSize:5 tabSizeVariable:true ui:(xftype1DDL,xftype2DDL,xftype3DDL,xftype4DDL,xftype5DDL) default:1
		m_xwavetype type:#intTab tabSize:5 tabSizeVariable:true ui:(xfwave1DDL,xfwave2DDL,xfwave3DDL,xfwave4DDL,xfwave5DDL) default:1
		m_xbase type:#floatTab tabSize:5 tabSizeVariable:true ui:(xfbase1SP,xfbase2SP,xfbase3SP,xfbase4SP,xfbase5SP) default:0.0
		m_xfreq type:#floatTab tabSize:5 tabSizeVariable:true ui:(xffreq1SP,xffreq2SP,xffreq3SP,xffreq4SP,xffreq5SP) default:0.0
		m_xphase type:#floatTab tabSize:5 tabSizeVariable:true ui:(xfphase1SP,xfphase2SP,xfphase3SP,xfphase4SP,xfphase5SP) default:0.0
		m_xamp type:#floatTab tabSize:5 tabSizeVariable:true ui:(xfamp1SP,xfamp2SP,xfamp3SP,xfamp4SP,xfamp5SP) default:0.0
	)

	rollout coordparams  "Coordinates"
	(
		label labc01 "Offset" across:5
		label labc02 "Tiling"
		label labc03 "Mirror"
		label labc04 "Tile"
		label labc05 "Angle"
	
		spinner offsetxSP "X:" type:#float range:[-100.0,100.0,0.0] scale:0.01 fieldwidth:40 across:5
		spinner tilingxSP type:#float range:[0.0,9999.0,1.0] scale:0.1 fieldwidth:40
		checkBox mirrorCB align:#center
		checkBox tileCB align:#center
		spinner angleSP type:#float range:[-180.0,180.0,0.0] scale:1.0 fieldwidth:40
	
		spinner offsetySP "Y:" type:#float range:[-99.0,99.0,0.0] scale:0.01 fieldwidth:40 across:5
		spinner tilingySP type:#float range:[0.0,9999.0,1.0] scale:0.1 fieldwidth:40
		checkBox mirror2CB align:#center visible:false
		checkBox tile2CB align:#center visible:false
		spinner angle2SP visible:false
	
		label labc11 "Map Channel:" align:#right offset:[-70,0]
		spinner coordsetSP type:#integer range:[1,99,1] fieldwidth:40 offset:[0,-24]
	)

	rollout bitmapparams  "Bitmap Parameters"
	(
		label labbm01 "Bitmap:" align:#left across:3 
		button fileButton "None" width:140 align:#right
		button reloadButton "Reload" align:#right
		group "Texture Type"
		(
			radiobuttons texttypeRB labels:#("Texture","Animated","Cubic") columns:1 align:#left
			radiobuttons text1d2dRB labels:#("1D","2D") columns:1 align:#left offset:[85,-50]
			spinner numframesSB "Frames:" type:#integer range:[0,999,0] fieldwidth:60 align:#left offset:[85,-30] visible:false
			spinner durationSB "Duration:" type:#float range:[0.0,99.0,0.0] fieldwidth:60 align:#left offset:[85,0] visible:false
			radiobuttons combsepRB labels:#("Combined UVW (reflection)","Separate UV (skybox)") columns:1 width:100 align:#left offset:[85,-40] visible:false
			label labt00 "" visible:false align:#left
			checkBox envmapCB "Environment mapping" align:#right width:120 offset:[50,0] across:2
			dropdownlist mapping1DDL items:#("spherical","planar") width:80 align:#right
			dropdownlist mapping2DDL items:#("reflection","normal") width:80 align:#right offset:[0,-27]
		)
		Group "Filtering"
		(
			radiobuttons filteringRB labels:#("None","Bilinear","Trilinear","Anisotropic","Custom") columns:1 align:#left
			label labf01 "Minification:" align:#left width:150 offset:[118,-80]
			dropdownlist filterminDDL items:#("point","linear","anisotropic") aligh:#left width:80 offset:[200,-25]
			label labf02 "Magnification:" align:#left width:150 offset:[108, 0]
			dropdownlist filtermagDDL items:#("point","linear","anisotropic") aligh:#left width:80 offset:[200,-25]
			label labf03 "Mip:" align:#left width:150 offset:[120, 0] 
			dropdownlist filtermipDDL items:#("none","point","linear") align:#left width:80 offset:[200,-25]
			label labf04 "Max anisotropy degree:" align:#left offset:[130,5]
			spinner maxanisSP type:#integer range:[1,10,1] fieldwidth:34 align:#left offset:[240,-20]
			label labf06 "" visible:false align:#left
		)
		
		on fileButton pressed do
		(
			tmpname = getBitmapOpenFileName()
			if (tmpname != indefined) then
			(
				m_bitmapname = tmpname
				fileButton.caption = m_bitmapname
			)
		)
		
		on reloadButton pressed do
		(
			delegate.reload()
		)
		
		on combsepRB changed state do
		(
			if (state == 1) then
			(
				envmapCB.enabled = true
				mapping1DDL.visible = false
				mapping2DDL.visible = true
			)
			else
			(
				envmapCB.enabled = false
				mapping1DDL.visible = false
				mapping2DDL.visible = false
			)
		)

		on text1d2dRB changed state do
		(
			if (state == 2) then
			(
				envmapCB.enabled = true
				mapping1DDL.visible = true
				mapping2DDL.visible = false
			)
			else
			(
				envmapCB.enabled = false
				mapping1DDL.visible = false
				mapping2DDL.visible = false
			)
		)
		
		on filteringRB changed state do
		(
			val = ( state == 5 )
			labf01.enabled = val
			labf02.enabled = val
			labf03.enabled = val
			filterminDDL.enabled = val
			filtermagDDL.enabled = val
			filtermipDDL.enabled = val
		
			if (state == 1) then
			(
				m_filtermin = 1
				m_filtermag = 1
				m_filtermip = 1
			)
			else
			if (state == 2) then
			(
				m_filtermin = 2
				m_filtermag = 2
				m_filtermip = 2
			)
			else
			if (state == 3) then
			(
				m_filtermin = 2
				m_filtermag = 2
				m_filtermip = 3
			)
			else
			if (state == 4) then
			(
				m_filtermin = 3
				m_filtermag = 3
				m_filtermip = 3
			)
			
			if ((m_filtermin == 3) or (m_filtermag == 3)) then
			(
				labf04.enabled = true
				maxanisSP.enabled = true
			)	
			else
			(
				labf04.enabled = false
				maxanisSP.enabled = false
			)
		)
		
		on filterminDDL selected num do
		(
			val = (( num == 3) or (m_filtermag == 3))
			labf04.enabled = val
			maxanisSP.enabled = val
		)

		on filtermagDDL selected num do
		(
			val = (( num == 3) or (m_filtermin == 3))
			labf04.enabled = val
			maxanisSP.enabled = val
		)
		
		on texttypeRB changed num do
		(
			if (num == 1) then
			(
				numframesSB.visible = false
				durationSB.visible = false
				combsepRB.visible = false
				text1d2dRB.visible = true
			)
			else
			if (num == 2) then
			(
				text1d2dRB.visible = false
				combsepRB.visible = false
				numframesSB.visible = true
				durationSB.visible = true
			)
			else
			if (num == 3) then
			(
				text1d2dRB.visible = false
				numframesSB.visible = false
				durationSB.visible = false
				combsepRB.visible = true
			)
			
			if ((m_texttype == 1) and (m_text1d2d == 2)) then
			(
				envmapCB.enabled = true
				mapping1DDL.visible = true
				mapping2DDL.visible = false
			)
			else
			if ((m_texttype == 3) and (m_combsep == 1)) then
			(
				envmapCB.enabled = true
				mapping1DDL.visible = false
				mapping2DDL.visible = true
			)
			else
			(
				envmapCB.enabled = false
				mapping1DDL.visible = false
				mapping2DDL.visible = false
			)
		)
		
		on bitmapparams open do
		(
			if ( m_bitmapname == "") then
				bitmapparams.fileButton.caption = "None"
			else
				bitmapparams.fileButton.caption = m_bitmapname
				
			val = ( m_filtering == 5 )
			labf01.enabled = val
			labf02.enabled = val
			labf03.enabled = val
			filterminDDL.enabled = val
			filtermagDDL.enabled = val
			filtermipDDL.enabled = val
			
			if ((m_filtermin == 3) or (m_filtermag == 3)) then
			(
				labf04.enabled = true
				maxanisSP.enabled = true
			)	
			else
			(
				labf04.enabled = false
				maxanisSP.enabled = false
			)
	
			if (m_texttype == 1) then
			(
				numframesSB.visible = false
				durationSB.visible = false
				combsepRB.visible = false
				text1d2dRB.visible = true
			)
			else
			if (m_texttype == 2) then
			(
				text1d2dRB.visible = false
				combsepRB.visible = false
				numframesSB.visible = true
				durationSB.visible = true
			)
			else
			if (m_texttype == 3) then
			(
				text1d2dRB.visible = false
				numframesSB.visible = false
				durationSB.visible = false
				combsepRB.visible = true
			)
			
			if ((m_texttype == 1) and (m_text1d2d == 2)) then
			(
				envmapCB.enabled = true
				mapping1DDL.visible = true
				mapping2DDL.visible = false
			)
			else
			if ((m_texttype == 3) and (m_combsep == 1)) then
			(
				envmapCB.enabled = true
				mapping1DDL.visible = false
				mapping2DDL.visible = true
			)
			else
			(
				envmapCB.enabled = false
				mapping1DDL.visible = false
				mapping2DDL.visible = false
			)
		)

		on bitmapparams reload do
		(
			if ( m_bitmapname == "") then
				fileButton.caption = "None"
			else
				fileButton.caption = m_bitmapname
 				
			val = ( m_filtering == 5 )
			labf01.enabled = val
			labf02.enabled = val
			labf03.enabled = val
			filterminDDL.enabled = val
			filtermagDDL.enabled = val
			filtermipDDL.enabled = val
			
			if ((m_filtermin == 3) or (m_filtermag == 3)) then
			(
				labf04.enabled = true
				maxanisSP.enabled = true
			)	
			else
			(
				labf04.enabled = false
				maxanisSP.enabled = false
			)
			
			if (m_texttype == 1) then
			(
				numframesSB.visible = false
				durationSB.visible = false
				combsepRB.visible = false
				text1d2dRB.visible = true
			)
			else
			if (m_texttype == 2) then
			(
				text1d2dRB.visible = false
				combsepRB.visible = false
				numframesSB.visible = true
				durationSB.visible = true
			)
			else
			if (m_texttype == 3) then
			(
				text1d2dRB.visible = false
				numframesSB.visible = false
				durationSB.visible = false
				combsepRB.visible = true
			)
			
			if ((m_texttype == 1) and (m_text1d2d == 2)) then
			(
				envmapCB.enabled = true
				mapping1DDL.visible = true
				mapping2DDL.visible = false
			)
			else
			if ((m_texttype == 3) and (m_combsep == 1)) then
			(
				envmapCB.enabled = true
				mapping1DDL.visible = false
				mapping2DDL.visible = true
			)
			else
			(
				envmapCB.enabled = false
				mapping1DDL.visible = false
				mapping2DDL.visible = false
			)
		)
	)
	
	rollout blendings "Blending Options"
	(
		label labb01 "Color blending type " align:#right across:2
		dropdownlist colouropDDL items:#("replace","add","modulate","alpha_blend","custom") align:#left width:100
		group "Advanced Color Blending"
		(
			label labb02 "Operation" across:3
			label labb03 "Source 1"
			label labb04 "Source 2"
			dropdownlist operDDL items:#("source1","source2","modulate","modulate_x2","modulate_x4","add","add_signed","add_smooth","subtract","blend_diffuse_alpha","blend_texture_alpha","blend_current_alpha","blend_manual") across:3
			dropdownlist src1DDL items:#("src_current","src_texture","src_diffuse","src_specular","src_manual")
			dropdownlist src2DDL items:#("src_current","src_texture","src_diffuse","src_specular","src_manual")
			spinner manfSP type:#float align:#right fieldwidth:60 across:3
			colorpicker mansrc1CP align:#right fieldwidth:60
			colorpicker mansrc2CP align:#right fieldwidth:60
			checkBox fallbackCB "Fallback " across:3
			dropdownlist fallbacksDDL "Source" items:#("one ","zero","DEST.colour","SRC.colour","1-(DEST.colour) ","1-(SRC.colour)","DEST.alpha","SRC.alpha","1-(DEST.alpha)","1-(SRC.alpha)") default:1 align:#left
			dropdownlist fallbackdDDL "Destination" items:#("one ","zero","DEST.colour","SRC.colour","1-(DEST.colour) ","1-(SRC.colour)","DEST.alpha","SRC.alpha","1-(DEST.alpha)","1-(SRC.alpha)") default:1 align:#left
			)
		
		group "Advanced Alpha Blending"
		(
			label labb05 "Operation" across:3
			label labb06 "Source 1"
			label labb07 "Source 2"
			dropdownlist alphaoperDDL items:#("source1","source2","modulate","modulate_x2","modulate_x4","add","add_signed","add_smooth","subtract","blend_diffuse_alpha","blend_texture_alpha","blend_current_alpha","blend_manual") across:3
			dropdownlist alphasrc1DDL items:#("src_current","src_texture","src_diffuse","src_specular","src_manual")
			dropdownlist alphasrc2DDL items:#("src_current","src_texture","src_diffuse","src_specular","src_manual")
			spinner alphamanfSP type:#float across:3
			spinner alphamans1SP type:#float
			spinner alphamans2SP type:#float
		)
	
		on colouropDDL selected num do
		(
			val = (num == 5)
			labb02.enabled = val
			labb03.enabled = val
			labb04.enabled = val			
			operDDL.enabled = val
			src1DDL.enabled = val
			src2DDL.enabled = val
			manfSP.enabled = (val and (m_colouropex_type ==13))
			mansrc1CP.enabled = (val and (m_colouropex_s1 == 5))
			mansrc2CP.enabled = (val and (m_colouropex_s2 == 5))
			fallbackCB.enabled = val
			fallbacksDDL.enabled = (val and m_fallback)
			fallbackdDDL.enabled = (val and m_fallback)
			labb05.enabled = val
			labb06.enabled = val
			labb07.enabled = val
			alphaoperDDL.enabled = val
			alphasrc1DDL.enabled = val
			alphasrc2DDL.enabled = val
			alphamanfSP.enabled = (val and (m_alphaopex_type ==13))
			alphamans1SP.enabled = (val and (m_alphaopex_s1 == 5))
			alphamans2SP.enabled = (val and (m_alphaopex_s2 == 5))
		)
		
		on operDDL selected num do
			manfSP.enabled = (num == 13)

		on src1DDL selected num do
			mansrc1CP.enabled = (num == 5)

		on src2DDL selected num do
			mansrc2CP.enabled = (num == 5)
			
		on fallbackCB changed state do
		(
			fallbacksDDL.enabled = state
			fallbackdDDL.enabled = state
		)
		
		on alphaoperDDL selected num do
			alphamanfSP.enabled = (num == 13)

		on alphasrc1DDL selected num do
			alphamans1SP.enabled = (num == 5)

		on alphasrc2DDL selected num do
			alphamans2SP.enabled = (num == 5)
		
		on blendings open do
		(
			val = (m_colourop == 5)
			labb02.enabled = val
			labb03.enabled = val
			labb04.enabled = val			
			operDDL.enabled = val
			src1DDL.enabled = val
			src2DDL.enabled = val
			manfSP.enabled = (val and (m_colouropex_type ==13))
			mansrc1CP.enabled = (val and (m_colouropex_s1 == 5))
			mansrc2CP.enabled = (val and (m_colouropex_s2 == 5))
			fallbackCB.enabled = val
			fallbacksDDL.enabled = (val and m_fallback)
			fallbackdDDL.enabled = (val and m_fallback)
			labb05.enabled = val
			labb06.enabled = val
			labb07.enabled = val
			alphaoperDDL.enabled = val
			alphasrc1DDL.enabled = val
			alphasrc2DDL.enabled = val
			alphamanfSP.enabled = (val and (m_alphaopex_type ==13))
			alphamans1SP.enabled = (val and (m_alphaopex_s1 == 5))
			alphamans2SP.enabled = (val and (m_alphaopex_s2 == 5))
		)

		on blendings reload do
		(
			val = (m_colourop == 5)
			labb02.enabled = val
			labb03.enabled = val
			labb04.enabled = val			
			operDDL.enabled = val
			src1DDL.enabled = val
			src2DDL.enabled = val
			manfSP.enabled = (val and (m_colouropex_type ==13))
			mansrc1CP.enabled = (val and (m_colouropex_s1 == 5))
			mansrc2CP.enabled = (val and (m_colouropex_s2 == 5))
			fallbackCB.enabled = val
			fallbacksDDL.enabled = (val and m_fallback)
			fallbackdDDL.enabled = (val and m_fallback)
			labb05.enabled = val
			labb06.enabled = val
			labb07.enabled = val
			alphaoperDDL.enabled = val
			alphasrc1DDL.enabled = val
			alphasrc2DDL.enabled = val
			alphamanfSP.enabled = (val and (m_alphaopex_type ==13))
			alphamans1SP.enabled = (val and (m_alphaopex_s1 == 5))
			alphamans2SP.enabled = (val and (m_alphaopex_s2 == 5))
		)
	)
	
	rollout animations "Animation Effects"
	(
		group "Linear Animation" 
		(
			checkBox scrollanimCB "Scroll"
			spinner scrollanimxSP "scroll speed X:" fieldwidth:50 type:#float across:2
			spinner scrollanimySP "scroll speed Y:" fieldwidth:50 type:#float
			checkBox rotateanimCB  "Rotate"
			spinner rotateanimSP  "rotations per second:" fieldwidth:50 type:#float
		)
		group "Wave XForm Animation"
		(
			label laban01 "func" across:6
			label laban02 "waveform"
			label laban03 "base"
			label laban04 "freq"
			label laban05 "phase"
			label laban06 "amp"
		
			dropdownlist xftype1DDL width:48 across:6 items:#("-none-","scroll X","scroll Y","rotate","scale X","scale Y") 
			dropdownlist xfwave1DDL width:50 items:#("sine","triangle","square","sawtooth","inv.sawtooth")
			spinner xfbase1SP type:#float fieldwidth:34 
			spinner xffreq1SP type:#float fieldwidth:34
			spinner xfphase1SP type:#float fieldwidth:34
			spinner xfamp1SP type:#float fieldwidth:34

			dropdownlist xftype2DDL width:48 across:6 items:#("-none-","scroll X","scroll Y","rotate","scale X","scale Y") 
			dropdownlist xfwave2DDL width:50 items:#("sine","triangle","square","sawtooth","inv.sawtooth")
			spinner xfbase2SP type:#float fieldwidth:34
			spinner xffreq2SP type:#float fieldwidth:34
			spinner xfphase2SP type:#float fieldwidth:34
			spinner xfamp2SP type:#float fieldwidth:34

			dropdownlist xftype3DDL width:48 across:6 items:#("-none-","scroll X","scroll Y","rotate","scale X","scale Y") 
			dropdownlist xfwave3DDL width:50 items:#("sine","triangle","square","sawtooth","inv.sawtooth")
			spinner xfbase3SP type:#float fieldwidth:34
			spinner xffreq3SP type:#float fieldwidth:34
			spinner xfphase3SP type:#float fieldwidth:34
			spinner xfamp3SP type:#float fieldwidth:34

			dropdownlist xftype4DDL width:48 across:6 items:#("-none-","scroll X","scroll Y","rotate","scale X","scale Y") 
			dropdownlist xfwave4DDL width:50 items:#("sine","triangle","square","sawtooth","inv.sawtooth")
			spinner xfbase4SP type:#float fieldwidth:34
			spinner xffreq4SP type:#float fieldwidth:34
			spinner xfphase4SP type:#float fieldwidth:34
			spinner xfamp4SP type:#float fieldwidth:34

			dropdownlist xftype5DDL width:48 across:6 items:#("-none-","scroll X","scroll Y","rotate","scale X","scale Y") 
			dropdownlist xfwave5DDL width:50 items:#("sine","triangle","square","sawtooth","inv.sawtooth")
			spinner xfbase5SP type:#float fieldwidth:34
			spinner xffreq5SP type:#float fieldwidth:34
			spinner xfphase5SP type:#float fieldwidth:34
			spinner xfamp5SP type:#float fieldwidth:34
		)
		
		on scrollanimCB changed state do
		(
			scrollanimxSP.enabled = state
			scrollanimySP.enabled = state
		)
		
		on rotateanimCB changed state do
		(
			rotateanimSP.enabled = state
		)
		
		on xftype1DDL selected num do
		(
			val = not (num == 1)
			xfwave1DDL.enabled = val
			xfbase1SP.enabled = val
			xffreq1SP.enabled = val
			xfphase1SP.enabled = val
			xfamp1SP.enabled = val
		)

		on xftype2DDL selected num do
		(
			val = not (num == 1)
			xfwave2DDL.enabled = val
			xfbase2SP.enabled = val
			xffreq2SP.enabled = val
			xfphase2SP.enabled = val
			xfamp2SP.enabled = val
		)
		
		on xftype3DDL selected num do
		(
			val = not (num == 1)			xfwave3DDL.enabled = val
			xfbase3SP.enabled = val
			xffreq3SP.enabled = val
			xfphase3SP.enabled = val
			xfamp3SP.enabled = val
		)
		
		on xftype4DDL selected num do
		(
			val = not (num == 1)
			xfwave4DDL.enabled = val
			xfbase4SP.enabled = val
			xffreq4SP.enabled = val
			xfphase4SP.enabled = val
			xfamp4SP.enabled = val
		)
		
		on xftype5DDL selected num do
		(
			val = not (num == 1)
			xfwave5DDL.enabled = val
			xfbase5SP.enabled = val
			xffreq5SP.enabled = val
			xfphase5SP.enabled = val
			xfamp5SP.enabled = val
		)
		
		on animations open do
		(
			scrollanimxSP.enabled = m_doscroll
			scrollanimySP.enabled = m_doscroll
			rotateanimSP.enabled = m_dorotate
		
			if (m_xformtype[1] == 1) then
			(
				xfwave1DDL.enabled = false
				xfbase1SP.enabled = false
				xffreq1SP.enabled = false
				xfphase1SP.enabled = false
				xfamp1SP.enabled = false
			)
			else
			(
				xfwave1DDL.enabled = true
				xfbase1SP.enabled = true
				xffreq1SP.enabled = true
				xfphase1SP.enabled = true
				xfamp1SP.enabled = true
			)

			if (m_xformtype[2] == 1) then
			(
				xfwave2DDL.enabled = false
				xfbase2SP.enabled = false
				xffreq2SP.enabled = false
				xfphase2SP.enabled = false
				xfamp2SP.enabled = false
			)
			else
			(
				xfwave2DDL.enabled = true
				xfbase2SP.enabled = true
				xffreq2SP.enabled = true
				xfphase2SP.enabled = true
				xfamp2SP.enabled = true
			)

			if (m_xformtype[3] == 1) then
			(
				xfwave3DDL.enabled = false
				xfbase3SP.enabled = false
				xffreq3SP.enabled = false
				xfphase3SP.enabled = false
				xfamp3SP.enabled = false
			)
			else
			(
				xfwave3DDL.enabled = true
				xfbase3SP.enabled = true
				xffreq3SP.enabled = true
				xfphase3SP.enabled = true
				xfamp3SP.enabled = true
			)

			if (m_xformtype[4] == 1) then
			(
				xfwave4DDL.enabled = false
				xfbase4SP.enabled = false
				xffreq4SP.enabled = false
				xfphase4SP.enabled = false
				xfamp4SP.enabled = false
			)
			else
			(
				xfwave4DDL.enabled = true
				xfbase4SP.enabled = true
				xffreq4SP.enabled = true
				xfphase4SP.enabled = true
				xfamp4SP.enabled = true
			)

			if (m_xformtype[5] == 1) then
			(
				xfwave5DDL.enabled = false
				xfbase5SP.enabled = false
				xffreq5SP.enabled = false
				xfphase5SP.enabled = false
				xfamp5SP.enabled = false
			)
			else
			(
				xfwave5DDL.enabled = true
				xfbase5SP.enabled = true
				xffreq5SP.enabled = true
				xfphase5SP.enabled = true
				xfamp5SP.enabled = true
			)
		)

		on animations reload do
		(
			scrollanimxSP.enabled = m_doscroll
			scrollanimySP.enabled = m_doscroll
			rotateanimSP.enabled = m_dorotate
		
			if (m_xformtype[1] == 1) then
			(
				xfwave1DDL.enabled = false
				xfbase1SP.enabled = false
				xffreq1SP.enabled = false
				xfphase1SP.enabled = false
				xfamp1SP.enabled = false
			)
			else
			(
				xfwave1DDL.enabled = true
				xfbase1SP.enabled = true
				xffreq1SP.enabled = true
				xfphase1SP.enabled = true
				xfamp1SP.enabled = true
			)

			if (m_xformtype[2] == 1) then
			(
				xfwave2DDL.enabled = false
				xfbase2SP.enabled = false
				xffreq2SP.enabled = false
				xfphase2SP.enabled = false
				xfamp2SP.enabled = false
			)
			else
			(
				xfwave2DDL.enabled = true
				xfbase2SP.enabled = true
				xffreq2SP.enabled = true
				xfphase2SP.enabled = true
				xfamp2SP.enabled = true
			)

			if (m_xformtype[3] == 1) then
			(
				xfwave3DDL.enabled = false
				xfbase3SP.enabled = false
				xffreq3SP.enabled = false
				xfphase3SP.enabled = false
				xfamp3SP.enabled = false
			)
			else
			(
				xfwave3DDL.enabled = true
				xfbase3SP.enabled = true
				xffreq3SP.enabled = true
				xfphase3SP.enabled = true
				xfamp3SP.enabled = true
			)

			if (m_xformtype[4] == 1) then
			(
				xfwave4DDL.enabled = false
				xfbase4SP.enabled = false
				xffreq4SP.enabled = false
				xfphase4SP.enabled = false
				xfamp4SP.enabled = false
			)
			else
			(
				xfwave4DDL.enabled = true
				xfbase4SP.enabled = true
				xffreq4SP.enabled = true
				xfphase4SP.enabled = true
				xfamp4SP.enabled = true
			)

			if (m_xformtype[5] == 1) then
			(
				xfwave5DDL.enabled = false
				xfbase5SP.enabled = false
				xffreq5SP.enabled = false
				xfphase5SP.enabled = false
				xfamp5SP.enabled = false
			)
			else
			(
				xfwave5DDL.enabled = true
				xfbase5SP.enabled = true
				xffreq5SP.enabled = true
				xfphase5SP.enabled = true
				xfamp5SP.enabled = true
			)
		)
	)
	
	on create do
	(
	)
)