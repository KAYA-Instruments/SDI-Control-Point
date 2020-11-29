PATH=%PATH%;%QTHOME%\5.12.9\msvc2017\bin
SET TARGETDIR=..\..\..\__DIST\SDIControlPoint\QIF

:fill_qif
xcopy ..\..\application\QIF\*.* %TARGETDIR% /e /y /i

:call_windeployqt
windeployqt --verbose 2 --dir %TARGETDIR%\packages\Application\data SDIControlPoint.exe
copy /b .\SDIControlPoint.exe %TARGETDIR%\packages\Application\data

copy /b %KAYA_3RD_PARTY_SW_ROOT%\VS2019\vcredist_msvc2019_x86.exe %TARGETDIR%\packages\vcredist\data

:call_binarycreator
pushd %TARGETDIR%
%QTHOME%\Tools\QtInstallerFramework\4.0\bin\binarycreator.exe --offline-only -c config\config.xml -p packages SDIControlPoint_Setup.exe
popd
