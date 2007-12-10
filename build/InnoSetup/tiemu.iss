; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!
;
; (c) Copyright 2001-2007, the TiEmu team
;
; $Id: tiemu.iss 639 2004-08-19 15:35:33Z roms $

[Setup]
AppName=TiEmu3-gdb
AppVerName=TiEmu 3.02 No Gdb
AppPublisher=The TiEmu Team
AppPublisherURL=http://lpg.ticalc.org/prj_tiemu/index.html
AppSupportURL=http://lpg.ticalc.org/prj_tiemu/mailing_list.html
AppUpdatesURL=http://lpg.ticalc.org/prj_tiemu/win32_download.html
DefaultDirName={pf}\TiEmu3-gdb
DefaultGroupName=TiEmu3-gdb
AllowNoIcons=yes
LicenseFile=C:\sources\roms\tiemu3\COPYING
InfoBeforeFile=C:\sources\roms\tiemu3\README.win32
InfoAfterFile=C:\sources\roms\tiemu3\RELEASE

PrivilegesRequired = admin

;--- Shared Stuffs ---
[Files]
; TI libraries
Source: "C:\sources\roms\tifiles2\tests\libtifiles2-5.dll"; DestDir: "{cf}\LPG Shared\libs"; Flags: sharedfile; BeforeInstall: DeleteDll('libtifiles2-3.dll');
Source: "C:\sources\roms\ticables2\tests\libticables2-1.dll"; DestDir: "{cf}\LPG Shared\libs"; Flags: sharedfile; BeforeInstall: DeleteDll('libticables2-1.dll');
Source: "C:\sources\roms\ticalcs2\tests\libticalcs2-7.dll"; DestDir: "{cf}\LPG Shared\libs"; Flags: sharedfile; BeforeInstall: DeleteDll('libticalcs2-2.dll');
Source: "C:\sources\roms\ticonv\tests\libticonv-3.dll"; DestDir: "{cf}\LPG Shared\libs"; Flags: sharedfile; BeforeInstall: DeleteDll('libticonv-2.dll');

; I18n files
Source: "C:\sources\roms\tifiles2\po\fr.gmo"; DestDir: "{cf}\LPG Shared\libs\locale\fr\LC_MESSAGES"; DestName: "libtifiles2.mo"; Flags: ignoreversion sharedfile;
Source: "C:\sources\roms\ticables2\po\fr.gmo"; DestDir: "{cf}\LPG Shared\libs\locale\fr\LC_MESSAGES"; DestName: "libticables2.mo"; Flags: ignoreversion sharedfile;
Source: "C:\sources\roms\ticalcs2\po\fr.gmo"; DestDir: "{cf}\LPG Shared\libs\locale\fr\LC_MESSAGES"; DestName: "libticalcs2.mo"; Flags: ignoreversion sharedfile;

; Misc
Source: "C:\Gtk2Dev\bin\libxml2.dll"; DestDir: "{cf}\LPG Shared\libs"; Flags: onlyifdoesntexist sharedfile; BeforeInstall: DeleteDll('libxml2.dll');
Source: "C:\Gtk2Dev\bin\libglade-2.0-0.dll"; DestDir: "{cf}\LPG Shared\libs"; Flags: onlyifdoesntexist sharedfile; BeforeInstall: DeleteDll('libglade-2.0-0.dll');

Source: "C:\Gtk2Dev\bin\gtkthemeselector.exe"; DestDir: "{cf}\LPG Shared\bin"; Flags: ignoreversion sharedfile; BeforeInstall: DeleteExe('gtkthemeselector.exe');

; Downloader
Source: "C:\sources\roms\tilp2\build\InnoSetup\wget\*.dll"; DestDir: "{cf}\LPG Shared\wget"; Flags: ignoreversion
Source: "C:\sources\roms\tilp2\build\InnoSetup\wget\wget.exe"; DestDir: "{cf}\LPG Shared\wget"; Flags: ignoreversion
Source: "C:\sources\roms\tilp2\build\InnoSetup\wget\d_and_i.bat"; DestDir: "{cf}\LPG Shared\wget"; Flags: ignoreversion

; DhaHelper driver
Source: "C:\sources\roms\ticables2\src\win32\dha\dhahelper.sys"; DestDir: "{cf}\LPG Shared\drivers\dha"; Flags: sharedfile;
Source: "C:\sources\roms\ticables2\src\win32\dha\dhasetup.exe";  DestDir: "{cf}\LPG Shared\drivers\dha"; Flags: sharedfile;

; RwPorts driver
Source: "C:\sources\roms\ticables2\src\win64\rwp\rwports.sys"; DestDir: "{cf}\LPG Shared\drivers\rwp"; Flags: sharedfile; Check: Is64BitInstallMode
Source: "C:\sources\roms\ticables2\src\win64\rwp\rwpsetup.exe";  DestDir: "{cf}\LPG Shared\drivers\rwp"; Flags: sharedfile; Check: Is64BitInstallMode

