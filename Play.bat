@echo off
setlocal EnableExtensions
set "ROOT=%~dp0"

if exist "%ROOT%GTAWorld.exe" set "EXE=%ROOT%GTAWorld.exe" & goto run
if exist "%ROOT%WindowsNoEditor\GTAWorld.exe" set "EXE=%ROOT%WindowsNoEditor\GTAWorld.exe" & goto run
if exist "%ROOT%Build\WindowsNoEditor\GTAWorld.exe" set "EXE=%ROOT%Build\WindowsNoEditor\GTAWorld.exe" & goto run

powershell -NoProfile -ExecutionPolicy Bypass -File "%ROOT%Scripts\ShowPlayHelp.ps1"
exit /b 1

:run
start "" "%EXE%"
exit /b 0
