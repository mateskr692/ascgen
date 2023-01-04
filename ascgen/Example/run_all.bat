@echo off
set orgd=%~dp0
set exed=%~dp0..\..\x64\Release\

cd %~dp0\addbin
%exed%ascgen.exe -i _examples.txt

cd %~dp0\convert
%exed%ascgen.exe -i _examples.txt

cd %~dp0\addbcd
%exed%ascgen.exe -i _examples.txt

cd %~dp0\addex3
%exed%ascgen.exe -i _examples.txt

cd %~dp0\mult
%exed%ascgen.exe -i _examples.txt

cd %~dp0\div
%exed%ascgen.exe -i _examples.txt


cd %orgd%