# Introduction
The SDI Control Point GUI is a graphical user interface used to set up and control Iron SDI cameras.

We invite you to visit us at [KAYA Cameras website](www.kayacameras.com). 
Our website offers straightforward access to essential information about KAYA Instruments’ cameras, latest news, about us and how to get in touch with us or our distributors.

# Disclaimer
KAYA Instruments will not provide any warranty for SDI Control Point. It is a free software published under GPLv3 that uses the QT framework by the QT Company and other open source components. 

# Downloads
You can download the latest SDI Control Point compiled binaries for Windows [here](https://cloud.kayainstruments.com/s/SDI).

# Documentation
A user manual is provided [here](https://cloud.kayainstruments.com/s/SDI?path=%2FDocumentation) and also in the doc repository. The user manual thoroughly covers the different screens and menus of SDI Control Point, includes screenshots of each window of the GUI with detailed descriptions on its usage.

# Project Structure
The project contains four folders with multiple sub-directories:

* **application:** The Qt project file for the application as well as the main header, source and ui files  
* **dct_widgets:** A base class for custom widgets and sub-directories for each of widgets used in the application, as well as a dedicated Qt project to [build the widgets stand-alone](#import-custom-widgets-to-the-qt-designer)
* **libraries:** A collection of c libraries used throughout the project
* **ressource:** Ressources like images and icons used in the application
* **Qt Installer Framework (QIF) is used in this project to create an installation package

# License
This software development is based on the Dream Chips ProVideo GUI open source. Please read the software’s license as well as all other third-party software licenses used in SDI Control Point before using this software. Users may redistribute the software under certain conditions as elaborated in the license.

# Working on the Project
## Development Environment
The easiest way to edit the software is by using the Qt Creator, which is available for free for open source development at https://info.qt.io/download-qt-for-application-development    
It is recommended to use the latest version of Qt Creator and the Qt framework. The last tested version is Qt Creator 4.10.0 with Qt 5.13.0.

The master-branch of this project contains the most recent GUI version used for our current line of products. Branches may be created for feature development and new device integration.

Development is done in the *dct_development* branch which will be pushed to the master when new features are ready. Other developers are encouraged to create their own branches and issue merge requests.

The software is based on the Qt framework and provided as a Qt-Project, which can be opened using the Qt Creator.

## Import Custom Widgets to the Qt Designer
**Note:** This step is only needed to make the widgets visible in the designer.

The application uses a lot of custom widgets which can be build as a stand-alone project. This is needed to add them to the widgets available in the Qt Designer.
The integrated Designer of the Qt Creator fetches its plugins from

	```C:\Qt\Tools\QtCreator\bin\plugins\designer```

You might have to restart the Qt Creator to access the new plugins from the Designer.

## Open the Project
Open the application project file "application.pro" in the "application" folder using the "Open Project" dialog from the Qt Designers welcome tab. After configuring the project with the kit needed by your operating system (MinGW 5.3 for Windows, GCC for Linux) open the "Project" tab for the "application" project and open the "Build" settings. Click "Add Build Step", select "Make" and enter "install" as the "Make arguments". This will ensure that additional files placed in the "application/tools_and_condigs" folder which are needed to run the software are copied to the build folder during the build process. Keep in mind that you will have to add this additional build step for every build target separately (e.g. release and debug).

To test whether the import of the custom widgets to the designer has worked, open "mainwindow.ui" in the "Forms" folder. If you see the groups "DCT Basic Widgets" and "DCT Dialog Widgets" in the widget list on the left side of the designer window, the import was successful. You can now start making changes to the software. Please note that you directly edit the widgets from the "application" project, you do not need to use the "dct_widgets" project. As noted above, recompiling the "dct_widgets" project is only needed to add new widgets to the designer.

## Deploy the project on Windows
By default Qt uses dynamic linking, that means all DLLs needed by the application have to be provided with it. To conveniently create a deploy folder that includes the application and DLLs run the script ```<project folder>\application\deploy_qt5.9.1_mingw5.3_win32.bat```. It will copy all needed files to ```<project folder>\application\deploy```.

If you want to use static linking instead (only one executable, no DLLs needed), please take a look at the [steps provided by Qt](https://wiki.qt.io/Building_a_static_Qt_for_Windows_using_MinGW).

