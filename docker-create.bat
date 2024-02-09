@echo off
setlocal

set "CMD=docker build -t os-fall-2023:latest ."

echo Building a docker container
%CMD%

endlocal