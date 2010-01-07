
-- initOctopus.ms
--
-- Initializes the ogre 3dsmax tools. Just place in scripts/startup
-- folder and Octopus extentions will be loaded with max.

-- load custom modifiers
  fileIn "Octopus/lib/plugins/OctopusMeshModifier.ms"
  fileIn "Octopus/lib/plugins/ODEHelpers.ms"
  fileIn "Octopus/lib/plugins/ODEBody.ms"

-- load custom materials
  fileIn "Octopus/lib/plugins/OctopusTextureUnit.ms"
  fileIn "Octopus/lib/plugins/OctopusMaterial.ms"
  fileIn "Octopus/lib/plugins/OctopusTechnique.ms"
  fileIn "Octopus/lib/plugins/OctopusAdvancedMaterial.ms"

-- load custom toolbar
  fileIn "Octopus/macros/OctopusToolbar.mcr"