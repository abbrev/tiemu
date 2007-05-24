; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!
;
; (c) Copyright 2001-2006, the TiEmu team
;
; $Id: tiemu.iss 639 2004-08-19 15:35:33Z roms $

[Setup]
AppName=TiEmu3-gdb
AppVerName=TiEmu 3.00 No Gdb
AppPublisher=The TiEmu Team
AppPublisherURL=http://lpg.ticalc.org/prj_tiemu/index.html
AppSupportURL=http://lpg.ticalc.org/prj_tiemu/mailing_list.html
AppUpdatesURL=http://lpg.ticalc.org/prj_tiemu/win32_download.html
DefaultDirName={pf}\TiEmu3-gdb
DefaultGroupName=TiEmu3-gdb
AllowNoIcons=yes
LicenseFile=C:\sources\roms\tiemu3\COPYING
InfoBeforeFile=C:\sources\roms\tiemu3\README.win32
InfoAfterFile=C:\sources\roms\tiemu3\RELEASE.nogdb

[Tasks]
Name: "desktopicon"; Description: "Create a &desktop icon"; GroupDescription: "Additional icons:"; MinVersion: 4,4
Name: "quicklaunchicon"; Description: "Create a &Quick Launch icon"; GroupDescription: "Additional icons:"; MinVersion: 4,4; Flags: unchecked

Name: "slv_drv"; Description: "Copy SilverLink drivers"; GroupDescription: "Drivers:"; MinVersion: 0,4
Name: "tlk_drv"; Description: "Install BlackLink/Parallel cable"; GroupDescription: "Drivers:"; MinVersion: 0,4

Name: "com_ole"; Description: "Install TiEmuOle object for TIGCC and others"; GroupDescription: "Misc";

[Dirs]
Name: "{app}\screenshots"

[Files]
; Glade files
Source: "C:\sources\roms\tiemu3\glade\*.glade"; DestDir: "{app}\glade"; Flags: ignoreversion;

; Help files
Source: "C:\sources\roms\tiemu3\help\*.jpg"; DestDir: "{app}\help"; Flags: ignoreversion;
Source: "C:\sources\roms\tiemu3\help\*.png"; DestDir: "{app}\help"; Flags: ignoreversion;
Source: "C:\sources\roms\tiemu3\help\*.htm?"; DestDir: "{app}\help"; Flags: ignoreversion;

; Pixmaps files
Source: "C:\sources\roms\tiemu3\pixmaps\*.xpm"; DestDir: "{app}\pixmaps"; Flags: ignoreversion;
Source: "C:\sources\roms\tiemu3\build\msvc\small.ico"; DestDir: "{app}"; DestName: "romdump.ico"; Flags: ignoreversion;

; Skin files
Source: "C:\sources\roms\tiemu3\skins\*.skn"; DestDir: "{app}\skins"; Flags: ignoreversion;

; Keymap files
Source: "C:\sources\roms\tiemu3\skins\*.map"; DestDir: "{app}\skins"; Flags: ignoreversion;
Source: "C:\sources\roms\tiemu3\skins\ti92.map"; DestDir: "{app}\skins"; DestName: "v200plt.map"; Flags: ignoreversion;

; i18n files
Source: "C:\sources\roms\tifiles2\po\fr.gmo"; DestDir: "{app}\locale\fr\LC_MESSAGES"; DestName: "libtifiles2.mo"; Flags: ignoreversion;
Source: "C:\sources\roms\ticables2\po\fr.gmo"; DestDir: "{app}\locale\fr\LC_MESSAGES"; DestName: "libticables2.mo"; Flags: ignoreversion;
Source: "C:\sources\roms\ticalcs2\po\fr.gmo"; DestDir: "{app}\locale\fr\LC_MESSAGES"; DestName: "libticalcs2.mo"; Flags: ignoreversion;
Source: "C:\sources\roms\tiemu3\po\fr.gmo"; DestDir: "{app}\locale\fr\LC_MESSAGES"; DestName: "tiemu3.mo"; Flags: ignoreversion;

