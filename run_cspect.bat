@echo off

REM Add the path to CSpect.exe in your PATH environment variable (e.g. C:\cspect) or
REM set CSPECT_HOME in this batch file by uncommenting and modifying the example below.

for /f %%a in ('where CSpect.exe') do set CSPECT_HOME=%%~dpa

REM set CSPECT_HOME=C:\cspect

set DEMODIR=%~dp0

@echo on

%CSPECT_HOME%\CSpect.exe -w2 -zxnext -mmc=%DEMODIR% %DEMODIR%\pawndemo.sna
