@echo off
setlocal

set "PWD=%cd%"
set "CMD=docker run -it --rm -v %PWD%:/home/os-fall-2023 os-fall-2023:latest"

echo Creating a container and attaching it to the current shell
%CMD%

endlocal