; Misc files
Source: "C:\sources\roms\tiemu3\AUTHORS"; DestDir: "{app}"; DestName: "Authors.txt"; Flags: ignoreversion
Source: "C:\sources\roms\tiemu3\BUGS"; DestDir: "{app}"; DestName: "Bugs.txt"; Flags: ignoreversion
Source: "C:\sources\roms\tiemu3\CHANGELOG"; DestDir: "{app}"; DestName: "ChangeLog.txt"; Flags: ignoreversion
Source: "C:\sources\roms\tiemu3\COPYING"; DestDir: "{app}"; DestName: "License.txt"; Flags: ignoreversion
Source: "C:\sources\roms\tiemu3\LICENSES"; DestDir: "{app}"; DestName: "Licenses.txt"; Flags: ignoreversion
Source: "C:\sources\roms\tiemu3\man\ManPage.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\sources\roms\tiemu3\README.win32"; DestDir: "{app}"; DestName: "ReadMe.txt"; Flags: ignoreversion isreadme
Source: "C:\sources\roms\tiemu3\RELEASE"; DestDir: "{app}"; DestName: "Release.txt"; Flags: ignoreversion
Source: "C:\sources\roms\tiemu3\TODO"; DestDir: "{app}"; DestName: "ToDo.txt"; Flags: ignoreversion

; Resource files
Source: "C:\sources\roms\tiemu3\misc\romcalls.txt"; DestDir: "{app}\misc"; Flags: ignoreversion
Source: "C:\sources\roms\tiemu3\misc\iodefs*.txt"; DestDir: "{app}\misc"; Flags: ignoreversion
Source: "C:\sources\roms\tiemu3\misc\memmap*.txt"; DestDir: "{app}\misc"; Flags: ignoreversion

; PedRom files
Source: "C:\sources\roms\tiemu3\pedrom\pedrom*.tib"; DestDir: "{app}\pedrom"; Flags: ignoreversion

; TiEmu/GTK
Source: "C:\sources\roms\tifiles2\tests\libtifiles2-3.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\sources\roms\ticables2\tests\libticables2-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\sources\roms\ticalcs2\tests\libticalcs2-2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\sources\roms\ticonv\tests\libticonv-2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\sources\roms\tiemu3\build\msvc\tiemu.exe"; DestDir: "{app}"; DestName: "tiemu.exe"; Flags: ignoreversion

; Copy PortTalk driver for Windows NT4/2000/XP
Source: "C:\sources\roms\misc\Porttalk22\PortTalk.sys"; DestDir: "{sys}\drivers"; Flags: ignoreversion; Tasks: tlk_drv;
Source: "C:\sources\roms\misc\Porttalk22\PortTalk.sys"; DestDir: "{app}\PortTalk"; Flags: ignoreversion; Tasks: tlk_drv;
Source: "C:\sources\roms\misc\Porttalk22\AllowIO.exe"; DestDir: "{app}\PortTalk"; Flags: ignoreversion; Tasks: tlk_drv;
Source: "C:\sources\roms\misc\Porttalk22\Uninstall.exe"; DestDir: "{app}\PortTalk"; Flags: ignoreversion; Tasks: tlk_drv;

; Copy LPG's SilverLink driver
Source: "C:\sources\roms\tiglusb\src\xp\driver\License.txt"; DestDir: "{app}\slvdrvXP"; Tasks: slv_drv;
Source: "C:\sources\roms\tiglusb\src\xp\driver\TiglUsb.dll"; DestDir: "{app}\slvdrvXP"; Tasks: slv_drv;
Source: "C:\sources\roms\tiglusb\src\xp\driver\TiglUsb.inf"; DestDir: "{app}\slvdrvXP"; Tasks: slv_drv;
Source: "C:\sources\roms\tiglusb\src\xp\driver\TiglUsb.sys"; DestDir: "{app}\slvdrvXP"; Tasks: slv_drv;
Source: "C:\sources\roms\tiglusb\src\xp\driver\TiglUsb.dll"; DestDir: "{sys}\drivers";  Tasks: slv_drv;
Source: "C:\sources\roms\tiglusb\src\xp\driver\TiglUsb.sys"; DestDir: "{sys}\drivers";  Tasks: slv_drv; MinVersion: 0,4;

