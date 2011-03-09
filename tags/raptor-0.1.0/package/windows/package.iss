; ----------------------------------------------------------------------
; Edit the define value before compiling the script
; ----------------------------------------------------------------------
#define MyAppSrcDir "C:\raptor"
#define MyAppName "Raptor"
#define MyAppVersion "0.1.0"
#define MyAppPublisher "Raptor"
#define MyAppURL "http://code.google.com/p/raptor/"
#define MyAppExeName "raptor.exe"

; ----------------------------------------------------------------------

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{BCC0D579-D0B8-4DE3-9DDE-F3FE9481E2A7}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppVerName={#MyAppName}-{#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
AllowNoIcons=yes
SourceDir={#MyAppSrcDir}
LicenseFile=LICENSE.GPL3
;InfoBeforeFile=
;InfoAfterFile=
OutputDir=package/windows
OutputBaseFilename=setup
SetupIconFile=src/images/raptor.ico
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "french"; MessagesFile: "compiler:Languages\French.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Files]
Source: "delivery\release\raptor.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "delivery\release\libgcc_s_dw2-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "delivery\release\mingwm10.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "delivery\release\QtCore4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "delivery\release\QtGui4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "delivery\release\QtNetwork4.dll"; DestDir: "{app}"; Flags: ignoreversion
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
Root: HKCU; Subkey: "Software\Raptor"; ValueType: string; ValueName: "Version"; ValueData: "{#MyAppVersion}"

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
