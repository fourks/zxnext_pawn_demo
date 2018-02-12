@echo off

REM This batch file needs to know in which directory CSpect.exe is located,
REM e.g. C:\cspect\. Add this directory to your PATH environment variable
REM or set the CSPECT_HOME environment variable to point to it.

if not defined CSPECT_HOME (
  for /f %%a in ('where CSpect.exe') do set CSPECT_HOME=%%~dpa
)

set DEMODIR=%~dp0

@echo on

%CSPECT_HOME%\CSpect.exe -w2 -zxnext -mmc=%DEMODIR% %DEMODIR%\pawndemo.sna
