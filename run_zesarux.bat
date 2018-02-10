@echo off

REM First you have to manually copy the contents of the pawndemo folder to
REM extras\media\spectrum\esxdos_handler\0.8.6_TBBLUE in your ZEsarUX installation.

REM Add the path to zesarux.exe in your PATH environment variable (e.g. C:\ZEsarUX_win-6.1)
REM or set ZESARUX_HOME in this batch file by uncommenting and modifying the example below.

for /f %%a in ('where zesarux.exe') do set ZESARUX_HOME=%%~dpa

REM set ZESARUX_HOME=C:\ZEsarUX_win-6.1

set DEMODIR=%~dp0

@echo on

%ZESARUX_HOME%\zesarux.exe --machine tbblue --enabletimexvideo --enable-mmc --enable-divmmc-ports --mmc-file tbblue.mmc --enable-esxdos-handler --esxdos-root-dir %ZESARUX_HOME%\extras\media\spectrum\esxdos_handler\0.8.6_TBBLUE --quickexit --tape %DEMODIR%\pawndemo.tap
