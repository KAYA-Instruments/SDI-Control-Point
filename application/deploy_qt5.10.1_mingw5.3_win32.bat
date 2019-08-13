set

PATH=C:\Qt\5.12.1\mingw730_64\bin;%PATH%
PATH=C:\Qt\Tools\mingw730_64\bin;%PATH%

rem Build application
mkdir build
cd build
qmake ..\application.pro
mingw32-make
cd ..

rem Copy files to deploy folder
mkdir deploy
copy build\release\*.exe deploy
mkdir deploy\tools
copy tools\cygwin1.dll deploy\tools
copy tools\flashloader.exe deploy\tools
mkdir deploy\tools\supported_lenses
copy tools\supported_lenses\Supported_Lenses.txt deploy\tools\supported_lenses

	
rem Run qt deploy to copy needed DLLs
windeployqt deploy

