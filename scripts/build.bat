@echo off
setlocal

cd /d "%~dp0.."

where make >NUL 2>NUL
if not errorlevel 1 (
    make %*
    exit /b %errorlevel%
)

where mingw32-make >NUL 2>NUL
if not errorlevel 1 (
    mingw32-make %*
    exit /b %errorlevel%
)

echo Could not find make or mingw32-make on PATH.
exit /b 1
