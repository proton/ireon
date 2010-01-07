function uppercase instring = 
(
	local upper, lower, outstring
	upper="ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	lower="abcdefghijklmnopqrstuvwxyz"
	outstring=copy instring
	
	for i=1 to outstring.count do
	(
		j = findString lower outstring[i]
		if (j != undefined) do outstring[i]=upper[j]
	)
	return outstring
)

--------------------------------------------------------------------------
function getBaseAnimName str =
(
	str_ret = str
	str_filename = getFilenameFile str
	str_ext = getFilenameType str
	
	str_array = filterString str_filename "_"
	if (str_array != undefined) then
	(
		al = arrayLength str_array
		if (al > 1) then
		(
			num = str_array[al] as number
			if (num != undefined) then
			(
				pos = findString str_filename ( "_" + str_array[al])

				if ((pos != undefined) and (pos > 1)) then
				(
					str_ret = substring str_filename 1 (pos - 1)
					str_ret = str_ret + str_ext
				)
			)
		)
	)
	return str_ret
)

--------------------------------------------------------------------------
function getBaseCubicName str =
(
	str_ret = str
	str_filename = getFilenameFile str
	str_ext = getFilenameType str
	
	str_array = filterString str_filename "_"
	if (str_array != undefined) then
	(
		al = arrayLength str_array
		if (al > 1) then
		(
			str_al = uppercase str_array[al]
			if ((str_al=="FR") or (str_al=="BK") or (str_al=="UP") or (str_al=="DN") or (str_al=="LF") or (str_al=="RT")) then
			(
				pos = findString str_filename ( "_" + str_array[al])

				if ((pos != undefined) and (pos > 1)) then
				(
					str_ret = substring str_filename 1 (pos - 1)
					str_ret = str_ret + str_ext
				)
			)
		)
	)
	return str_ret
)

