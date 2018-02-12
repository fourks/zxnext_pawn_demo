@echo off

REM First you have to manually copy the contents of the pawndemo folder to
REM extras\media\spectrum\esxdos_handler\0.8.6_TBBLUE in your ZEsarUX installation.

REM This batch file needs to know in which directory zesarux.exe is located,
REM e.g. C:\ZEsarUX_win-6.1\. Add this directory to your PATH environment
REM variable or set the ZESARUX_HOME environment variable to point to it.

if not defined ZESARUX_HOME (
  for /f %%a in ('where zesarux.exe') do set ZESARUX_HOME=%%~dpa
)

set DEMODIR=%~dp0

@echo on

cd %ZESARUX_HOME% && zesarux.exe --machine tbblue --enabletimexvideo --enable-mmc --enable-divmmc-ports --mmc-file tbblue.mmc --enable-esxdos-handler --esxdos-root-dir extras\media\spectrum\esxdos_handler\0.8.6_TBBLUE --quickexit --tape %DEMODIR%\pawndemo.tap
