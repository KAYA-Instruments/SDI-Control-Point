# Introduction
The ProVideo GUI is a graphical user interface used to setup Dream Chips ProVideo devices like cameras and color correctors.

The master-branch of this project contains the most recent GUI version used for our current line of products. Branches may be created for special or legacy devices.  

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
It is recommended to use the latest version of Qt Creator and the Qt framework. The last tested version is Qt Creator 4.3.1 based on Qt 5.9.1.

## Unit Tests for C-Libraries
The unit tests are written using an open source framework called embUnit which is available here: https://sourceforge.net/projects/embunit/  
At the moment the unit tests are not complete and outdated see [Issue #1](dreamchip/provideo-gui#1).

## Import Custom Widgets to the Qt Designer
The application uses a lot of custom widgets which can be build as a stand-alone project. This is needed to add them to the widgets available in the Qt Designer.  
To build the widgets open the Qt Creator and and click "Open Project" on the "Welcome" tab. Navigate to the "dct_widgets" folder in the checked out project folder and open the "dct_widgets.pro" file. Configure the project with the kit needed by your operating system. On Windows this should be Qt 5.9.1 with MinGW 5.3, on Linux it should be Qt 5.9.1 with GCC. Build the project with the release target.

You sould now have a Qt library file called "libdct_widgets.so" in the build folder (e.g. "build-dct_widgets-Desktop_Qt_5_9_1_GCC_64bit-Release"). Copy this file to the plugins folder of the Qt Creator, the default paths are as follows:  
* Windows: ```C:\qt\Tools\QtCreator\lib\Qt\plugins\Designer```
* Linux: ```/opt/Qt/Tools/QtCreator/lib/Qt/plugins/designer/``` (you might have to grant yourself write access to this folder)

If you want to automatically copy the library file to the designer folder you can add a custom build step. Therefore open the projects tab in Qt Creator, select the "dct_widgets" project from the dropdown menu and klick "Add Build Step -> Custom Process Step". An example for the copy command under Linux would be:

>**Command:** cp  
>**Arguments:** libdct_widgets.so /opt/Qt/Tools/QtCreator/lib/Qt/plugins/designer/libdct_widgets.so  
>**Working directory:** %{buildDir}

**Note:** This step is only needed to make the widgets visible in the designer. Rebuilding and copying the widget library is only needed when new widgets are added to the library, not after every change made to a widget.

## Open the Project
Open the application project file "application.pro" in the "application" folder using the "Open Project" dialog from the Qt Designers welcome tab. After configuring the project with the kit needed by your operating system (MinGW 5.3 for Windows, GCC for Linux) open the "Project" tab for the "application" project and open the "Build" settings. Click "Add Build Step", select "Make" and enter "install" as the "Make arguments". This will ensure that additional files placed in the "application/tools" folder which are needed to run the software are copied to the build folder during the build process. Keep in mind that you will have to add this additional build step for every build target separately (e.g. release and debug).

To test whether the import of the custom widgets to the designer has worked, open "mainwindow.ui" in the "Forms" folder. If you see the groups "DCT Basic Widgets" and "DCT Dialog Widgets" in the widget list on the left side of the designer window, the import was successful. You can now start making changes to the software. Please not that you directly edit the widgets from the "application" project, you do not need to use the "dct_widgets" project. As noted above, recompiling the "dct_widgets" project is only needed to add new widgets to the designer.