--------------------------------------------------------------------------
function writeOGRETextureUnit texture outFile =
(
	local str_textype = #("1d","2d")
	local str_cubtextype = #("combinedUVW","separateUV")
	local str_filtermin = #("point","linear","anisotropic")
	local str_filtermip = #("none","point","linear")
	local str_envmap2D =#("spherical","planar")
	local str_envmapcubic =#("cubic_reflection","cubic_normal")
	local str_xftype =#("scroll_x","scroll_y","rotate","scale_x","scale_y")
	local str_xwtype =#("sine","triangle","square","sawtooth","inverse_sawtooth")
	local str_colourop =#("replace","add","modulate","alpha_blend")
	local str_cotype =#("source1","source2","modulate","modulate_x2","modulate_x4","add","add_signed","add_smooth","subtract","blend_diffuse_alpha","blend_texture_alpha","blend_current_alpha","blend_manual")
	local str_cos =#("src_current","src_texture","src_diffuse","src_specular","src_manual")
	local str_fallback = #("one","zero","dest_colour","src_colour","one_minus_dest_colour","one_minus_src_colour","dest_alpha","src_alpha","one_minus_dest_alpha","one_minus_src_alpha")

	if (texture.m_bitmapname == undefined) then
	(
		MessageBox ("Texture \"" + texture.name + "\" has no bitmap file assigned!\nThis texture unit will not be exported.")
		return()
	)

	tnames = filterstring texture.m_bitmapname "\\" ;
	
	if (tnames == undefined) then
	(
		MessageBox ("Texture \"" + texture.name + "\" has no bitmap file assigned!\nThis texture unit will not be exported.")
		return()
	)
	
	if (arrayLength tnames == 0) then
	(
		MessageBox ("Texture \"" + texture.name + "\" has no bitmap file assigned!\nThis texture unit will not be exported.")
		return()
	)
	else
		tname = tnames [arrayLength tnames]

	format("\n\t\t\ttexture_unit\n") to:outFile
	format("\t\t\t{\n") to:outFile
	
	if(texture.m_texttype == 1)	then -- texture
	(
		format ("\t\t\t\ttexture % %\n") tname str_textype[texture.m_text1d2d] to:outFile ;
	)
	else
	if(texture.m_texttype == 2)	then -- anim_texture
	(
		tname = getBaseAnimName tname
		format ("\t\t\t\tanim_texture % % %\n") tname texture.m_nframes texture.m_duration to:outFile ;	
	)
	else
	if(texture.m_texttype == 3)	then -- cubic_texture
	(
		tname = getBaseCubicName tname
		format ("\t\t\t\tcubic_texture % %\n") tname str_cubtextype[texture.m_combsep] to:outFile ;
	)
	
	-- format ("\t\t\t\ttex_coord_set % \n") texture.m_coordset to:outFile ;
	
	if (texture.m_mirror) then
		format ("\t\t\t\ttex_address_mode mirror\n") to:outFile;
	else
	if (texture.m_tile) then
		format ("\t\t\t\ttex_address_mode wrap\n") to:outFile;
	else
		format ("\t\t\t\ttex_address_mode clamp\n") to:outFile;
		
	format ("\t\t\t\tfiltering % % %\n") str_filtermin[texture.m_filtermin] str_filtermin[texture.m_filtermag] str_filtermip[texture.m_filtermip] to:outFile;
	if ((texture.m_filtermin == 3) or (texture.m_filtermag == 3)) then
		format ("\t\t\t\tmax_anisotropy %\n") texture.m_maxanisotropy to:outFile;

	if (texture.m_colourop == 5) then
	(
		format ("\t\t\t\tcolour_op_ex % % %") str_cotype[texture.m_colouropex_type] str_cos[texture.m_colouropex_s1] str_cos[texture.m_colouropex_s2] to:outFile;

		if (texture.m_colouropex_type == 13) then
			format " %" texture.m_colouropex_mf to:outFile
		if (texture.m_colouropex_s1 == 5) then
			format " % % %" (texture.m_colouropex_m1.r/255.0) (texture.m_colouropex_m1.g/255.0) (texture.m_colouropex_m1.b/255.0) to:outFile
		if (texture.m_colouropex_s2 == 5) then
			format " % % %" (texture.m_colouropex_m2.r/255.0) (texture.m_colouropex_m2.g/255.0) (texture.m_colouropex_m2.b/255.0) to:outFile
		format "\n" to:outFile

		if (texture.m_fallback) then
			format ("\t\t\t\tcolour_op_multipass_fallback % %\n") str_fallback[texture.m_fallback_s] str_fallback[texture.m_fallback_d] to:outFile;
		
		format ("\t\t\t\talpha_op_ex % % %") str_cotype[texture.m_alphaopex_type] str_cos[texture.m_alphaopex_s1] str_cos[texture.m_alphaopex_s2] to:outFile;
		if (texture.m_alphaopex_type == 13) then
			format " %" texture.m_alphaopex_mf to:outFile
		if (texture.m_alphaopex_s1 == 5) then
			format " %" texture.m_alphaopex_m1 to:outFile
		if (texture.m_alphaopex_s2 == 5) then
			format " %" texture.m_alphaopex_m2 to:outFile
		format "\n" to:outFile
	)
	else
	(
		format ("\t\t\t\tcolour_op %\n") str_colourop[texture.m_colourop] to:outFile;
	)
	
	if (texture.m_envmap) then
	(
		if (texture.m_texttype == 1) then
		(
			format ("\t\t\t\tenv_map %\n") str_envmap2D[texture.m_mapping2D] to:outFile;
		)
		else
		if (texture.m_texttype == 3) then
		(
			format ("\t\t\t\tenv_map %\n") str_envmapcubic[texture.m_mappingCubic] to:outFile;
		)
	)
	
	format ("\t\t\t\tscroll % %\n") texture.m_offsetx texture.m_offsety to:outFile;
	format ("\t\t\t\tscale % %\n") (1.0/texture.m_tilingx) (1.0/texture.m_tilingy) to:outFile;
	format ("\t\t\t\trotate %\n") texture.m_angle to:outFile;
	
	
	if (texture.m_dorotate) then
		format ("\t\t\t\trotate_anim %\n") texture.m_rotateanim to:outFile;
	if (texture.m_doscroll) then
		format ("\t\t\t\tscroll_anim % %\n") texture.m_scrollanimx texture.m_scrollanimy to:outFile;
		
	for i = 1 to 5 do
	(
		if (texture.m_xformtype[i] > 1) then
		(
			format ("\t\t\t\twave_xform % % % % % %\n") str_xftype[texture.m_xformtype[i]-1] str_xwtype[texture.m_xwavetype[i]] texture.m_xbase[i] texture.m_xfreq[i] texture.m_xphase[i] texture.m_xamp[i] to:outFile;
		)
	)
	
	format("\t\t\t}\n") to:outFile
)