Source: "C:\sources\roms\tiglusb\src\98\driver\License.txt"; DestDir: "{app}\slvdrv98"; Tasks: slv_drv;
Source: "C:\sources\roms\tiglusb\src\98\driver\TiglUsb.dll"; DestDir: "{app}\slvdrv98";  Tasks: slv_drv;
Source: "C:\sources\roms\tiglusb\src\98\driver\TiglUsb.inf"; DestDir: "{app}\slvdrv98"; Tasks: slv_drv;
Source: "C:\sources\roms\tiglusb\src\98\driver\TiglUsb.sys"; DestDir: "{app}\slvdrv98";  Tasks: slv_drv;
Source: "C:\sources\roms\tiglusb\src\98\driver\TiglUsb.dll"; DestDir: "{sys}\drivers";  Tasks: slv_drv;
Source: "C:\sources\roms\tiglusb\src\98\driver\TiglUsb.sys"; DestDir: "{sys}\drivers";  Tasks: slv_drv; MinVersion: 4,0;

; GTK+ specific
Source: "C:\Gtk2Dev\bin\gtkthemeselector.exe"; DestDir: "{app}";
;libglade/libxml add-on (ignore since no version checking is possible)
Source: "C:\Gtk2Dev\bin\libxml2.dll"; DestDir: "{app}"; Flags: onlyifdoesntexist;
Source: "C:\Gtk2Dev\bin\libglade-2.0-0.dll"; DestDir: "{app}"; Flags: onlyifdoesntexist;

; Downloader
Source: "C:\sources\roms\tilp2\build\InnoSetup\wget\*.dll"; DestDir: "{app}\wget";
Source: "C:\sources\roms\tilp2\build\InnoSetup\wget\wget.exe"; DestDir: "{app}\wget";
Source: "C:\sources\roms\tilp2\build\InnoSetup\wget\d_and_i.bat"; DestDir: "{app}\wget";

; COM/OLE object registration
Source: "C:\sources\roms\tiemu3\src\ipc\com\tiemups.dll"; DestDir: "{app}"; Flags: regserver;
Source: "C:\sources\roms\tiemu3\src\ipc\com\oleaut.tlb"; DestDir: "{app}"; DestName: "tiemu.tlb";

[INI]
Filename: "{app}\tiemu.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://lpg.ticalc.org/prj_tiemu"

[Icons]
Name: "{group}\TiEmu"; Filename: "{app}\tiemu.exe"; WorkingDir: "{app}"
Name: "{group}\TiEmu on the Web"; Filename: "{app}\tiemu.url"
Name: "{group}\Uninstall TiEmu"; Filename: "{uninstallexe}"
Name: "{group}\User's Manual"; Filename: "{app}\help\Manual_en.html"
Name: "{group}\GTK theme selector"; Filename: "{app}\gtkthemeselector.exe";
Name: "{group}\Install GTK+ from web"; Filename: "{app}\wget\d_and_i.bat";
Name: "{group}\Bug Report"; Filename: "http://sourceforge.net/tracker/?func=add&group_id=23169&atid=377680";

Name: "{userdesktop}\TiEmu"; Filename: "{app}\tiemu.exe"; WorkingDir: "{app}"; MinVersion: 4,4; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\TiEmu"; Filename: "{app}\tiemu.exe"; WorkingDir: "{app}"; MinVersion: 4,4; Tasks: quicklaunchicon

