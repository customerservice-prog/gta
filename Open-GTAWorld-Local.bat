@echo off
set "ROOT=%~dp0"
powershell -NoProfile -ExecutionPolicy Bypass -File "%ROOT%Scripts\Launch-GTAWorld.ps1"
exit /b %ERRORLEVEL%
