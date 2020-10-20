PATH=%PATH%;C:\Qt\5.12.9\msvc2017\bin
SET TARGETDIR=..\..\..\__DIST\SDIControlPoint\QIF

goto call_binarycreator

xcopy ..\..\application\QIF %TARGETDIR%\ /e

windeployqt --verbose 2 --dir %TARGETDIR%\packages\Application\data SDIControlPoint.exe
copy /b .\SDIControlPoint.exe %TARGETDIR%\packages\Application\data

:call_binarycreator
pushd %TARGETDIR%
C:\Qt\Tools\QtInstallerFramework\3.2\bin\binarycreator.exe -c config\config.xml -p packages SDIControlPoint_Setup.exe
popd