[Run]
; Remove any previously installed PortTalk driver (especially v1.x)
Filename: "{app}\PortTalk\Uninstall.exe"; Parameters: ""; MinVersion: 0,4; Tasks: tlk_drv;
Filename: "{app}\tiemu.exe"; Description: "Launch TiEmu"; StatusMsg: "Running TiEmu..."; Flags: postinstall nowait unchecked
Filename: "{app}\wget\d_and_i.bat"; Description: "Download and install GTK+"; StatusMsg: "Running ..."; Flags: nowait postinstall unchecked hidewizard;

; COM/OLE  registration
Filename: "{app}\tiemu.exe"; Parameters: "/RegServer"; Tasks: com_ole;

[UninstallRun]
; Remove any previously installed PortTalk driver (especially v1.x)
Filename: "{app}\PortTalk\Uninstall.exe"; Parameters: ""; MinVersion: 0,4; Tasks: tlk_drv;

; COM/OLE  un-registration
Filename: "{app}\tiemu.exe"; Parameters: "/UnregServer"; Tasks: com_ole;

[Registry]
; Install the NT PortTalk driver
Root: HKLM; SubKey: "SYSTEM\CurrentControlSet\Services\PortTalk"; ValueType: dword; ValueName: "Type"; ValueData: "1";  MinVersion: 0,4; Tasks: tlk_drv;
Root: HKLM; SubKey: "SYSTEM\CurrentControlSet\Services\PortTalk"; ValueType: dword; ValueName: "Start"; ValueData: "2"; MinVersion: 0,4; Tasks: tlk_drv;
Root: HKLM; SubKey: "SYSTEM\CurrentControlSet\Services\PortTalk"; ValueType: dword; ValueName: "ErrorControl"; ValueData: "1"; MinVersion: 0,4; Tasks: tlk_drv;
Root: HKLM; SubKey: "SYSTEM\CurrentControlSet\Services\PortTalk"; ValueType: string; ValueName: "DisplayName"; ValueData: "PortTalk"; MinVersion: 0,4; Tasks: tlk_drv;
; Install the LPG's SilverLink driver
Root: HKLM; SubKey: "SYSTEM\CurrentControlSet\Services\TiglUsb"; ValueType: dword; ValueName: "Type"; ValueData: "1";  Tasks: slv_drv;
Root: HKLM; SubKey: "SYSTEM\CurrentControlSet\Services\TiglUsb"; ValueType: dword; ValueName: "Start"; ValueData: "3"; Tasks: slv_drv;
Root: HKLM; SubKey: "SYSTEM\CurrentControlSet\Services\TiglUsb"; ValueType: dword; ValueName: "ErrorControl"; ValueData: "1"; Tasks: slv_drv;
Root: HKLM; SubKey: "SYSTEM\CurrentControlSet\Services\TiglUsb"; ValueType: string; ValueName: "DisplayName"; ValueData: "TiglUsb.sys TI-GRAPH / DIRECT LINK USB driver"; Tasks: slv_drv;
Root: HKLM; SubKey: "SYSTEM\CurrentControlSet\Services\TiglUsb"; ValueType: string; ValueName: "ImagePath"; ValueData: "System32\Drivers\TiglUsb.sys"; Tasks: slv_drv;
; Boost GTK2 (WinNT/2000/XP)
Root: HKLM; SubKey: "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"; ValueType: string; ValueName: "PANGO_WIN32_NO_UNISCRIBE"; ValueData: "anything"; MinVersion: 0,4;
; File associations
Root: HKCR; Subkey: ".sav"; ValueType: string; ValueName: ""; ValueData: "TiEmu.Sav";
Root: HKCR; Subkey: "TiEmu.Sav"; ValueType: string; ValueName: ""; ValueData: "TiEmu state";
Root: HKCR; Subkey: "TiEmu.Sav\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\tiemu.exe,0"
Root: HKCR; Subkey: "TiEmu.Sav\shell\open";  ValueType: string; ValueData: "Open with &TiEmu";
Root: HKCR; Subkey: "TiEmu.Sav\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\tiemu.exe"" ""%1""";

