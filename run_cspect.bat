@echo off

REM Add the path to CSpect.exe in your PATH environment variable (e.g. C:\cspect)
REM or prepend the path directly in this batch file (e.g. C:\cspect\CSpect.exe).

set DEMODIR=%~dp0

@echo on

CSpect.exe -w2 -zxnext -mmc=%DEMODIR% %DEMODIR%\pawndemo.sna
