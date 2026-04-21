@echo off
set "ROOT=%~dp0"
powershell -NoProfile -ExecutionPolicy Bypass -File "%ROOT%Scripts\Register-UProject-For-Unreal.ps1"