--------------------------------------------------------------------------
function writeBitmapTextureUnit texture outFile =
(
	if (texture.filename == undefined) then
	(
		MessageBox ("Texture \"" + texture.name + "\" has no bitmap file assigned!\nThis texture unit will not be exported.")
		return()
	)

	tnames = filterstring texture.filename "\\";
	
	if (tnames == undefined) then
	(
		MessageBox ("Texture \"" + texture.name + "\" has no bitmap file assigned!\nThis texture unit will not be exported.")
		return()
	)
	
	if (arrayLength tnames == 0) then
	(
		MessageBox ("Texture \"" + texture.name + "\" has no bitmap file assigned!\nThis texture unit will not be exported.")
		return()
	)
	else
		tname = tnames [arrayLength tnames]

	format("\n\t\t\ttexture_unit\n") to:outFile
	format("\t\t\t{\n") to:outFile
	
	format ("\t\t\t\ttexture %\n") tname to:outFile ;

	-- format ("\t\t\t\ttex_coord_set % \n") texture.mapChannel to:outFile ;
	
	if ((texture.coordinates.U_Mirror) and (texture.coordinates.V_Mirror)) then
		format ("\t\t\t\ttex_address_mode mirror\n") to:outFile;
	else
	if ((texture.coordinates.U_Tile) and (texture.coordinates.V_Tile)) then
		format ("\t\t\t\ttex_address_mode wrap\n") to:outFile;
	else
		format ("\t\t\t\ttex_address_mode clamp\n") to:outFile;
	
	if (texture.coordinates.mappingType == 1) then
	(
		if (texture.coordinates.mapping == 0) then
			format ("\t\t\t\tenv_map spherical\n") to:outFile;
		else
		if (texture.coordinates.mapping == 3) then
			format ("\t\t\t\tenv_map planar\n") to:outFile;
	)
	
	format ("\t\t\t\tscroll % %\n") texture.coordinates.U_Offset texture.coordinates.V_Offset to:outFile;
	if ((texture.coordinates.U_Tiling != 0) and (texture.coordinates.V_Tiling != 0)) then
		format ("\t\t\t\tscale % %\n") (1.0/texture.coordinates.U_Tiling) (1.0/texture.coordinates.V_Tiling) to:outFile;
	format ("\t\t\t\trotate %\n") texture.coordinates.W_Angle to:outFile;
	
	format("\t\t\t}\n") to:outFile
)

--------------------------------------------------------------------------
function writeOGRESinglePass material outFile =
(
	local str_blends = #("one","zero","dest_colour","src_colour",\
						"one_minus_dest_colour","one_minus_src_colour",\
						"dest_alpha","src_alpha","one_minus_dest_alpha",\
						"one_minus_src_alpha")
	
	local str_dfuncs = #("always_fail","always_pass","less","less_equal","equal","not_equal","greater_equal","greater")
	local str_cullh = #("clockwise","anitclockwise","none")
	local str_culls = #("back","front","none")
	local str_shading = #("flat","gouraud","phong")

	format "\t\tpass\n" to:outFile ;
	format "\t\t{\n" to:outFile ;
	
	format("\t\t\tambient % % %\n") (material.m_ambcolor.r/255.0) (material.m_ambcolor.g/255.0) (material.m_ambcolor.b/255.0) to:outFile;
	format("\t\t\tdiffuse % % %\n") (material.m_diffcolor.r/255.0) (material.m_diffcolor.g/255.0) (material.m_diffcolor.b/255.0) to:outFile;
	format("\t\t\tspecular % % % %\n") ((material.m_speccolor.r/255.0)*((material.m_speclevel as float)/100.0)) ((material.m_speccolor.g/255.0)*((material.m_speclevel as float)/100.0)) ((material.m_speccolor.b/255.0)*((material.m_speclevel as float)/100.0)) material.m_specgloss to:outFile;
	format("\t\t\temissive % % %\n") (material.delegate.self_Illum_color.r/255.0) (material.delegate.self_Illum_color.g/255.0) (material.delegate.self_Illum_color.b/255.0) to:outFile;

	format("\t\t\tscene_blend % %\n") str_blends[material.m_sceneblend_src] str_blends[material.m_sceneblend_dest] to:outFile;
	
	if (material.m_depthcheck) then
	(
		format("\t\t\tdepth_check on\n") to:outFile;
		format("\t\t\tdepth_func %\n") str_dfuncs[material.m_depthfunc] to:outFile;
	)
	else
		format("\t\t\tdepth_check off\n") to:outFile;

	if (material.m_depthwrite) then
		format("\t\t\tdepth_write on\n") to:outFile;
	else
		format("\t\t\tdepth_write off\n") to:outFile;

	format("\t\t\tdepth_bias %\n") material.m_depthbias to:outFile;

	format("\t\t\tcull_software %\n") str_culls[material.m_cullsoftware] to:outFile;

	if (material.m_lighting) then
		format("\t\t\tlighting on\n") to:outFile;
	else
		format("\t\t\tlighting off\n") to:outFile;

	if (material.m_fogoverride) then
		format("\t\t\tfog_override false\n") to:outFile;
	else
		format("\t\t\tfog_override true\n") to:outFile;
		
	format("\t\t\tshading %\n") str_shading[material.m_shadertype] to:outFile;

	if (material.m_colourwrite) then
		format("\t\t\tcolour_write on\n") to:outFile;
	else
		format("\t\t\tcolour_write off\n") to:outFile;
		
	for i = 1 to 4 do
	(
		tex = material.TextureUnits[i]
		if (tex != undefined) then
		(
			if (classof tex == OctopusTextureUnit) then
			(
				writeOGRETextureUnit tex outFile
			)
			else
			if (classof tex == BitmapTexture) then
			(
				writeBitmapTextureUnit tex outFile
			)
		)
	)

	format "\t\t}\n" to:outFile ;
)

