[Setup]
Compression=lzma/ultra
SolidCompression=yes

AppName=ireon.org client
AppVerName=ireon.org client r3 Mayflower
VersionInfoVersion=0.0.1.34
VersionInfoCopyright=ireon.org
AppPublisher=ireon.org
AppPublisherURL=http://www.ireon.org
AppSupportURL=http://www.ireon.org
AppUpdatesURL=http://www.ireon.org

DefaultDirName={pf32}\ireon.org client
DefaultGroupName=ireon.org client
AllowNoIcons=yes
DisableReadyPage=yes

SourceDir=client_setup

OutputDir=.
OutputBaseFilename=ireon.org_client_r3_setup
SetupIconFile=setup.ico

LicenseFile=setup_src\LICENSE
InfoAfterFile="setup_src\RELEASE NOTES"


[Languages]
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "setup_src\*"; DestDir: "{app}"; Flags: recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[INI]
Filename: "{app}\ireon.org website.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://www.ireon.org"

[Icons]
Name: "{group}\ireon.org client launcher"; Filename: "{app}\tools\python-bin\launcher.exe"; WorkingDir: "{app}\tools\python-bin"
Name: "{group}\{cm:ProgramOnTheWeb,ireon.org}"; Filename: "{app}\ireon.org website.url";
Name: "{group}\{cm:UninstallProgram,ireon.org client}"; Filename: "{uninstallexe}"
Name: "{userdesktop}\ireon.org client launcher"; Filename: "{app}\tools\python-bin\launcher.exe"; WorkingDir: "{app}\tools\python-bin"; Tasks: desktopicon

[Run]
Filename: "{app}\update.bat"; Flags: shellexec waituntilterminated
Filename: "{app}\tools\python-bin\launcher.exe"; WorkingDir: "{app}\tools\python-bin";  Description: "{cm:LaunchProgram,ireon.org client}"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
Type: files; Name: "{app}\ireon.org website.url"
Type: filesandordirs; Name: "{app}\bin\.svn\*"
Type: filesandordirs; Name: "{app}\bin\.svn\"

