-----------------------------------------------------------
function IsValidOctopusObject obj =
(
	if (((Superclassof obj == GeometryClass) and (obj.isTarget == false)) or (Superclassof obj == light) or (Superclassof obj == camera)) then
		return true
	else
		return false
)
-----------------------------------------------------------
function WriteCommonScript obj scriptfile =
(
	lpos = in coordsys parent obj.pos
	lquat = in coordsys parent obj.rotation
	format "\tname = \"%\",\n" obj.name to:scriptfile
	if (obj.parent != undefined) then
	(
		format "\tparent = \"%\",\n" obj.parent.name to:scriptfile
		format "\tposition = { %, %, % },\n" (lpos.x * obj.parent.scale.x) (lpos.y * obj.parent.scale.y) (lpos.z * obj.parent.scale.z) to:scriptfile
	)
	else
	(
		format "\tposition = { %, %, % },\n" lpos.x lpos.y lpos.z to:scriptfile
	)
	format "\torientation = { %, %, %, % }\n" lquat.w lquat.x lquat.y lquat.z to:scriptfile
)
-----------------------------------------------------------
function WriteTemplateScript obj prefix scriptfile =
(
	-- TODO: check for referenced objects
	templatename = prefix + obj.name
	meshname = obj.name

	format "\tname = \"%\",\n\tentity = \"%\"\n" templatename meshname to:scriptfile
)
-----------------------------------------------------------
function WriteObjectScript obj prefix scriptfile =
(
	templatename = prefix + obj.name
	-- TODO: check for unique mesh name and name creation settings
	meshname = obj.name

	format "\ttype = \"object\",\n" to:scriptfile	
	format "\ttemplate = \"%\",\n" templatename to:scriptfile
)
-----------------------------------------------------------
function WriteLightScript obj scriptfile =
(
	format "\ttype = \"light\",\n" to:scriptfile	
)
-----------------------------------------------------------
function WriteCameraScript obj scriptfile =
(
	format "\ttype = \"camera\",\n" to:scriptfile	
)
-----------------------------------------------------------
-----------------------------------------------------------
-----------------------------------------------------------
function ExportOctopusObject obj prefix ScriptO ScriptS =
(
	if (IsValidOctopusObject obj) then
	(
		format "create\n{\n" to:ScriptS
	
		if(Superclassof obj == GeometryClass) then
		( -- Geometry class, i.e. mesh
			-- Export Mesh Parameters:
			WriteObjectScript obj prefix ScriptS

			-- Export Templates:
			format "defineObjectTemplate\n{\n" to:ScriptO
			WriteTemplateScript obj prefix ScriptO
			format "}\n\n" to:ScriptO
		)
		else
		if (Superclassof obj == camera) then
		(
			-- Export Camera Parameters:
			WriteCameraScript obj ScriptS
		)
		else
		if (Superclassof obj == light) then
		(
			-- Export Light Parameters:
			WriteLightScript obj ScriptS
		)
		
		WriteCommonScript obj ScriptS
		
		format "}\n\n" to:ScriptS
	)
)
-----------------------------------------------------------
function ExportAliasScript scenename scriptfile =
(
	format "defineScript\n{\n" to:scriptfile 
	format "\tname = \"%\",\n" scenename to:scriptfile 
	format "\tfile = \"%.lua\"\n" scenename to:scriptfile 
	format "}\n" to:scriptfile
)
