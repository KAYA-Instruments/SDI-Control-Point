set

PATH=C:\Qt\5.9.1\mingw53_32\bin;%PATH%
PATH=C:\Qt\Tools\mingw530_32\bin;%PATH%

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

rem Run qt deploy to copy needed DLLs
windeployqt deploy

