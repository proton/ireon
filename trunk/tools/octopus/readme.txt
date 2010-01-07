1. copy dir 'Scripts' in installation dir 3ds max, replace file if need
2. Run 3DSMAX and go to MAXScript/Run Script
3. Browse '3dsmax\scripts\octopus\macros'
4. Choose “OctopusToolbar.mcr”, and there is nothing appears.
5. Go to Customize/Customize User Interface, click on Toolbars tab and choose “Octopus Extensions” from the Category pull-down menu.
6. You will see an icon named “Export scene to Octopus”. Put that icon on the new tab with the name, “Octopus Exporter”. (or any name you wish.)
7. Click on the “Export scene to Octopus” icon, open OctopusScript.ini. There are a few things to be corrected.
8. The following lines are supposed to point to your Octopus directory:

[Directories]
XMLConverterPath=C:\3dsmax\scripts\octopus\exe
OgreViewerExePath=C:\3dsmax\scripts\octopus\viewer
OgreViewerMediaPath=C:\3dsmax\scripts\octopus\viewer
