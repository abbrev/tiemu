; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!
;
; (c) Copyright 2001-2004, the TiLP team
;
; $Id$

[Setup]
AppName=TiEmu
AppVerName=TiEmu 2.00
AppPublisher=The TiLP Team
AppPublisherURL=http://lpg.ticalc.org/prj_tilp/tilp-news.php
AppSupportURL=http://lpg.ticalc.org/prj_tilp/tilp-staff.php
AppUpdatesURL=http://lpg.ticalc.org/prj_tilp/tilp-download.php
DefaultDirName={pf}\TiEmu
DefaultGroupName=TiEmu
AllowNoIcons=yes
LicenseFile=C:\sources\roms\tiemu\COPYING
InfoBeforeFile=C:\sources\roms\tiemu\README.win32
InfoAfterFile=C:\sources\roms\tiemu\RELEASE

[Tasks]
Name: "desktopicon"; Description: "Create a &desktop icon"; GroupDescription: "Additional icons:"; MinVersion: 4,4
Name: "quicklaunchicon"; Description: "Create a &Quick Launch icon"; GroupDescription: "Additional icons:"; MinVersion: 4,4; Flags: unchecked

[Files]
; Glade files
Source: "C:\sources\roms\tiemu\glade\*.glade"; DestDir: "{app}\glade"; Flags: ignoreversion;
; Pixmaps files
Source: "C:\sources\roms\tiemu\pixmaps\*.xpm"; DestDir: "{app}\pixmaps"; Flags: ignoreversion;
; Skin files
Source: "C:\sources\roms\tiemu\skins\*.skn"; DestDir: "{app}\skins"; Flags: ignoreversion;
Source: "C:\sources\roms\tiemu\skins\ti92.skn"; DestDir: "{app}\skins"; DestName: "ti92+.skn"; Flags: ignoreversion;
Source: "C:\sources\roms\tiemu\skins\ti89.skn"; DestDir: "{app}\skins"; DestName: "ti89t.skn"; Flags: ignoreversion;
; i18n files
;Source: "C:\sources\roms\libs\files\po\fr.gmo"; DestDir: "{app}\locale\fr\LC_MESSAGES"; DestName: "tifiles.mo"; Flags: ignoreversion;
;Source: "C:\sources\roms\libs\cables\po\fr.gmo"; DestDir: "{app}\locale\fr\LC_MESSAGES"; DestName: "ticables.mo"; Flags: ignoreversion;
;Source: "C:\sources\roms\libs\calcs\po\fr.gmo"; DestDir: "{app}\locale\fr\LC_MESSAGES"; DestName: "ticalcs.mo"; Flags: ignoreversion;
;Source: "C:\sources\roms\tiemu\po\fr.gmo"; DestDir: "{app}\locale\fr\LC_MESSAGES"; DestName: "tiemu.mo"; Flags: ignoreversion;
; Misc files
Source: "C:\sources\roms\tiemu\README.win32"; DestDir: "{app}"; DestName: "Readme_Win32.txt"; Flags: ignoreversion
Source: "C:\sources\roms\tiemu\AUTHORS"; DestDir: "{app}"; DestName: "Authors.txt"; Flags: ignoreversion
Source: "C:\sources\roms\tiemu\CHANGELOG"; DestDir: "{app}"; DestName: "ChangeLog.txt"; Flags: ignoreversion
Source: "C:\sources\roms\tiemu\COPYING"; DestDir: "{app}"; DestName: "License.txt"; Flags: ignoreversion
Source: "C:\sources\roms\tiemu\man\ManPage.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\sources\roms\tiemu\README"; DestDir: "{app}"; DestName: "ReadMe.txt"; Flags: ignoreversion isreadme
Source: "C:\sources\roms\tiemu\RELEASE"; DestDir: "{app}"; DestName: "Release.txt"; Flags: ignoreversion
Source: "C:\sources\roms\tiemu\ROMs\romcalls.lst"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\sources\roms\tiemu\ROMs\romcalls.txt"; DestDir: "{app}"; Flags: ignoreversion
; TiEmu/GTK
Source: "C:\sources\roms\tifiles\tests\tifiles.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\sources\roms\ticables\tests\ticables.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\sources\roms\ticalcs\tests\ticalcs.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\sources\roms\tiemu\build\msvc\tiemu.exe"; DestDir: "{app}"; DestName: "tiemu.exe"; Flags: ignoreversion
;Source: "C:\WinNT\system32\MSVCRTD.DLL"; DestDir: "{app}"; Flags: ignoreversion
; Copy PortTalk driver for Windows NT4/2000/XP
Source: "C:\sources\roms\Porttalk22\PortTalk.sys"; DestDir: "{sys}\drivers"; Flags: ignoreversion
Source: "C:\sources\roms\Porttalk22\PortTalk.sys"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\sources\roms\Porttalk22\AllowIO.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\sources\roms\Porttalk22\Uninstall.exe"; DestDir: "{app}"; Flags: ignoreversion
; Script to modify AUTOEXEC.bat
; Install helper
Source: "C:\sources\roms\tiemu\build\InnoSetup\AddEntry\AddEntry.exe"; DestDir: "{app}"; Flags: ignoreversion; Attribs: hidden; MinVersion: 4,0;