; USB driver
Source: "C:\sources\roms\libusb-win32\bin_modified\libusb0.sys"; DestDir: "{cf}\LPG Shared\drivers\usb"; Flags: sharedfile;
;Source: "C:\sources\roms\libusb-win32\bin\*.sys"; DestDir: "{cf}\LPG Shared\drivers\usb"; Flags: sharedfile;
Source: "C:\sources\roms\libusb-win32\bin\*.dll"; DestDir: "{cf}\LPG Shared\drivers\usb"; Flags: sharedfile;
Source: "C:\sources\roms\ticables2\src\win32\usb\*.cat"; DestDir: "{cf}\LPG Shared\drivers\usb"; Flags: sharedfile;
Source: "C:\sources\roms\ticables2\src\win32\usb\*.inf"; DestDir: "{cf}\LPG Shared\drivers\usb"; Flags: sharedfile;
Source: "C:\sources\roms\libusb-win32\bin\libusb0.dll"; DestDir: "{win}\system32"; Flags: replacesameversion restartreplace uninsneveruninstall;
Source: "C:\sources\roms\libusb-win32\bin\libusb0_x64.dll"; DestDir: "{win}\system32"; Flags: replacesameversion restartreplace uninsneveruninstall; Check: Is64BitInstallMode

[Registry]
; Create entries for shared libs (needed by other programs)
Root: HKLM; Subkey: "Software\LPG Shared"; ValueType: string; ValueName: "Path"; ValueData: "{cf}\LPG Shared"
Root: HKLM; Subkey: "Software\LPG Shared"; ValueType: string; ValueName: "DllPath"; ValueData: "{cf}\LPG Shared\libs"
;--- End of Shared Stuffs ---

[Tasks]
Name: "desktopicon"; Description: "Create a &desktop icon"; GroupDescription: "Additional icons:"; MinVersion: 4,4
Name: "quicklaunchicon"; Description: "Create a &Quick Launch icon"; GroupDescription: "Additional icons:"; MinVersion: 4,4; Flags: unchecked
Name: "com_ole"; Description: "Install TiEmuOle object for TIGCC and others"; GroupDescription: "Misc:";
Name: "slv_drv"; Description: "Install USB drivers"; GroupDescription: "Drivers:";
Name: "dha_drv"; Description: "Install BlackLink/Parallel cable for NT/2k/XP"; GroupDescription: "Drivers:"; MinVersion: 0,4

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

; Binaries
Source: "C:\sources\roms\tifiles2\tests\libtifiles2-4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\sources\roms\ticables2\tests\libticables2-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\sources\roms\ticalcs2\tests\libticalcs2-6.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\sources\roms\ticonv\tests\libticonv-3.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\sources\roms\tiemu3\build\msvc\tiemu.exe"; DestDir: "{app}"; DestName: "tiemu.exe"; Flags: ignoreversion
Source: "C:\SDL-1.2\lib\SDL.dll"; DestDir: "{app}"; Flags: ignoreversion

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
Name: "{group}\GTK theme selector"; Filename: "{cf}\LPG Shared\bin\gtkthemeselector.exe";
Name: "{group}\Install GTK+ from web"; Filename: "{app}\wget\d_and_i.bat";
Name: "{group}\Bug Report"; Filename: "http://sourceforge.net/tracker/?func=add&group_id=23169&atid=377680";

Name: "{userdesktop}\TiEmu"; Filename: "{app}\tiemu.exe"; WorkingDir: "{app}"; MinVersion: 4,4; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\TiEmu"; Filename: "{app}\tiemu.exe"; WorkingDir: "{app}"; MinVersion: 4,4; Tasks: quicklaunchicon

