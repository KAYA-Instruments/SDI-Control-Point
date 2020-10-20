PATH=%PATH%;C:\Qt\5.12.9\msvc2017_64\bin
SET TARGETDIR=..\..\..\__DIST\SDIControlPoint\windeployqt
windeployqt --verbose 2 --dir %TARGETDIR% SDIControlPoint.exe
copy /b .\SDIControlPoint.exe %TARGETDIR%

