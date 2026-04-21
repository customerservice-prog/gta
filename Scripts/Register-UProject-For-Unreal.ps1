# Run elevated once: registers .uproject to open with Epic's Unreal Version Selector.
$ErrorActionPreference = 'Stop'
$selector = Join-Path $env:ProgramFiles 'Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe'
if (-not (Test-Path -LiteralPath $selector)) {
    Write-Host "Not found: $selector"
    Write-Host "Install Epic Games Launcher, then try again."
    Read-Host "Press Enter"
    exit 1
}

$isAdmin = ([Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole(
    [Security.Principal.WindowsBuiltInRole]::Administrator
)
if (-not $isAdmin) {
    $here = $MyInvocation.MyCommand.Path
    Start-Process powershell.exe -Verb RunAs -ArgumentList @(
        '-NoProfile', '-ExecutionPolicy', 'Bypass', '-File', $here
    ) | Out-Null
    exit 0
}

# Bare "%1" with Version Selector triggers "Invalid command line"; /editor is required for CLI opens.
$cmd = ('ftype Unreal.ProjectFile="{0}" /editor "%1"' -f $selector)
cmd /c $cmd
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
cmd /c 'assoc .uproject=Unreal.ProjectFile'
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

Write-Host "OK: double-click .uproject files should open Unreal (not a code editor)."
Read-Host "Press Enter"
