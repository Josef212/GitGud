@echo off
pushd ..\
call vendor\premake\bin\premake5.exe vs2019
popd
IF %ERRORLEVEL% NEQ 0 (
    PAUSE
)