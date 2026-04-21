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

# Prefer the installed 5.4 editor directly (reliable). Version Selector needs /editor; bare path => "Invalid command line".
$editor54 = Join-Path $env:ProgramFiles 'Epic Games\UE_5.4\Engine\Binaries\Win64\UnrealEditor.exe'
$selector = @(
    "${env:ProgramFiles}\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe",
    "${env:ProgramFiles(x86)}\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe"
) | Where-Object { Test-Path -LiteralPath $_ } | Select-Object -First 1

if (Test-Path -LiteralPath $editor54) {
    Start-Process -FilePath $editor54 -ArgumentList $uproject
} elseif ($selector) {
    Start-Process -FilePath $selector -ArgumentList @('/editor', $uproject)
} else {
    Start-Process -FilePath $uproject
}
