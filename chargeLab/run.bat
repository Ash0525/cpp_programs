@echo off
setlocal
set PATH=%CD%\lib;%PATH%
echo Starting ChargeLab...
run.exe
if errorlevel 1 (
    echo.
    echo Error occurred! Press any key to close...
    pause
) else (
    echo Program closed successfully
)