--------------------------------------------------------------------------
function writeOGRESingleMaterial material prefix outFile =
(
	local t ;
	matname = replaceSpaces material.name ;
	-- writes header	
	format "material %\n" (prefix + matname) to:outFile ;
	format "{\n" to:outFile ;
	format "\ttechnique\n" to:outFile ;
	format "\t{\n" to:outFile ;
	
	writeOGRESinglePass material outFile
	
	format "\t}\n" to:outFile ;
	format "}\n" to:outFile ;
)

--------------------------------------------------------------------------
function WriteMultiMaterial material prefix outFile=
(
	local sub,i,matname ;
	matname = replaceSpaces material.name ;
	prefix = prefix + matname + "/"
	
	for sub in material.materiallist do
	(
		if (classof sub == StandardMaterial) then
			writeStandardMaterial sub prefix outFile ;
		else
		if (classof sub == OctopusPassMaterial) then
			writeOGRESingleMaterial sub prefix outFile ;

		format "\n" to:outfile ;
	)
)

--------------------------------------------------------------------------
function previewOctopusMaterial material =
(
	local outFile;
	local matname = replaceSpaces material.name;

	ViewerConvPath = getINISetting ((getDir #scripts) + "\\octopus\\OctopusScript.ini") "Directories" "OgreViewerExePath"
	ViewerMedia= getINISetting ((getDir #scripts) + "\\octopus\\OctopusScript.ini") "Directories" "OgreViewerMediaPath"
	ViewerExe= getINISetting ((getDir #scripts) + "\\octopus\\OctopusScript.ini") "Exe" "OgreViewerExe"
	
	outFile = createfile ( ViewerMedia + "\\" + "preview.material" )
	
	if (outFile == undefined) then
	(
		messageBox ("Error: cannot create file \"" + ViewerMedia + "\\" + "preview.material" + "\"\nPreview operation aborted")
	)
	else
	(
		writeOGRESingleMaterial material "preview/" outFile

		close outFile;

		matname = "preview/" + matname;

		olddir = sysInfo.currentdir
		sysInfo.currentdir  =  ViewerConvPath
		ShellLaunch ("\"" + ViewerConvPath + "\\" + ViewerExe + "\"") ("\"" + matname + "\"" ) ;
		sysInfo.currentdir  =  olddir
	)
)

--------------------------------------------------------------------------
function exportSomeMaterial material prefix outfile =
(
	if (classof material == MultiMaterial) then
		WriteMultiMaterial material prefix outFile ;
	else 
	if (classof material == StandardMaterial) then
		exportStandardMaterial material outFile ;
	else
	if (classof material == OctopusPassMaterial) then
		writeOGRESingleMaterial material prefix outFile ;

	format "\n" to:outfile ;
)