Root: HKCR; Subkey: ".rom"; ValueType: string; ValueName: ""; ValueData: "TiEmu.Rom";
Root: HKCR; Subkey: "TiEmu.Rom"; ValueType: string; ValueName: ""; ValueData: "Rom Dump";
Root: HKCR; Subkey: "TiEmu.Rom\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\romdump.ico";
Root: HKCR; Subkey: "TiEmu.Rom\shell\open";  ValueType: string; ValueData: "Open with &TiEmu";
Root: HKCR; Subkey: "TiEmu.Rom\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\tiemu.exe"" ""%1""";

[UninstallDelete]
Type: files; Name: "{app}\tiemu.url"

; OLE Registration
;  ExecWait '"$INSTDIR\bin\tiemu.exe" /RegServer'
;  RegDLL "$INSTDIR\bin\tiemups.dll"
; OLE Unregistration
;  UnregDLL "$INSTDIR\bin\tiemups.dll"
;  ExecWait '"$INSTDIR\bin\tiemu.exe" /UnregServer'

;; Taken from "http://www.dropline.net/gtk/support.php" with some customizations

[Code]
var
  Exists: Boolean;
  GtkPath: String;
  WimpPath: String;
  GtkVersion: String;

function GetGtkInstalled (): Boolean;
begin
  Exists := RegQueryStringValue (HKLM, 'Software\GTK\2.0', 'Path', GtkPath);
  if not Exists then begin
    Exists := RegQueryStringValue (HKCU, 'Software\GTK\2.0', 'Path', GtkPath);
  end;
   Result := Exists
end;

function GetGtkVersionInstalled (): Boolean;
begin
  Exists := RegQueryStringValue (HKLM, 'Software\GTK\2.0', 'Version', GtkVersion);
  if not Exists then begin
    Exists := RegQueryStringValue (HKCU, 'Software\GTK\2.0', 'Version', GtkVersion);
  end;
   Result := Exists
end;

function GetGtkPath (S: String): String;
begin
    Result := GtkPath;
end;

function GetGtkVersion (S: String): String;
begin
    Result := GtkVersion;
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

function DisplayWarning(I: Integer): Boolean;
var
  S: String;
begin
  if(I = 1) then begin
    S := 'The GTK+ libraries are not installed: ';
  end;
  if(I = 2) then begin
    S := 'The GTK+ libraries are installed but the version is old: ';
  end;
  MsgBox(S + 'you will need the GTK+ 2.6.x Runtime Environnement! But, the installer can download and install it for you; simply think to check the box at the last tab/page. Otherwise, you can still download it from the start menu (start menu > programs > tiemu > install gtk+ from the web).', mbError, MB_OK);
end;

function InitializeSetup(): Boolean;
begin
  // Retrieve GTK path
  Result := GetGtkInstalled ();
  if not Result then begin
    DisplayWarning(1);
  end;

  // Retrieve GTK version
  if Result then begin
    Result := GetGtkVersionInstalled ();

    // and check
    if CompareStr(GtkVersion, '2.6.10') < 0 then begin
      DisplayWarning(2);
    end;
  end;

  // Check version of USB driver
  if IsTiglUsbVersion3Mini() then begin
    MsgBox('SilverLink driver v2.x has been removed of your system. Now, TiLP/TiEmu requires v3.x (check out the README for download location).', mbError, MB_OK);
  end;

  // Check for non-NT and WiMP theme
  WimpPath := GtkPath + '\lib\gtk-2.0\2.4.0\engines\libwimp.dll';
  if FileExists(WimpPath) and not UsingWinNT() then begin
        MsgBox('Tip: you are running a non-NT platform with the GTK+ WiMP theme engine installed. If you get a lot of warnings about fonts in console, run the Gtk+ Theme Selector as provided in the start menu group of TiLP/TiEmu', mbError, MB_OK);
  end;

  Result := true;
end;
