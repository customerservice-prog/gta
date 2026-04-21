@echo off
:: Epic repair when UnrealVersionSelector shows "Invalid command line".
:: Run once; click Yes on UAC. You can also use Epic Launcher: "Fix project files associations" if it offers it.
powershell -NoProfile -ExecutionPolicy Bypass -Command "Start-Process -LiteralPath (Join-Path $env:ProgramFiles 'Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe') -Verb RunAs -ArgumentList '-fileassociations'"
exit /b 0
