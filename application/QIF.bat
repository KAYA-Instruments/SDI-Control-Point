PATH=%PATH%;C:\Qt\5.12.9\msvc2017\bin
SET TARGETDIR=..\..\..\__DIST\SDIControlPoint\QIF

:fill_qif
xcopy ..\..\application\QIF %TARGETDIR%\ /e /y

:call_windeployqt
windeployqt --verbose 2 --dir %TARGETDIR%\packages\Application\data SDIControlPoint.exe
copy /b .\SDIControlPoint.exe %TARGETDIR%\packages\Application\data

copy /b %KAYA_3RD_PARTY_SW_ROOT%\VS2019\vcredist_msvc2019_x86.exe %TARGETDIR%\packages\vcredist\data

:call_binarycreator
pushd %TARGETDIR%
C:\Qt\Tools\QtInstallerFramework\3.2\bin\binarycreator.exe -c config\config.xml -p packages SDIControlPoint_Setup.exe
popd