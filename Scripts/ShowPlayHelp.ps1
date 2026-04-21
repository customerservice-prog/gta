# Shown when Play.bat finds no packaged GTAWorld.exe
Add-Type -AssemblyName System.Windows.Forms
$text = @"
No GTAWorld.exe was found next to Play.bat.

This repo is Unreal ENGINE SOURCE, not a ready-made .exe. Unreal games do not run with "nothing installed" unless someone builds a packaged game once.

What actually works:
  1) Install the free Epic Games Launcher, add Unreal Engine 5.4.
  2) Double-click GTAWorld.uproject, wait for compile, press Play in the editor.

Optional later: In Unreal use File -> Package Project; put GTAWorld.exe beside Play.bat (or in WindowsNoEditor) — then Play.bat will launch it.

Open the Epic Games download page now?
"@
$r = [System.Windows.Forms.MessageBox]::Show(
    $text,
    'GTAWorld',
    [System.Windows.Forms.MessageBoxButtons]::YesNo,
    [System.Windows.Forms.MessageBoxIcon]::Information
)
if ($r -eq [System.Windows.Forms.DialogResult]::Yes) {
    Start-Process 'https://store.epicgames.com/en-US/download'
}
