; ----------------------------------------------------------------------
; Edit the define value before compiling the script
; ----------------------------------------------------------------------
#define AppName "Raptor"
#define AppPublisher "Raptor"
#define AppURL "http://code.google.com/p/raptor/"

; #define arguments :
; - AppVersion : raptor version
; - AppSrcDir  : raptor root directory
; - AppInstallDir : raptor installation directory
; - OutputDir  : innosetup output directory
; ----------------------------------------------------------------------

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{BCC0D579-D0B8-4DE3-9DDE-F3FE9481E2A7}
AppName={#AppName}
AppVersion={#AppVersion}
AppVerName={#AppName}-{#AppVersion}
AppPublisher={#AppPublisher}
AppPublisherURL={#AppURL}
AppSupportURL={#AppURL}
AppUpdatesURL={#AppURL}
DefaultDirName={pf}\{#AppName}
DefaultGroupName={#AppName}
AllowNoIcons=yes
SourceDir={#AppSrcDir}
LicenseFile=LICENSE.GPL3
;InfoBeforeFile=
;InfoAfterFile=
OutputDir={#OutputDir}
OutputBaseFilename=raptor-{#AppVersion}-setup
SetupIconFile=src/images/raptor.ico
Compression=lzma
SolidCompression=yes
PrivilegesRequired=none
WizardImageFile=package/windows/WizModernImage-IS.bmp
WizardSmallImageFile=package/windows/WizModernSmallImage-IS.bmp

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "french"; MessagesFile: "compiler:Languages\French.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Files]
Source: "{#AppInstallDir}\*.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#AppInstallDir}\*.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#AppInstallDir}\translations\*"; DestDir: "{app}\translations"; Flags: ignoreversion
Source: "LICENSE.GPL3"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\Raptor"; Filename: "{app}\raptor.exe"
Name: "{group}\{cm:ProgramOnTheWeb,Raptor}"; Filename: "http://code.google.com/p/raptor/"
Name: "{group}\{cm:UninstallProgram,Raptor}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\Raptor"; Filename: "{app}\raptor.exe"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\Raptor"; Filename: "{app}\raptor.exe"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\raptor.exe"; Description: "{cm:LaunchProgram,Raptor}"; Flags: nowait postinstall skipifsilent


[Registry]
; Raptor registry parameters
Root: HKCU; Subkey: "Software\Raptor"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\Raptor"; ValueType: string; ValueName: "InstallDir"; ValueData: "{app}"
Root: HKCU; Subkey: "Software\Raptor"; ValueType: string; ValueName: "Version"; ValueData: "{#AppVersion}"

; Paths
Root: HKLM; Subkey: SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\raptor.exe; ValueType: string; ValueData: {app}\raptor.exe; Flags: uninsdeletekey;
Root: HKLM; Subkey: SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\raptor.exe; ValueType: string; ValueName: Path; ValueData: {app};

; Contextual Menu & Native Associations
Root: HKCR; SubKey: Raptor; ValueType: string; ValueData: Universal Extractor Archive; Flags: uninsdeletekey;
;Root: HKCR; SubKey: Raptor\DefaultIcon; ValueType: string; ValueData: {app}\raptor.ico;
Root: HKCR; SubKey: Raptor\shell\raptor; ValueType: string; ValueData: "Edit with Raptor";
Root: HKCR; SubKey: Raptor\shell\raptor\command; ValueType: string; ValueData: """{app}\raptor.exe"" ""%1""";
Root: HKCR; SubKey: *\shell\raptor; ValueType: string; ValueData: "Edit with Raptor"; Flags: uninsdeletekey;
Root: HKCR; SubKey: *\shell\raptor\command; ValueType: string; ValueData: """{app}\raptor.exe"" ""%1""";
