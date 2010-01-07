global dotsceneID
global linkID
-----------------------------------------------------------
-----------------------------------------------------------
-----------------------------------------------------------
function IsValidSceneMesh obj =
(
	if ((Superclassof obj == GeometryClass) and (obj.isTarget == false)) then
		return true
	else
		return false
)
-----------------------------------------------------------
-----------------------------------------------------------
-----------------------------------------------------------
function IsValidSceneLight obj =
(
	if (Superclassof obj == Light) then
	(
		case (classOf obj) of
		(
--			TargetSpot: 			return true
--			TargetDirectionalLight: return true
			Omnilight: 				return true
			DirectionalLight: 		return true
			FreeSpot: 				return true
			default: 				return false
 		)
	)
	else
		return false
)
-----------------------------------------------------------
-----------------------------------------------------------
-----------------------------------------------------------
function IsPartOfYappModel obj =
(
	if (obj.parent == YappModel) then
		return true
	else
		return false
)
-----------------------------------------------------------
-----------------------------------------------------------
-----------------------------------------------------------
function IsValidRootSceneNode obj =
(
	messageBox(obj.parent.name)
	if (obj.parent == undefined) then
	(
		messageBox("un");
		return true
	)
	else
	if (obj.parent == YappModel) then
	(
		messageBox("ym");
		return true
	)
	else
		return false
)
-----------------------------------------------------------
-----------------------------------------------------------
-----------------------------------------------------------
function IsValidSceneNode obj =
(
	for mod in obj.modifiers do
	(
		if (mod.name == "ODE Body") do 
			return false
	)
	if (IsValidSceneMesh obj) then
		return true
	else
	if (classOf obj == Dummy) then
		return true
	else
	if (classOf obj.parent == YappModel) then
		return true
	else
	if (IsValidSceneLight obj) then
		return true	
	else
		return false
)
-----------------------------------------------------------
-----------------------------------------------------------
-----------------------------------------------------------
function WriteSceneFileBegin outfile scenename =
(
	format "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" to:outfile
    format "<!-- \n" to:outfile
    format "\t '%' Scene file for ireon.org client.\n" scenename to:outfile
	format "--> \n" to:outfile
	
)
-----------------------------------------------------------
-----------------------------------------------------------
-----------------------------------------------------------
function WriteSceneNodesBegin outfile =
(
	dotsceneID = 0
	format "<cfg:scene formatVersion=\"0.1\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n" to:outfile
    format "\txmlns:cfg=\"http://www.ireon.org/ireon/client/\"\n" to:outfile
    format "\txsi:schemaLocation=\"http://www.ireon.org/ireon/client/ zone.xsd\">\n" to:outfile
	format "\t<nodes>\n" to:outfile
	
)
-----------------------------------------------------------
-----------------------------------------------------------
-----------------------------------------------------------
function WriteSceneNodesEnd outfile =
(
	format "\t</nodes>\n" to:outfile
	format "</cfg:scene>" to:outfile
)
-----------------------------------------------------------
-----------------------------------------------------------
-----------------------------------------------------------
function WriteSceneFileEnd outfile =
(
	-- format "</cfg:scene>" to:outfile
	
)
-----------------------------------------------------------
-----------------------------------------------------------
-----------------------------------------------------------
function getMeshNameForGeometryObject origs obj =
(
	format "Looking for mesh name for % in % originals...\n" obj.name origs.count
	for orig in origs do
	(
		local pos = findItem orig obj
		format "  trying %: % \n" orig[1].name pos
		if pos == 1 then
		(
			local ret = obj.name
			format "  using ORIG % \n" ret
			return ret
		)
		else
		if pos > 1 then
		(
			local ret = orig[1].name
			format "  using CLONE % \n" ret
			return ret
		)
	)
	format "  using DEFAULT %\n" obj.name
	return obj.name
)
-----------------------------------------------------------
-----------------------------------------------------------
-----------------------------------------------------------
function WriteSceneEntity origs obj tabstring scenename outfile =
(
	bStatic = true ;
	bCastsShadow = false ;
	bExport = true ;

	for mod in obj.modifiers do
	(
		if (mod.name == "OctopusExport") then
		(
			bCastsShadow = mod.castsShadow;
			bExport = mod.exportThis;
				
			--TODO static
		)
	)
	if (bExport) then
	(
		local meshFile = getMeshNameForGeometryObject origs obj

		dotsceneID = dotsceneID + 1
		format "%<entity/>\n" tabstring to:outfile 
		-- format "%\tname=\"%\"\n" tabstring obj.name to:outfile
		-- format "%\tid=\"%\"\n" tabstring dotsceneID to:outfile
		-- format "%\tmeshFile=\"%.mesh\"\n" tabstring meshFile to:outfile
		-- format "%\tmaterialFile=\"%.material\"\n" tabstring scenename to:outfile
		--- if (bCastsShadow) then
		---	  format "%\tcastsShadow=\"true\"\n" tabstring to:outfile
		--- else
		---	  format "%\tcastsShadow=\"false\"\n" tabstring to:outfile
		--- format "%/>\n" tabstring to:outfile
	)
)
-----------------------------------------------------------
-----------------------------------------------------------
-----------------------------------------------------------
function WriteSceneLight obj tabstring unitsScale outfile =
(
	dotsceneID = dotsceneID + 1
	format "%<light\n" tabstring to:outfile
	format "%\tname=\"%\"\n" tabstring obj.name to:outfile
	format "%\tid=\"%\"\n" tabstring dotsceneID to:outfile

	bHasNormal = false;
	bHasRange = false;
	bHasAttenuation = false;
	
	if (classOf obj == Omnilight) then
	(
		format "%\ttype=\"point\"\n" tabstring to:outfile
		bHasAttenuation = true
	)
	else
	if ((classOf obj == TargetDirectionalLight) or (classOf obj == DirectionalLight)) then
	(
		format "%\ttype=\"directional\"\n" tabstring to:outfile
		bHasNormal = true
	)
	else
	if ((classOf obj == TargetSpot) or (classOf obj == FreeSpot)) then
	(
		format "%\ttype=\"spot\"\n" tabstring to:outfile
		bHasNormal = true
		bHasRange = true
		bHasAttenuation = true
	)

	if (obj.enabled) then
		format "%\tvisible=\"true\" " tabstring to:outfile
	else
		format "%\tvisible=\"false\" " tabstring to:outfile
		
	if (obj.baseObject.castShadows) then
		format "%\tcastShadows=\"true\" " tabstring to:outfile
	else
		format "%\tcastShadows=\"false\" " tabstring to:outfile
	
	format ">\n" to:outfile

	format "%\t<position x=\"0.0\" y=\"0.0\" z=\"0.0\" />\n" tabstring to:outfile

	if (bHasNormal) do
	(
		format "%\t<normal x=\"0.0\" y=\"0.0\" z=\"-1.0\" />\n" tabstring to:outfile
	)

	if (obj.affectDiffuse) do
	(
		format "%\t<colourDiffuse r=\"%\" g=\"%\" b=\"%\" />\n" tabstring ((obj.rgb.r/255.0)*obj.multiplier) ((obj.rgb.g/255.0)*obj.multiplier) ((obj.rgb.b/255.0)*obj.multiplier) to:outfile
	)
	
	if (obj.affectSpecular) do
	(
		format "%\t<colourSpecular r=\"%\" g=\"%\" b=\"%\" />\n" tabstring ((obj.rgb.r/255.0)*obj.multiplier) ((obj.rgb.g/255.0)*obj.multiplier) ((obj.rgb.b/255.0)*obj.multiplier) to:outfile
	)
	
	if (bHasRange) do
	(
		format "%\t<lightRange\n" tabstring to:outfile
		format "%\t\tinner=\"%\"\n" tabstring (degToRad(obj.hotspot)) to:outfile
		format "%\t\touter=\"%\"\n" tabstring (degToRad(obj.falloff)) to:outfile
		format "%\t\tfalloff=\"1.0\"\n" tabstring to:outfile
		format "%\t/>\n" tabstring to:outfile
	)

	if (bHasAttenuation ) do
	(
		format "%\t<lightAttenuation\n" tabstring to:outfile
		format "%\t\trange=\"%\"\n" tabstring (obj.farAttenEnd/unitsScale) to:outfile
		atten0 = 0.0
		atten1 = 0.0
		atten2 = 0.0
		case obj.attenDecay of
		(
			1: ( atten0 = 1.0; atten1 = 0.0; atten2 = 0.0 )
			2: ( atten0 = 0.0; atten1 = 1.0; atten2 = 0.0 )
			3: ( atten0 = 0.0; atten1 = 0.0; atten2 = 1.0 )
		)
		format "%\t\tconstant=\"%\"\n" tabstring atten0 to:outfile
		format "%\t\tlinear=\"%\"\n" tabstring atten1 to:outfile
		format "%\t\tquadratic=\"%\"\n" tabstring atten2 to:outfile
		format "%\t/>\n" tabstring to:outfile
	)
	
	format "%</light>\n" tabstring to:outfile
)
-----------------------------------------------------------
-----------------------------------------------------------
-----------------------------------------------------------
function transposeObject lpos outfile =
(
	-- get coord rectangle 'ireon_zone'
	zone = getnodebyname "ireon_zone" exact:false ignoreCase:true all:false
	-- format "zone: %\n" zone to:outfile

	-- compute coord center rectangle
	centerZone = copy zone.pos;
	-- format "center: \n%" centerZone to:outfile


	-- transpose object on center rectangle
	-- format "lpos: %\n" newPos to:outfile
	newPos = copy lpos
	newPos[1] = newPos[1] - centerZone[1];
	newPos[2] = newPos[2] - centerZone[2];
	newPos[3] = newPos[3] - centerZone[3];
	-- format "newPos: %\n" newPos to:outfile

	return newPos

	
)

