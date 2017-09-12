# Introduction
The ProVideo GUI is a graphical user interface used to setup Dream Chips ProVideo devices like cameras and color correctors.

The master-branch of this project contains the most recent GUI version used for our current line of products. Branches may be created for special or legacy devices. Below is a list of all branches and the devices that they support, the term given in braces is the internal device codename:

* **master:**
  * ATOM one (xbow) - Small 2k broadcast camera
  * ATOM one Mini (cooper) - Tiny 2k broadcast camera
  * ATOM one 4k (condor4k) - Small 4k broadcast camera
  * ATOM one 4k Mini (condor4k_mini) - Even smaller 4k broadcast camera
  * Cameleon (cameleon) - Image processor box

Development by Dream Chip is done in the *dct_development* branch which will be pushed to the master when new features are ready. Other developers are encouraged to create their own branches and issue merge requests.

The software is based on the Qt framework and provided as a Qt-Project, which can be opened using the Qt Creator.

Below is a short overview of the project files and a guide on how to start working on this project. For more information about the usage and development of this software, take a look at the [wiki pages](https://gitlab.com/dreamchip/provideo-gui/wikis/home).

# Project Structure
The project contains four folders with multiple sub-directories:

* **application:** The Qt project file for the application as well as the main header, source and ui files  
* **dct_widgets:** A base class for custom widgets and sub-directories for each of widgets used in the application, as well as a dedicated Qt project to [build the widgets stand-alone](#import-custom-widgets-to-the-qt-designer)
* **libraries:** A collection of c libraries used throughout the project including [unit tests](#unit-tests-for-c-libraries)
* **ressource:** Ressources like images and icons used in the application

# Working on the Project
## Development Environment
The easiest way to edit the software is by using the Qt Creator, which is available for free for open source development at https://www.qt.io/ide/  
It is recommended to use the latest version of Qt Creator and the Qt framework. The last tested version is Qt Creator 4.4.0 based on Qt 5.9.1.

## Unit Tests for C-Libraries
The unit tests are written using an open source framework called embUnit which is available here: https://sourceforge.net/projects/embunit/  
At the moment the unit tests are not complete and outdated see [Issue #1](dreamchip/provideo-gui#1).

## Import Custom Widgets to the Qt Designer
The application uses a lot of custom widgets which can be build as a stand-alone project. This is needed to add them to the widgets available in the Qt Designer.  
To build the widgets open the Qt Creator and and click "Open Project" on the "Welcome" tab. Navigate to the "dct_widgets" folder in the checked out project folder and open the "dct_widgets.pro" file. Configure the project with the kit needed by your operating system. On Windows this should be Qt 5.9.1 with MinGW 5.3, on Linux it should be Qt 5.9.1 with GCC. Build the project with the release target.

You sould now have a Qt library file called "libdct_widgets.so" in the build folder (e.g. "build-dct_widgets-Desktop_Qt_5_9_1_GCC_64bit-Release"). Copy this file to the plugins folder of the Qt Creator, the default paths are as follows:  
* Windows: ```C:\qt\Tools\QtCreator\lib\Qt\plugins\Designer```
* Linux: ```/opt/Qt/Tools/QtCreator/lib/Qt/plugins/designer/``` (you might have to grant yourself write access to this folder)

Restart the Qt Creator to make the widgets available.

If you want to automatically copy the library file to the designer folder you can add a custom build step. Therefore open the projects tab in Qt Creator, select the "dct_widgets" project from the dropdown menu and klick "Add Build Step -> Custom Process Step". An example for the copy command under Linux would be:

>**Command:** cp  
>**Arguments:** libdct_widgets.so /opt/Qt/Tools/QtCreator/lib/Qt/plugins/designer/libdct_widgets.so  
>**Working directory:** %{buildDir}

**Note:** This step is only needed to make the widgets visible in the designer. Rebuilding and copying the widget library is only needed when new widgets are added to the library, not after every change made to a widget.

## Open the Project
Open the application project file "application.pro" in the "application" folder using the "Open Project" dialog from the Qt Designers welcome tab. After configuring the project with the kit needed by your operating system (MinGW 5.3 for Windows, GCC for Linux) open the "Project" tab for the "application" project and open the "Build" settings. Click "Add Build Step", select "Make" and enter "install" as the "Make arguments". This will ensure that additional files placed in the "application/tools" folder which are needed to run the software are copied to the build folder during the build process. Keep in mind that you will have to add this additional build step for every build target separately (e.g. release and debug).

To test whether the import of the custom widgets to the designer has worked, open "mainwindow.ui" in the "Forms" folder. If you see the groups "DCT Basic Widgets" and "DCT Dialog Widgets" in the widget list on the left side of the designer window, the import was successful. You can now start making changes to the software. Please not that you directly edit the widgets from the "application" project, you do not need to use the "dct_widgets" project. As noted above, recompiling the "dct_widgets" project is only needed to add new widgets to the designer.

## Deploy the project on Windows
By default Qt uses dynamic linking, that means all DLLs needed by the application have to be provided with it. To conveniently create a deploy folder that includes the application and DLLs run the script ```<project folder>\application\deploy_qt5.9.1_mingw5.3_win32.bat```. It will copy all needed files to ```<project folder>\application\deploy```.

If you want to use static linking instead (only one executable, no DLLs needed), please take a look at the [steps provided by Qt](https://wiki.qt.io/Building_a_static_Qt_for_Windows_using_MinGW).

## Cross-Compilation under Linux
It is possible to cross-compile a 32 Bit windows executable under Linux by using the M cross environment (MXE):
How to crosscompile a QT5 project using MXE

1. Get the MXE repo from github:  
   ```# git clone https://github.com/mxe/mxe.git```
2. Build the cross compile toolchain for qt5. This will just build the needed functionality for this project to decrease compile time:  
   ```# cd mxe && make qtbase qtserialport```
3. Go to the application folder of the project (where the application *.pro file lies)  
   ```# cd <project folder>/application/```
4. Add the MXE path to the path variable:  
   ```# export PATH=<mxe root>/usr/bin:$PATH```
5. Run qmake from the MXE toolchain to create the Makefile  
   ```# <mxe root>/usr/i686-w64-mingw32.static/qt5/bin/qmake```
6. Build the application:  
   ```# make```
7. The resulting executable will be placed in ```<project folder>/application/release/ProVideo.exe```. If you want to deploy the executable, do not forget to place the file ```<project folder>/application/tools/flashloader.exe``` in a folder named ```tools``` on the same folder as the executable, otherwise firmware updates will not be possible.

The above steps will build a Win32 static executable. To use dynamic libraries or generate a 64 bit executable you have to use a different MXE_TARGET in step 2:

* MinGW-w64, 32-bit, static libs: ```# make qtbase qtserialport MXE_TARGETS=i686-w64-mingw32.static``` (default)
* MinGW-w64, 32-bit, shared libs: ```# make qtbase qtserialport MXE_TARGETS=i686-w64-mingw32.shared```
* MinGW-w64, 64-bit, static libs: ```# make qtbase qtserialport MXE_TARGETS=x86_64-w64-mingw32.static```
* MinGW-w64, 64-bit, shared libs: ```# make qtbase qtserialport MXE_TARGETS=x86_64-w64-mingw32.shared```

And use the according toolchain-path in step 5.