; Fix Gtk-Wimp installation problem (file is not at the right location)
;Source: "C:\Program Files\Fichiers Communs\GTK\2.0\lib\libwimp.dll"; DestDir: "{code:GetGtkPath}\lib\gtk-2.0\2.2.0\engines"; Flags: onlyifdoesntexist uninsneveruninstall;
Source: "C:\Program Files\Common Files\GTK\2.0\lib\libwimp.dll"; DestDir: "{code:GetGtkPath}\lib\gtk-2.0\2.2.0\engines"; Flags: onlyifdoesntexist uninsneveruninstall; MinVersion: 0,4;

[Dirs]
;Name: "{app}\My TI images"; Flags: uninsneveruninstall;
;Name: "{app}\plugins"; Flags: uninsneveruninstall;

[INI]
Filename: "{app}\tiemu.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://lpg.ticalc.org/prj_tiemu"

[Icons]
Name: "{group}\TiEmu"; Filename: "{app}\tiemu.exe"; WorkingDir: "{app}\My TI files"
Name: "{group}\TiEmu on the Web"; Filename: "{app}\tiemu.url"
Name: "{group}\Uninstall TiEmu"; Filename: "{uninstallexe}"
Name: "{group}\User's Manual"; Filename: "{app}\help\User_Manual.html"

Name: "{userdesktop}\TiEmu"; Filename: "{app}\tiemu.exe"; WorkingDir: "{app}\My TI files"; MinVersion: 4,4; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\TiEmu"; Filename: "{app}\tiemu.exe"; WorkingDir: "{app}\My TI files"; MinVersion: 4,4; Tasks: quicklaunchicon

[Run]
; Remove any previously installed PortTalk driver (especially v1.x)
Filename: "{app}\Uninstall.exe"; Parameters: ""; MinVersion: 0,4;
; Boost GTK2 (Win9x/Me)
Filename: "{app}\AddEntry.exe"; Description: "Modify AUTOEXEC.BAT (you will have to restart Windows !)"; StatusMsg: "Modifying AUTOEXEC.BAT..."; Flags: postinstall nowait runminimized; MinVersion: 4,0;

[UninstallRun]
; Remove any previously installed PortTalk driver (especially v1.x)
Filename: "{app}\Uninstall.exe"; Parameters: ""; MinVersion: 0,4;

[Registry]
; This adds the GTK+ libraries to gtk-foo.exe's path
Root: HKLM; Subkey: "Software\Microsoft\Windows\CurrentVersion\App Paths\tiemu.exe"; Flags: uninsdeletekeyifempty
Root: HKLM; Subkey: "Software\Microsoft\Windows\CurrentVersion\App Paths\tiemu.exe"; ValueType: string; ValueData: "{app}\tiemu.exe"; Flags: uninsdeletevalue
Root: HKLM; Subkey: "Software\Microsoft\Windows\CurrentVersion\App Paths\tiemu.exe"; ValueType: string; ValueName: "Path"; ValueData: "{app};{code:GetGtkPath}\lib"; Flags: uninsdeletevalue

[UninstallDelete]
Type: files; Name: "{app}\tiemu.url"

;; Taken from "http://www.dropline.net/gtk/support.php"

[Code]
var
  Exists: Boolean;
  GtkPath: String;
  WimpPath: String;

function GetGtkInstalled (): Boolean;
begin
  Exists := RegQueryStringValue (HKLM, 'Software\GTK\2.0', 'Path', GtkPath);
  if not Exists then begin
    Exists := RegQueryStringValue (HKCU, 'Software\GTK\2.0', 'Path', GtkPath);
  end;
   Result := Exists
end;

function GetOldGtkInstalled (): Boolean;
begin
  Exists := RegQueryStringValue (HKLM, 'Software\Microsoft\Windows\CurrentVersion\Uninstall\gtk-win32-runtime_is1', 'DisplayName', GtkPath);
  if not Exists then begin
    Exists := RegQueryStringValue (HKCU, 'Software\Microsoft\Windows\CurrentVersion\Uninstall\gtk-win32-runtime_is1', 'DisplayName', GtkPath);
  end;
   Result := Exists
end;

function GetGtkPath (S: String): String;
begin
    Result := GtkPath;
end;

function IsTiglUsbVersion3Mini (): Boolean;
var
  Version: String;
begin
  GetVersionNumbersString('C:\WinNT\System\TiglUsb.dll', Version);
  if CompareStr(Version, '3.0.0.0') < 0 then begin
    Result := false;
  end;
end;

function InitializeSetup(): Boolean;
begin
  Result := GetOldGtkInstalled ();
  if Result then begin
    MsgBox ('Warning: it seems you have the "GTK+/Win32 Runtime (2003-05-14)" package installed. TiEmu is now using a more standard package. You must uninstall it else TiEmu will fail to start.', mbError, MB_OK);
  end;
  
  Result := GetGtkInstalled ();
  if not Result then begin
    MsgBox ('Please install the "GTK+ 2.0 Runtime Environment" (v2.2.4-3) of DropLine Systems. You can obtain GTK+ from <http://prdownloads.sourceforge.net/gtk-win/GTK-Runtime-Environment-2.2.4-3.exe?download>.', mbError, MB_OK);
  end;
  
  if Result then begin
      WimpPath := GtkPath + '\lib\gtk-2.0\2.2.0\engines\libwimp.dll';
      if FileExists(WimpPath) and not UsingWinNT() then begin
        DeleteFile(WimpPath);
        MsgBox('The GTK+ Wimp theme engine has been disabled to avoid lot of warnings in console.', mbError, MB_OK);
      end;
  end;
  
  if IsTiglUsbVersion3Mini() then begin
    MsgBox('SilverLink driver v2.x has been removed of your system. Now, TiEmu requires v3.x (check out the README for download location).', mbError, MB_OK);
  end;
end;
