include "octopus/lib/ogre/ogreSkeletonLib.ms"
include "octopus/lib/OctopusOgreOverrides.ms"
include "octopus/lib/OctopusFileRoutines.ms"
include "octopus/lib/OctopusSceneExport.ms"
include "octopus/lib/OctopusMatExport.ms"
include "octopus/lib/OctopusODEExport.ms"

macroScript showOctopusSceneExport
	category:"Octopus Extentions"
	internalCategory:"Octopus Extentions"
	buttonText:"Export"
	tooltip:"Export scene to Octopus"
	Icon:#("OgreTools",3)
(
	OctopusExportFloater = newRolloutFloater "OctopusExportTool" 340 700 ; 
	
	rollout optionRollout "Export Plug-in Options"
	(
		label lab04 "rel.date:1.11.04" align:#left across:2
		button openButton "Open OctopusScript.ini" align:#right
		label lab041 "Units: " aligh:#left across:3
		dropdownlist unitsDDL items:#("Meters","Millimeters","Other") align:#left
		spinner unitsSP range:[0,10000,1000] width:100 type:#Float scale:0.1 enabled:false align:#right;

		on openButton pressed do
		(
			shellLaunch ((getDir #scripts) + "\\octopus\\octopusScript.ini") "" ;
		)
		
		on unitsDDL selected sel do
		(
			case sel of
			(
				1: (
				unitsSP.value = 1.0
				unitsSP.enabled = false
				)
				2: (
				unitsSP.value = 1000.0
				unitsSP.enabled = false
				)
				3:
				(
				unitsSP.enabled = true
				)
			)
		)
		
		on unitsSP changed val do
		(
			if (val == 1.0) then
			(
				unitsDDL.selection = 1
			)
			else
			if (val == 1000.0) then
			(
				unitsDDL.selection = 2
			)
			else
			if (unitsDDL.selection != 3) then
			(
				unitsDDL.selection = 3
			)
		)
		
		on optionRollout open do
		(
			unitsDDL.selection = 2
		)
	)

	rollout DefaultSettingsRollout "Default Mesh Settings"
	(
		label lab1 "Note: This settings will be applied to any\nobject in scene that does not have\n\"OctopusExport\" modifier." align:#left height:60
		
		spinner scaleSP "Scale:" range:[0,100,1] width:120 type:#Float scale:0.1 align:#right;
		checkBox flipYZCB "Flip Y/Z" align:#right ;
		
		checkBox flipNormalCB "Flip normals" align:#left ;
		checkBox ExportUVCB "Export texture coordinates" align:#left ;
		checkBox ExportColCB "Export colour information " align:#left ;
		
		on DefaultSettingsRollout open do
		(
			ExportUVCB.checked = true ;
			flipYZCB.checked = false ;
		)
	)
	
	rollout OctopusExportRollout "Export Scene"
	(
		group "General Options"
		(
			editText scenenameET "Scene name:" align:#left
			label lab02 "Objects to export:" align:#left
			radiobuttons selectRB labels:#("Entire scene","Current selection") align:#left offset:[40,0]
			button selectButton "Change selection..." align:#right
		)
		group "Meshes and Skeletons"
		(
			checkbox expmeshesCB "Export meshes (.mesh)" align:#left across:2
			checkbox expskelsCB "Export skeletons (.skeleton)" align:#left
			editText mfoldernameET "Meshes folder:" align:#left ;
			button chooseMDirButton "Browse..." align:#right width:58 height:24 ;
			checkbox runconverterCB "Run XMLconverter" align:#right width: 180;
			checkbox deletexmlCB "Delete XML files after conversion" align:#right width: 180;
		)
		group "Materials"
		(
			checkbox expmaterialsCB "Export Materials (.material)" align:#left across:2
			button chooseMtDirButton "Browse..." align:#right width:58 height:24 ;
			editText mtfoldernameET "Materials folder:" align:#left ;
		)
		group "Physics"
		(
			checkbox exportxodeCB "Export Physics (.xode)" align:#left across:2
			button chooseXDirButton "Browse..." align:#right width:58 height:24 ;
			editText xfoldernameET "Physics folder:" align:#left ;
		)
		group "Visual Scene"
		(
			checkbox makesceneCB "Export Scene (.scene)" align:#left across:2
			button chooseSDirButton "Browse..." align:#right width:58 height:24 ;
			editText sfoldernameET "Scene folder:" align:#left ;
		)
		group "Objects Link"
		(
			checkbox makelinkCB "Create Links (.model, .link)" align:#left across:2
			button chooseLDirButton "Browse..." align:#right width:58 height:24 ;
			editText lfoldernameET "Link folder:" align:#left ;
		)
		button RunButton "Export scene" align:#left width:88 height:34
		label statusLB "Status"
		progressBar OverallPB ;

		on RunButton pressed do
		(
			-- variables
			local meshesExported = 0 ;
			local meshesTotal = 0;
			local scriptsCreated = false;
			local materialsCreated = false;
			local physicsCreated = false;
			local linksCreated = false;
			local scenename = scenenameET.text;
			local unitsScale = (optionRollout.unitsSP.value)
			if (selectRB.state == 1) then
				objset = objects
			else
				objset = selection
				
			strUnits = (optionRollout.unitsSP.value) as string
			setINISetting ((getDir #scripts) + "\\octopus\\octopusScript.ini") "Settings" "units" strUnits
			if (optionRollout.unitsDDL.selection == 1) then
				setINISetting ((getDir #scripts) + "\\octopus\\octopusScript.ini") "Settings" "unitsPre" "meters"
			else 
			if (optionRollout.unitsDDL.selection == 2) then
				setINISetting ((getDir #scripts) + "\\octopus\\octopusScript.ini") "Settings" "unitsPre" "millimeters"
			else 
			if (optionRollout.unitsDDL.selection == 3) then
				setINISetting ((getDir #scripts) + "\\octopus\\octopusScript.ini") "Settings" "unitsPre" "other"

			if (DefaultSettingsRollout.flipYZCB.checked) then		
				setINISetting ((getDir #scripts) + "\\octopus\\octopusScript.ini") "Settings" "flipYZ" "yes"
			else
				setINISetting ((getDir #scripts) + "\\octopus\\octopusScript.ini") "Settings" "flipYZ" "no"

			if (expmeshesCB.checked) then
				setINISetting ((getDir #scripts) + "\\octopus\\octopusScript.ini") "Settings" "ExportMeshes" "yes"
			else
				setINISetting ((getDir #scripts) + "\\octopus\\octopusScript.ini") "Settings" "ExportMeshes" "no"

			if (deletexmlCB.checked) then
				setINISetting ((getDir #scripts) + "\\octopus\\octopusScript.ini") "Settings" "DeleteTemporaryXML" "yes"
			else
				setINISetting ((getDir #scripts) + "\\octopus\\octopusScript.ini") "Settings" "DeleteTemporaryXML" "no"

			if (expskelsCB.checked) then
				setINISetting ((getDir #scripts) + "\\octopus\\octopusScript.ini") "Settings" "ExportSkeletons" "yes"
			else
				setINISetting ((getDir #scripts) + "\\octopus\\octopusScript.ini") "Settings" "ExportSkeletons" "no"

			if (expmaterialsCB.checked) then
				setINISetting ((getDir #scripts) + "\\octopus\\octopusScript.ini") "Settings" "ExportMaterials" "yes"
			else
				setINISetting ((getDir #scripts) + "\\octopus\\octopusScript.ini") "Settings" "ExportMaterials" "no"

			if (exportxodeCB.checked) then
				setINISetting ((getDir #scripts) + "\\octopus\\octopusScript.ini") "Settings" "ExportPhysics" "yes"
			else
				setINISetting ((getDir #scripts) + "\\octopus\\octopusScript.ini") "Settings" "ExportPhysics" "no"

			if (makesceneCB.checked) then
				setINISetting ((getDir #scripts) + "\\octopus\\octopusScript.ini") "Settings" "ExportScripts" "yes"
			else
				setINISetting ((getDir #scripts) + "\\octopus\\octopusScript.ini") "Settings" "ExportScripts" "no"

			if (makelinkCB.checked) then
				setINISetting ((getDir #scripts) + "\\octopus\\octopusScript.ini") "Settings" "ExportLinks" "yes"
			else
				setINISetting ((getDir #scripts) + "\\octopus\\octopusScript.ini") "Settings" "ExportLinks" "no"
				
			-- Analyze geometry objects: Detect originals and instances and store them in a list of lists.
			local objcount = objset.count;

			format "Analyzing % objects...\n" objcount
			local currcount = 0;
			local origs = #()
			for obj in objset where superclassof obj == GeometryClass do -- ALL GEOMETRY OBJECTS IN SCENE
			(
				format "Processing % ...\n" obj.name

				-- look for obj.name entry
				local found = false
				for t in origs do
				(
					local pos = findItem origs obj
					if pos == 1 then
					(
						-- main orig
						format "  found as ORIG\n"
						found = true
					)
					else
					if pos > 1 then
					(
						-- it's a clone
						format "  found as CLONE\n"
						found = true
					)
				)
				if found == false then
				(
					-- now test all dependent geometry
					local refArr = refs.dependents obj.baseobject
					for ref in refArr where superclassof ref == GeometryClass do
					(
						format "  testing REF '%'\n" ref.name
						for t in origs do
						(
							local pos = findItem t ref
							if pos == 1 then
							(
								-- ref is the original
								format "  ref found as ORIG => added as clone\n"
								append t obj
								found = true
							)
							-- ignore clones (pos > 1)
						)
					)
					-- still not found?
					if found == false then
					(
						local newentry = #(obj)
						append origs newentry
						format "  added as ORIG\n"
					)
				)
			) -- for each object in the selection set (or all of them)

			-- Export Meshes
			if ((expmeshesCB.checked) or (expskelsCB.checked)) then
			(
				if (mfoldernameET.text == "") then
					messageBox "You have to choose a folder for meshes." ;
				else
				(
					local exportdirok = false;
					local batfileok = false;
					local dirarray = getDirectories mfoldernameET.text;
					local batfilename = "";
					exportdirok = (dirarray.count != 0);
					if (exportdirok == false) then
					(
						if (queryBox ("Export folder does not exist!\nCreate folder \"" + mfoldernameET.text + "\" and continiue ?")) then
						(
							exportdirok = makeNewFolder mfoldernameET.text;
							if (exportdirok == false) then
								messageBox ("Cannot create folder \"" + mfoldernameET.text + "\"!\nMeshes export aborted");
						)
					)
					
					if (exportdirok and runconverterCB.checked) then
					(
						batfilename = mfoldernameET.text + "\\~~xmlrun.bat";
						batfile = createFile batfilename;
						if (batfile == undefined) then 
						(
							messageBox ("Error: cannot create batch file \"" + batfilename + "\"")
						)
						else
						(
 							batfileok = true;
						)
					)
					
					if (exportdirok) then 
					(
						clearlistener() ;
						xmlConvPath = getINISetting ((getDir #scripts) + "\\octopus\\OctopusScript.ini") "Directories" "XMLConverterPath"
						xmlexe= getINISetting ((getDir #scripts) + "\\octopus\\OctopusScript.ini") "Exe" "XMLConverterExe"
						options = exportOptions firstFrame:0 scale:DefaultSettingsRollout.scaleSP.value flipYZ:false flipNormal:false exportColours:false exportUV:false;
						
						format "Exporting % original mesh(es) out of % total object(s)...\n" origs.count objcount
						local currcount = 0;
						
						-- NB Enable the following line to export mesh for each and every instance or reference.
						--    Otherwise only orignals are exported (using the name of the first instance encountered).
						--for obj in objset where superclassof obj == GeometryClass do -- ALL GEOMETRY OBJECTS IN SCENE
						for orig in origs do
						(
							local obj = orig[1] --first entry is original, others are clones (if any)
							format "Exporting % ...\n" obj.name
						
							local convert = true;
							currcount = currcount + 1;
							OverallPB.value = 100 * currcount / objcount;
							exportingMeshDone = false ;
							exportingSkelDone = false ;
			
							if (obj.isTarget == false) then -- GET OBJECT WITH MESH
							(
								if (DefaultSettingsRollout.flipYZCB.checked) then
									options.flipYZ= true
								else
									options.flipYZ= false
								if (DefaultSettingsRollout.flipNormalCB.checked) then
									options.flipNormal = true
								else
									options.flipNormal = false
								if (DefaultSettingsRollout.exportColCB.checked) then
									options.exportColours = true
								else
									options.exportColours = false
								if (DefaultSettingsRollout.exportUVCB.checked) then
									options.exportUV = true
								else
									options.exportUV = false
								options.scale = DefaultSettingsRollout.scaleSP.value
								
								castsShadow = false
									
								for mod in obj.modifiers do
								(
									if (mod.name == "OctopusExport") then
									(
										options.flipYZ = mod.flipYZ;
										options.flipNormal = mod.flipNormal;
										options.exportColours = mod.exportColours;
										options.exportUV = mod.exportUV;
										options.scale = mod.scale
										castsShadow = mod.castsShadow;
										convert = mod.exportThis;
									)
									else
									if (mod.name == "ODE Body") then
									(
										convert = false;
										exit;
									)
								)
								
								if (convert) then
								(
									statusLB.text = "Converting mesh " + obj.name +"..."
									meshesTotal = meshesTotal + 1
									filename = mfoldernameET.text + "\\" + obj.name;
									lastFolder = setINISetting ((getDir #scripts) + "\\octopus\\octopusScript.ini") "Settings" "lastFolder" mfoldernameET.text	
									options.scale = options.scale / unitsScale
									
									if (expmeshesCB.checked) then
									(
										exportingMeshDone = writeMesh2 obj options unitsScale filename;
									)
									
									if (expskelsCB.checked) then
									(
										options.animName = "none"
										options.firstFrame = 1
										options.lastFrame = 10
										options.length = 1.0
										
										for mod in obj.modifiers do
										(
											if (mod.name == "OctopusExport") then
											(
												if(mod.anames.count > 0) then
												(
													if (mod.atype == 1) then -- Skin only animation
														exportingSkelDone = writeSkeleton2 obj options mod filename;
													else
													if (mod.atype == 2) then -- Biped animation
														exportingSkelDone = writeBiped2 obj options mod filename;
												)
											)
										)
									)
									
									if ((exportingMeshDone) or (exportingSkelDone)) then 
									(
										meshesExported = meshesExported + 1
										if (runconverterCB.checked) then
										(
											if (batfileok) then
											(
												xmlfullpath = xmlConvPath + "\\" + xmlexe
												if (exportingMeshDone) then
													format "% \"%.mesh.xml\" \"%.mesh\"\n" xmlfullpath filename filename to:batfile;
												if (exportingSkelDone) then
													format "% \"%.skeleton.xml\" \"%.skeleton\"\n" xmlfullpath filename filename to:batfile;
												if (deletexmlCB.checked) then
												(
													if (exportingMeshDone) then
														format "del \"%.mesh.xml\"\n" filename to:batfile;
													if (exportingSkelDone) then
														format "del \"%.skeleton.xml\"\n" filename to:batfile;
												)
											)
											else
											(
												if (exportingMeshDone) then
													DOSCommand (xmlConvPath + "\\" + xmlexe + " \"" + filename + ".mesh.xml\" \"" + filename + ".mesh\"") ;
												if (exportingSkelDone) then
													DOSCommand (xmlConvPath + "\\" + xmlexe + " \"" + filename + ".skeleton.xml\" \"" + filename + ".skeleton\"") ;
												if (deletexmlCB.checked) then
												(
													if (exportingMeshDone) then
														DOSCommand ("del \"" + filename + ".mesh.xml\"");
													if (exportingSkelDone) then
														DOSCommand ("del \"" + filename + ".skeleton.xml\"");
												)
											)
										)
									)
								)
							) -- ENDOF if (Superclassof obj
						) -- ENDOF for obj in objset do
						if (batfileok) then
						(
							statusLB.text = "Processing .XML files..."
							close batfile;
							DOSCommand batfilename;
							deleteFile batfilename;
						)
					) -- ENDOF if exportdirok
					
				) -- ENDOF if (foldernameET.text
			) -- ENDOF if (expmeshesCB.checked 
			
			if (expmaterialsCB.checked) then
			(
				if (mtfoldernameET.text == "") then
					messageBox "You have to choose a folder for materials." ;
				else
				(
					local exportdirok = false;
					local dirarray = getDirectories mtfoldernameET.text;
					
					exportdirok = (dirarray.count != 0);
					if (exportdirok == false) then
					(
						if (queryBox ("Export folder does not exist!\nCreate folder \"" + mtfoldernameET.text + "\" and continiue ?")) then
						(
							exportdirok = makeNewFolder mtfoldernameET.text;
							if (exportdirok == false) then
								messageBox ("Cannot create folder \"" + mtfoldernameET.text + "\"!\nMaterials export aborted");
						)
					)
					
					if (exportdirok) then 
					(
						local filematerialsok = false;

						local matcount = sceneMaterials.count;
						local currcount = 0;
						
						setINISetting ((getDir #scripts) + "\\octopus\\OctopusScript.ini") "Settings" "lastMaterialsFolder" mtfoldernameET.text
				
						fname = mtfoldernameET.text + "\\" + scenename + ".material";
						fileMaterials = createFile fname;
						if (fileMaterials == undefined) then 
						(
							messageBox ("Error: cannot create file \"" + fname + "\"")
						)
						else
						(
							filematerialsok = true;
						)
						
						if( filematerialsok ) then
						(
							materialsCreated = true;
							for mat in sceneMaterials do -- ALL MATERIALS IN SCENE
							(
								if (mat != undefined) then
								(
									local matprefix = ""
									statusLB.text = "Exporting material " + mat.name +"..."
									currcount = currcount + 1;
									OverallPB.value = 100 * currcount / matcount;
									exportSomeMaterial mat matprefix fileMaterials
								)
							) -- ENDOF foro obj in objset do
						) -- ENDOF if ( fileobjectsok...
					
						if filematerialsok then close fileMaterials
					) -- ENDOF if (exportdirok) then 
				) -- ENDOF else
			) -- ENDOF if (expmaterialsCB.checked) then
			
			if (exportxodeCB.checked) then
			(
				if (xfoldernameET.text == "") then
					messageBox "You have to choose a folder for XODE files." ;
				else
				(
					local exportdirok = false;
					local dirarray = getDirectories xfoldernameET.text;
					
					exportdirok = (dirarray.count != 0);
					if (exportdirok == false) then
					(
						if (queryBox ("Export folder does not exist!\nCreate folder \"" + xfoldernameET.text + "\" and continiue ?")) then
						(
							exportdirok = makeNewFolder xfoldernameET.text;
							if (exportdirok == false) then
								messageBox ("Cannot create folder \"" + xfoldernameET.text + "\"!\Physics export aborted");
						)
					)
					
					if (exportdirok) then 
					(
						local filephysicsok = false;

						local objcount = objset.count;
						local currcount = 0;
						
						setINISetting ((getDir #scripts) + "\\octopus\\OctopusScript.ini") "Settings" "lastPhysicsFolder" xfoldernameET.text
				
						fname = xfoldernameET.text + "\\" + scenename + ".xode";
						fileXODE = createFile fname;
						if (fileXODE == undefined) then 
						(
							messageBox ("Error: cannot create file \"" + fname + "\"")
						)
						else
						(
							filephysicsok = true;
						)
						
						if( filephysicsok ) then
						(
							physicsCreated = true;
							exportPhysHeader scenename fileXODE
							flipyz = DefaultSettingsRollout.flipYZCB.checked ;
							for obj in objset do -- ALL OBJECTS IN SCENE
							(
								if (obj != undefined) then
								(
									currcount = currcount + 1;
									OverallPB.value = 100 * currcount / objcount;
									for mod in obj.modifiers do
									(
										if (mod.name == "ODE Body") then
										(
											statusLB.text = "Exporting physics for " + obj.name +"..."
											exportPhysics obj mod unitsScale flipyz fileXODE
										)
									)
								)
							) -- ENDOF for obj in objset do
							exportPhysFooter fileXODE
						) -- ENDOF if ( filephysicsok...
						if filephysicsok then close fileXODE
					) -- ENDOF if (exportdirok) then 
				) -- ENDOF else
			) -- ENDOF if (exportxodeCB.checked) then
			
			if (makesceneCB.checked) then
			(
				if (sfoldernameET.text == "") then
					messageBox "You have to choose a folder for scripts." ;
				else
				(
					local exportdirok = false;
					local dirarray = getDirectories sfoldernameET.text;
					
					exportdirok = (dirarray.count != 0);
					if (exportdirok == false) then
					(
						if (queryBox ("Export folder does not exist!\nCreate folder \"" + sfoldernameET.text + "\" and continiue ?")) then
						(
							exportdirok = makeNewFolder sfoldernameET.text;
							if (exportdirok == false) then
								messageBox ("Cannot create folder \"" + sfoldernameET.text + "\"!\nScript creation aborted");
						)
					)
					
					if (exportdirok) then 
					(
						local filesceneok = false;

						local objcount = objset.count;
						local currcount = 0;
						
						setINISetting ((getDir #scripts) + "\\octopus\\OctopusScript.ini") "Settings" "lastScriptFolder" sfoldernameET.text
				
						fname = sfoldernameET.text + "\\" + scenename + ".scene";
						fileScene = createFile fname;
						if (fileScene == undefined) then 
						(
							messageBox ("Error: cannot create file \"" + fname + "\"")
						)
						else
						(
							filesceneok = true;
						)

						if( filesceneok ) then
						(
							scriptsCreated = true;
							flipyz = (DefaultSettingsRollout.flipYZCB.checked)
							
							modelNames = #()
							
							WriteSceneFileBegin fileScene scenename
							WriteSceneNodesBegin fileScene

							for obj in objset do -- ALL OBJECTS IN SCENE
							(
								format "(.scene) Processing %\n" obj.name
								currcount = currcount + 1;
								OverallPB.value = 100 * currcount / objcount;
								if (obj.parent == undefined or classOf obj.parent == YappModel) then
								(
									if (classOf obj.parent == YappModel) then
									(
										--TODO currently there can only be ONE root scene node for a single model assembly.
										format "Found yapp::model %\n" obj.parent.name
										notAlreadyProcessed = true
										if (modelNames.count > 0 and (findItem modelNames obj.parent.name) > 0) then
										(
											notAlreadyProcessed = false
											format "-> already processed!\n"
										)
										if (notAlreadyProcessed and IsValidSceneNode obj) then
										(
											statusLB.text = "Processing tree for yapp::model root node " + obj.name +"..."
											
											InstanceMgr.GetInstances obj.parent &objInstances
											for objInstance in objInstances do
											(
												format "ENTRY %\n" objInstance.name
												append modelNames objInstance.name
											)
											
											fname2 = sfoldernameET.text + "\\" + scenename +"-" + obj.parent.name + ".scene";
											fileModelScene = createFile fname2;
											if (fileModelScene == undefined) then
											(
												messageBox("Error: cannot create file \"" + fname2 + "\"")
											)
											else
											(
												WriteSceneFileBegin fileModelScene
												WriteSceneNodesBegin fileModelScene
												WriteSceneNode origs obj 1 scenename unitsScale flipyz fileModelScene
												WriteSceneNodesEnd fileModelScene
												WriteSceneFileEnd fileModelScene
												close fileModelScene
											)
										)
									)
									else
									(
										statusLB.text = "Processing tree for root node " + obj.name +"..."
										WriteSceneNode origs obj 1 scenename unitsScale flipyz fileScene
									)
								)
							) -- ENDOF foro obj in objset do
							WriteSceneNodesEnd fileScene
							WriteSceneFileEnd fileScene
							
						) -- ENDOF if ( fileobjectsok...
					
						if filesceneok then close fileScene
					
					) -- ENDOF if (exportdirok)
				)
			) -- ENDOF if (makesceneCB.checked

			if (makelinkCB.checked) then
			(
				if (lfoldernameET.text == "") then
					messageBox "You have to choose a folder for links." ;
				else
				(
					local exportdirok = false;
					local dirarray = getDirectories lfoldernameET.text;
					
					exportdirok = (dirarray.count != 0);
					if (exportdirok == false) then
					(
						if (queryBox ("Export folder does not exist!\nCreate folder \"" + lfoldernameET.text + "\" and continiue ?")) then
						(
							exportdirok = makeNewFolder lfoldernameET.text;
							if (exportdirok == false) then
								messageBox ("Cannot create folder \"" + lfoldernameET.text + "\"!\nLinks creation aborted");
						)
					)
					
					if (exportdirok) then 
					(
						local filelinkok = false;
						local filemodelok = false;
						
						setINISetting ((getDir #scripts) + "\\octopus\\OctopusScript.ini") "Settings" "lastLinksFolder" lfoldernameET.text
				
						fname = lfoldernameET.text + "\\" + scenename + ".link";
						fileLink = createFile fname;
						if (fileLink == undefined) then 
						(
							messageBox ("Error: cannot create file \"" + fname + "\"")
						)
						else
						(
							filelinkok = true;
						)

						fname = lfoldernameET.text + "\\" + scenename + ".model";
						fileModel = createFile fname;
						if (fileModel == undefined) then 
						(
							messageBox ("Error: cannot create file \"" + fname + "\"")
						)
						else
						(
							filemodelok = true;
						)
					
						if ( (filelinkok) and (filemodelok)) then
						(
							linksCreated = true;
							
							WriteLinksBegin fileLink
							statusLB.text = "Creating links..."
							for obj in objset do -- ALL OBJECTS IN SCENE
							(
								if ((classOf obj) == OGRE_ODE_Link) do
								(
									if (obj.m_isValid) do 
										WriteLink obj fileLink
								)

							) -- ENDOF foro obj in objset do
							WriteLinksEnd fileLink

							WriteModel scenename fileModel
							
						) -- ENDOF if ( fileobjectsok...
					
						if filelinkok then close fileLink
						if filemodelok then close fileModel
					
					) -- ENDOF if (exportdirok)
				)
			)
			
			if ( (((expmeshesCB.checked) or (expskelsCB.checked)) and (meshesExported > 0)) or ((makelinkCB.checked) and (linksCreated)) or ((makesceneCB.checked) and (scriptsCreated)) or ((expmaterialsCB.checked) and (materialsCreated)) or ((exportxodeCB.checked) and (physicsCreated))) then
			(
				statusLB.text = "Done!"
				setINISetting ((getDir #scripts) + "\\octopus\\octopusScript.ini") "Settings" "lastScene" scenename
				if (runconverterCB.checked) then
					setINISetting ((getDir #scripts) + "\\octopus\\octopusScript.ini") "Tools" "runXMLConverter" "yes"
				else
					setINISetting ((getDir #scripts) + "\\octopus\\octopusScript.ini") "Tools" "runXMLConverter" "no"
				messageBox ("Scene export finished.\nExported " + meshesExported as string + " of " + meshesTotal as string + " meshes")
				closeRolloutFloater  OctopusExportFloater
			)
		) -- ENDOF RunButton handler
		
		on chooseMDirButton pressed do
		(
			pathname = getSavePath caption:"Select export folder"
			if (pathname != undefined) then
			(
				mfoldernameET.text = pathname ;			)
		)

		on chooseMtDirButton pressed do
		(
			pathname = getSavePath caption:"Select export folder"
			if (pathname != undefined) then
			(
				mtfoldernameET.text = pathname ;
			)
		)

		on chooseXDirButton pressed do
		(
			pathname = getSavePath caption:"Select export folder"
			if (pathname != undefined) then
			(
				xfoldernameET.text = pathname ;
			)
		)

		on chooseSDirButton pressed do
		(
			pathname = getSavePath caption:"Select export folder"
			if (pathname != undefined) then
			(
				sfoldernameET.text = pathname ;
			)
		)

		on chooseLDirButton pressed do
		(
			pathname = getSavePath caption:"Select export folder"
			if (pathname != undefined) then
			(
				lfoldernameET.text = pathname ;
			) 		
		)

		on expmeshesCB changed state do
		(
			state = state or expskelsCB.checked
			mfoldernameET.enabled = state;
			chooseMDirButton.enabled = state;
			runconverterCB.enabled = state;
			deletexmlCB.enabled = runconverterCB.checked and state;
			if ((makesceneCB.checked == false) and (expmaterialsCB.checked == false) and (exportxodeCB.checked == false)) then RunButton.enabled = state;
		)

		on expskelsCB changed state do
		(
			state = state or expmeshesCB.checked			mfoldernameET.enabled = state;
			chooseMDirButton.enabled = state;
			runconverterCB.enabled = state;
			deletexmlCB.enabled = runconverterCB.checked and state;
			if ((makesceneCB.checked == false) and (expmaterialsCB.checked == false) and (exportxodeCB.checked == false)) then RunButton.enabled = state;
		)

		on expmaterialsCB changed state do
		(
			mtfoldernameET.enabled = state;
			chooseMtDirButton.enabled = state;
			if ((expmeshesCB.checked == false) and (expskelsCB.checked == false) and (makesceneCB.checked == false) and (exportxodeCB.checked == false)) then RunButton.enabled = state;
		)

		on makesceneCB changed state do
		(
			sfoldernameET.enabled = state;
			chooseSDirButton.enabled = state;
			if ((expmeshesCB.checked == false) and (expskelsCB.checked == false) and (expmaterialsCB.checked == false) and (exportxodeCB.checked == false)) then RunButton.enabled = state;
		)
		
		on makelinkCB changed state do
		(
			lfoldernameET.enabled = state;
			chooseLDirButton.enabled = state;
			if ((expmeshesCB.checked == false) and (expskelsCB.checked == false) and (expmaterialsCB.checked == false) and (exportxodeCB.checked == false) and (makesceneCB.checked == false)) then RunButton.enabled = state;
		)

		on exportxodeCB changed state do
		(
			xfoldernameET.enabled = state;
			chooseXDirButton.enabled = state;
			if ((expmeshesCB.checked == false) and (expskelsCB.checked == false) and (expmaterialsCB.checked == false) and (makesceneCB.checked == false)) then RunButton.enabled = state;
		)
		
		on runconverterCB changed state do
		(
			deletexmlCB.enabled = state;
		)
		
		on selectRB changed state do
		(
			selectButton.enabled = (state == 2)
		)
		
		on selectButton pressed do
		(
			max tool hlist;
		)
		
		on OctopusExportRollout open do
		(
			units = getINISetting ((getDir #scripts) + "\\octopus\\OctopusScript.ini") "Settings" "units"
			optionRollout.unitsSP.value = units as float;
			unitsPre = getINISetting ((getDir #scripts) + "\\octopus\\OctopusScript.ini") "Settings" "unitsPre"
			if (unitsPre == "meters") then
				optionRollout.unitsDDL.selection = 1 ;
			else
			if (unitsPre == "millimeters") then
				optionRollout.unitsDDL.selection = 2 ;
			else
				optionRollout.unitsDDL.selection = 3 ;
		
			lastFolder = getINISetting ((getDir #scripts) + "\\octopus\\OctopusScript.ini") "Settings" "lastFolder"			
			mfoldernameET.text = lastFolder ;
			lastFolder = getINISetting ((getDir #scripts) + "\\octopus\\OctopusScript.ini") "Settings" "lastPhysicsFolder"			
			xfoldernameET.text = lastFolder ;
			lastFolder = getINISetting ((getDir #scripts) + "\\octopus\\OctopusScript.ini") "Settings" "lastScriptFolder"			
			sfoldernameET.text = lastFolder ;
			lastFolder = getINISetting ((getDir #scripts) + "\\octopus\\OctopusScript.ini") "Settings" "lastMaterialsFolder"
			mtfoldernameET.text = lastFolder ;
			lastFolder = getINISetting ((getDir #scripts) + "\\octopus\\OctopusScript.ini") "Settings" "lastLinksFolder"
			lfoldernameET.text = lastFolder ;
			scenename = getFilenameFile maxFileName;
			if (scenename == "") then
				scenename = "untitled";
			scenenameET.text = scenename;
			
			tmpstring = getINISetting ((getDir #scripts) + "\\octopus\\OctopusScript.ini") "Settings" "flipYZ"
			if ((tmpstring == "yes") or (tmpstring == "y") or (tmpstring == "1")) then
				DefaultSettingsRollout.flipYZCB.checked = true;
			else
				DefaultSettingsRollout.flipYZCB.checked = false;

			tmpstring = getINISetting ((getDir #scripts) + "\\octopus\\OctopusScript.ini") "Settings" "ExportMeshes"
			if ((tmpstring == "yes") or (tmpstring == "y") or (tmpstring == "1")) then
				b_expm = true;
			else
				b_expm = false;
			tmpstring = getINISetting ((getDir #scripts) + "\\octopus\\OctopusScript.ini") "Settings" "ExportSkeletons"
			if ((tmpstring == "yes") or (tmpstring == "y") or (tmpstring == "1")) then
				b_exps = true;
			else
				b_exps = false;
			tmpstring = getINISetting ((getDir #scripts) + "\\octopus\\OctopusScript.ini") "Settings" "ExportPhysics"
			if ((tmpstring == "yes") or (tmpstring == "y") or (tmpstring == "1")) then
				b_expx = true;
			else
				b_expx = false
			tmpstring = getINISetting ((getDir #scripts) + "\\octopus\\OctopusScript.ini") "Settings" "ExportScripts"
			if ((tmpstring == "yes") or (tmpstring == "y") or (tmpstring == "1")) then
				b_expss = true;
			else
				b_expss = false
			tmpstring = getINISetting ((getDir #scripts) + "\\octopus\\OctopusScript.ini") "Settings" "ExportLinks"
			if ((tmpstring == "yes") or (tmpstring == "y") or (tmpstring == "1")) then
				b_expl = true;
			else
				b_expl = false
			tmpstring = getINISetting ((getDir #scripts) + "\\octopus\\OctopusScript.ini") "Settings" "ExportMaterials"
			if ((tmpstring == "yes") or (tmpstring == "y") or (tmpstring == "1")) then
				b_expmt = true;
			else
				b_expmt = false
			
			expmeshesCB.checked = b_expm
			expskelsCB.checked = b_exps
			b_state = b_expm or b_exps
			mfoldernameET.enabled = b_state;
			chooseMDirButton.enabled = b_state;
			runconverterCB.enabled = b_state;

			exportxodeCB.checked = b_expx;
			xfoldernameET.enabled = b_expx;
			chooseXDirButton.enabled = b_expx;

			deletexmlCB.checked = true ; -- default
			sTmp = getINISetting ((getDir #scripts) + "\\octopus\\OctopusScript.ini") "Settings" "DeleteTemporaryXML"
			if (sTmp != "") then
			(
				if ((sTmp == "yes") or (sTmp == "y") or (sTmp == "1")) then
					deletexmlCB.checked = true ;
				else
					deletexmlCB.checked = false ;
			)

			makesceneCB.checked = b_expss;
			sfoldernameET.enabled = b_expss;
			chooseSDirButton.enabled = b_expss;

			makelinkCB.checked = b_expl;
			lfoldernameET.enabled = b_expl;
			chooseLDirButton.enabled = b_expl;

			expmaterialsCB.checked = b_expmt;
			mtfoldernameET.enabled = b_expmt;
			chooseMtDirButton.enabled = b_expmt;

			b_state = b_state or b_expss or b_expmt or b_expl or b_expx;
			RunButton.enabled = b_state;
			
			if (selection.count > 0) then
			(
				selectRB.state = 2;
				selectButton.enabled = true;
			)
			else
			(
				selectRB.state = 1;
				selectButton.enabled = false;
			)
			
			runstring = getINISetting ((getDir #scripts) + "\\octopus\\OctopusScript.ini") "Tools" "runXMLConverter"
			if ((runstring == "yes") or (runstring == "y") or (runstring == "1")) then
				runconverterCB.checked = true;
			else
				runconverterCB.checked = false;
			deletexmlCB.enabled = (runconverterCB.checked and runconverterCB.enabled);
		)
	)
	
	addRollout optionRollout OctopusExportFloater rolledUp:true;
	addRollout DefaultSettingsRollout OctopusExportFloater rolledUp:true;
	addRollout OctopusExportRollout OctopusExportFloater;
)
