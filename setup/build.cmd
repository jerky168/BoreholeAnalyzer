@echo off

echo Setting up a Qt environment...
echo -- QMAKESPEC set to "win32-msvc2015"

set QMAKESPEC=win32-msvc2015
if exist env.cmd (
	echo "environment set, using env.cmd."
	call env.cmd 
) else (
	echo "environment not set, using default."
	set QT_DIR="C:\Qt\Qt5.7.0\5.7\win32-msvc2015"
	set QT_INSTALLER_DIR="C:\Qt\QtIFW2.0.3"
	set VS_DIR="C:\Program Files (x86)\Microsoft Visual Studio 14.0"
)

echo -- QT_DIR set to %QT_DIR%
echo -- QT_INSTALLER_DIR set to %QT_INSTALLER_DIR%
echo -- VS_DIR set to %VS_DIR%

set PATH=%QT_DIR%\bin;%QT_INSTALLER_DIR%\bin;%PATH%
echo -- Added %QT_DIR%\bin;%QT_INSTALLER_DIR%\bin; to PATH


call %VS_DIR%\VC\vcvarsall.bat x86

set SRC=%cd%\..\src
echo --SRC set to %SRC%

set CURRENT_TIME="%date:~0,4%-%date:~5,2%-%date:~8,2% %time:~0,2%.%time:~3,2%.%time:~6,2%"
echo --CURRENT_TIME set to %CURRENT_TIME%
mkdir BoreHoleAnalyzer_%CURRENT_TIME%
cd BoreHoleAnalyzer_%CURRENT_TIME%

xcopy %SRC%\..\dist .\dist /e /i


qmake %SRC%\BoreholeAnalyzer.pro
nmake release /nologo

copy release\BoreholeAnalyzer.exe .\dist\packages\com.ylink.idt\data

cd dist\packages\com.ylink.idt\data
windeployqt BoreholeAnalyzer.exe

cd ..\..\..\..
binarycreator.exe -c dist\config\config.xml -p dist\packages "IDT(M) Installer.exe" -v