function WriteSceneNode origs obj level scenename unitsScale flipyz outfile =
(
    if (IsValidSceneLight obj) then
        return false  
	if (IsValidSceneNode obj) do
	(
		lpos = in coordsys parent obj.pos
		lpos =  lpos / unitsScale
		lquat = in coordsys parent obj.rotation
		lscale = in coordsys parent obj.scale
		
		lpos = transposeObject lpos outfile 
		
		if (flipyz) then
		(
			lposTmp = copy lpos ;
			lpos[2] = lposTmp[3] ;
			lpos[3] = -lposTmp[2] ;
			
			lquatTmp = copy lquat ;
			lquat.y = lquatTmp.z;
			lquat.z = -lquatTmp.y;
		)
		if ((obj.parent != undefined) and 
			(IsValidSceneNode obj.parent) and
			(classOf obj.parent != YappModel)) then
		(
		-- TODO: Does this work??
			format "SCENENODE: using parent scale\n"
			lpos = lpos * obj.parent.scale;
		)
		
		--format "INDENT\n"
		
		tabstring = ""
		for i = 0 to level do
		(
			tabstring = tabstring + "\t"
		)

		dotsceneID = dotsceneID + 1
	-- TODO: BUG HERE ??????????????????
	-- psyclonist / 09 Mar 2005:
	--   * fixed flipYZ problems above.
	--   * BUG still here?
		--- format "%<node name=\"%\" id=\"%\" >\n" tabstring obj.name dotsceneID to:outfile
		format "%<node name=\"%\" >\n" tabstring obj.name to:outfile
		format "%\t<position x=\"%\" y=\"%\" z=\"%\" />\n" tabstring lpos.x lpos.y lpos.z to:outfile
		format "%\t<rotation qx=\"%\" qy=\"%\" qz=\"%\" qw=\"%\" />\n" tabstring lquat.x lquat.y lquat.z lquat.w to:outfile
		format "%\t<scale x=\"%\" y=\"%\" z=\"%\" />\n" tabstring lscale.x lscale.y lscale.z to:outfile
	-- TODO: BUG HERE ??????????????????

		
		level = level + 1
		for chi in obj.children do
		(
			if (chi.parent == obj) do
			(
				WriteSceneNode chi level scenename unitsScale flipyz outfile
			)
		)
		
		if (IsValidSceneMesh obj) do
		(
			WriteSceneEntity origs obj (tabstring + "\t") scenename outfile
		)

		if (IsValidSceneLight obj) do
		(
			-- WriteSceneLight obj (tabstring + "\t") unitsScale outfile
		)
		
		format "%</node>\n" tabstring to:outfile
	)
)

function WriteLinksBegin outfile =
(
	format "<links>\n" to:outfile
	linkID = 1
)

function WriteLinksEnd outfile =
(
	format "</links>\n" to:outfile
)

function WriteLink obj outfile =
(
	if ((obj.m_node1 != undefined) and (obj.m_node2 != undefined)) do
	(
		format "\t<link id=\"%\" type=\"yake.movable\">\n" linkID to:outfile
		format "\t\t<link_source submodel=\"basePhysical\" type=\"physical\" elementtype=\"physics.actor\" element=\"%\">\n" obj.m_node1.name to:outfile
		format "\t\t\t<link_options />\n" to:outfile
		format "\t\t</link_source>\n" to:outfile
		format "\t\t<link_target submodel=\"baseGraphical\" type=\"graphical\" elementtype=\"graphics.scenenode\" element=\"%\">\n" obj.m_node2.name to:outfile
		format "\t\t\t<link_options />\n" to:outfile
		format "\t\t</link_target>\n" to:outfile
		format "\t</link>\n" to:outfile
		
		linkID = linkID + 1
	)
)

function WriteModel scenename outfile =
(

)