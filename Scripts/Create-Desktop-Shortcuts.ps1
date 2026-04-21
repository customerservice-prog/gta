# Creates two shortcuts on the Windows Desktop (Open Unreal + project folder).
$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path -Parent $PSScriptRoot
$bat = Join-Path $projectRoot 'Open-GTAWorld-Local.bat'
if (-not (Test-Path -LiteralPath $bat)) {
    Write-Error "Not found: $bat"
    exit 1
}

$desktop = [Environment]::GetFolderPath('Desktop')
$Wsh = New-Object -ComObject WScript.Shell

$s1 = $Wsh.CreateShortcut((Join-Path $desktop 'GTAWorld - Open in Unreal.lnk'))
$s1.TargetPath = $bat
$s1.WorkingDirectory = $projectRoot
$s1.Description = 'Launch GTAWorld in Unreal Editor (UE 5.4)'
$s1.Save()

$s2 = $Wsh.CreateShortcut((Join-Path $desktop 'GTAWorld project folder.lnk'))
$s2.TargetPath = $projectRoot
$s2.Description = 'Open the GTAWorld project folder'
$s2.Save()

Write-Host "Created on Desktop:"
Write-Host "  - GTAWorld - Open in Unreal.lnk"
Write-Host "  - GTAWorld project folder.lnk"