[Run]
Filename: "{app}\tiemu.exe"; Description: "Launch TiEmu"; StatusMsg: "Running TiEmu..."; Flags: postinstall nowait unchecked
Filename: "{cf}\LPG Shared\wget\d_and_i.bat"; Description: "Download and install GTK+"; StatusMsg: "Running ..."; Flags: nowait postinstall unchecked hidewizard;
; COM/OLE  registration
Filename: "{app}\tiemu.exe"; Parameters: "/RegServer"; Tasks: com_ole;
; Drivers installation
Filename: "{cf}\LPG Shared\drivers\dha\dhasetup.exe"; Parameters: "install"; MinVersion: 0,4; Tasks: dha_drv; StatusMsg: "Installing DHA driver (this may take few seconds) ..."
Filename: "rundll32"; Parameters: "libusb0.dll,usb_install_driver_np_rundll {cf}\LPG Shared\drivers\usb\silverlk.inf"; Tasks: slv_drv; StatusMsg: "Installing SilverLink driver (this may take few seconds) ..."
Filename: "rundll32"; Parameters: "libusb0.dll,usb_install_driver_np_rundll {cf}\LPG Shared\drivers\usb\titanium.inf"; Tasks: slv_drv; StatusMsg: "Installing Titanium driver (this may take few seconds) ..."
;Filename: "rundll32"; Parameters: "libusb0.dll,usb_install_driver_np_rundll {cf}\LPG Shared\drivers\usb\ti84plus.inf"; Tasks: slv_drv; StatusMsg: "Installing TI84+ driver (this may take few seconds) ..."
;Filename: "rundll32"; Parameters: "libusb0.dll,usb_install_driver_np_rundll {cf}\LPG Shared\drivers\usb\ti84pse.inf"; Tasks: slv_drv; StatusMsg: "Installing TI84+/SE driver (this may take few seconds) ..."
;Filename: "rundll32"; Parameters: "libusb0.dll,usb_install_driver_np_rundll {cf}\LPG Shared\drivers\usb\nspire.inf"; Tasks: slv_drv; StatusMsg: "Installing NSpire driver (this may take few seconds) ..."

[UninstallRun]
; Dha uninstallation
;Filename: "C:\sources\roms\ticables2\src\win32\dha\dhasetup.exe"; Parameters: "remove"; MinVersion: 0,4; Tasks: dha_drv;
; COM/OLE  un-registration
Filename: "{app}\tiemu.exe"; Parameters: "/UnregServer"; Tasks: com_ole;

[Registry]
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

; Add LPG libraries to the tiemu's path
Root: HKLM; Subkey: "Software\Microsoft\Windows\CurrentVersion\App Paths\tiemu.exe"; Flags: uninsdeletekeyifempty
Root: HKLM; Subkey: "Software\Microsoft\Windows\CurrentVersion\App Paths\tiemu.exe"; ValueType: string; ValueData: "{app}\tiemu.exe"; Flags: uninsdeletevalue
Root: HKLM; Subkey: "Software\Microsoft\Windows\CurrentVersion\App Paths\tiemu.exe"; ValueType: string; ValueName: "Path"; ValueData: "{app};{code:GetLpgDllPath}"; Flags: uninsdeletevalue;

[UninstallDelete]
Type: files; Name: "{app}\tiemu.url"

;; Taken from "http://www.dropline.net/gtk/support.php" with some customizations

[Code]
var
  Exists: Boolean;
  GtkPath: String;
  WimpPath: String;
  GtkVersion: String;
  LpgPath: String;

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

function GetLpgDllPath (S: String): String;
begin
  Exists := RegQueryStringValue (HKLM, 'Software\LPG Shared', 'DllPath', LpgPath);
  if not Exists then begin
    Exists := RegQueryStringValue (HKCU, 'Software\LPG Shared', 'DllPath', LpgPath);
  end;
  Result := LpgPath;
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

// Check for previous program presence and uninstall if needed
function CheckUninstall(S: String): Boolean;
var
  uninsexe: String;
  ResultCode: Integer;
  I: Integer;
  L: Integer;
begin
  Exists := RegKeyExists(HKLM, 'Software\Microsoft\Windows\CurrentVersion\Uninstall\' + S + '_is1');
  Result := false;

  if Exists then begin
    if MsgBox('The program need to be uninstalled. Click YES to uninstall it or NO to force installation.', mbConfirmation, MB_YESNO) = IDNO
    then begin
      Result := true
    end
    else begin
      if RegQueryStringValue(HKLM, 'Software\Microsoft\Windows\CurrentVersion\Uninstall\' + S + '_is1', 'UninstallString', uninsexe) then
      begin

        L := Length(uninsexe);
        for I:=1 to L-1
        do begin
          uninsexe[i] := uninsexe[i+1];
        end;
        SetLength(uninsexe, L-2);

        if not Exec(uninsexe, '', '', SW_SHOW, ewWaitUntilTerminated, ResultCode)
        then begin
            Result := false;
        end
        else begin
          if ResultCode <> 0
          then begin
            Result := false;
          end
          else begin
            Result := true;
          end;
        end;
      end;
    end;
  end
  else begin
    Result := true;
  end;
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
  
  // Uninstall before installing new release
  if not CheckUninstall('TiEmu3-gdb') then
    Result := false
  else
    Result := true;
end;

procedure DeleteDll(const FileName: string);
var
  pf: string;
begin
  pf := ExpandConstant('{app}');
  DeleteFile(pf + '\' + Filename);
end;

procedure DeleteExe(const FileName: string);
begin
  DeleteDll(FileName);
end;
