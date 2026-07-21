@echo off
setlocal

set "SRC=%~dp0web"
set "DEST=C:\Users\AlexAsh Tiu\OneDrive\Desktop\Programming\tutoringWebsite\simulations\chargelab"

if not exist "%DEST%" mkdir "%DEST%"

copy /Y "%SRC%\index.html" "%DEST%\index.html"
copy /Y "%SRC%\style.css" "%DEST%\style.css"
copy /Y "%SRC%\app.js" "%DEST%\app.js"
copy /Y "%SRC%\chargeLab.js" "%DEST%\chargeLab.js"
copy /Y "%SRC%\chargeLab.wasm" "%DEST%\chargeLab.wasm"

echo.
echo ChargeLab web files deployed to:
echo %DEST%
echo.
pause