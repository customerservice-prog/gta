# Opens GTAWorld.uproject in Unreal Editor after a visible confirmation popup.
$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path -Parent $PSScriptRoot
$uproject = Join-Path $projectRoot 'GTAWorld.uproject'

if (-not (Test-Path -LiteralPath $uproject)) {
    Add-Type -AssemblyName System.Windows.Forms
    [System.Windows.Forms.MessageBox]::Show(
        "Could not find:`n$uproject",
        'GTAWorld',
        [System.Windows.Forms.MessageBoxButtons]::OK,
        [System.Windows.Forms.MessageBoxIcon]::Error
    ) | Out-Null
    exit 1
}

$shell = New-Object -ComObject WScript.Shell
[void]$shell.Popup(
    "Launching Unreal Editor for GTAWorld (UE 5.4)...`n`nIf nothing opens, install Unreal Engine 5.4 from the Epic Games Launcher.",
    0,
    'GTAWorld',
    64
)

Start-Process -FilePath $uproject
