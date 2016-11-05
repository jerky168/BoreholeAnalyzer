@echo off

echo Setting up a Qt environment...

set QMAKESPEC=win32-msvc2015
echo -- QMAKESPEC set to "win32-msvc2015"

set QT_DIR=C:\Qt\Qt5.7.0\5.7\msvc2015
echo -- QT_DIR set to %QT_DIR%

set QT_INSTALLER_DIR=C:\Qt\QtIFW2.0.3
echo -- QT_INSTALLER_DIR set to %QT_INSTALLER_DIR%

set VS_DIR="C:\Program Files (x86)\Microsoft Visual Studio 14.0"
call %VS_DIR%\VC\vcvarsall.bat x86

set PATH=%QT_DIR%\bin;%QT_INSTALLER_DIR%\bin;%PATH%
echo -- Added %QT_DIR%\bin;%QT_INSTALLER_DIR%\bin; to PATH

set SRC=%cd%
echo --SRC set to %SRC%

rem 开始编译

cd ..
set CURRENT_TIME="%date:~0,4%-%date:~5,2%-%date:~8,2% %time:~0,2%.%time:~3,2%.%time:~6,2%"
echo --CURRENT_TIME set to %CURRENT_TIME%
mkdir BoreHoleAnalyzer_%CURRENT_TIME%
cd BoreHoleAnalyzer_%CURRENT_TIME%

xcopy %SRC%\dist .\dist /e /i


qmake %SRC%\BoreholeAnalyzer.pro
nmake release /nologo

copy release\BoreholeAnalyzer.exe .\dist\packages\com.ylink.idt\data
del Makefile*
rd /q /s build debug release


cd dist\packages\com.ylink.idt\data
copy %QT_DIR%\bin\Qt5Core.dll .
copy %QT_DIR%\bin\Qt5Gui.dll .
copy %QT_DIR%\bin\Qt5Widgets.dll .
mkdir platforms
cd platforms
copy %QT_DIR%\plugins\platforms\qwindows.dll .
cd ..

cd ..\..\..\..
binarycreator.exe -c dist\config\config.xml -p dist\packages "IDT(M) Installer.exe" -v
rd /q /s dist

