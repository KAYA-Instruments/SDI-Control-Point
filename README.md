# Introduction
The ProVideo GUI is a graphical user interface used to setup Dream Chips ProVideo devices like cameras and color correctors.

The master-branch of this project contains the most recent GUI version used for our current line of products. Branches may be created for special or legacy devices.

The software is based on the Qt framework and provided as a Qt-Project, which can be opened using the Qt Creator.

# Project Structure
The project contains four folders with multiple sub-directories:
* **application**: The Qt project file for the application as well as the main header, source and ui files
* **dct_widgets**: A base class for custom widgets and sub-directories for each of widgets used in the application, as well as a dedicated Qt project to [build the widgets stand-alone](#import-custom-widgets-to-the-qt-designer)
* **libraries**: A collection of c libraries used throughout the project including [unit tests](#unit-tests-for-c-libraries)
* **ressource**: Ressources like images and icons used in the application

# Working on the Project
## Development Environment
The easiest way to edit the software is by using the Qt Creator, which is available for free for open source development at https://www.qt.io/ide/

## Unit Tests for C-Libraries
The unit tests are written using an open source framework called embUnit which is available here: https://sourceforge.net/projects/embunit/

## Opening the Project
After checking out the project and installing Qt Creator, simply open Qt Creator and click "Open Project" on the "Welcome" tab. Navigate to the "application" folder in the checked out project folder and open the "application.pro" file. The Creator will now switch to the "Edit" tab and you can start editing the source code. 

## Import Custom Widgets to the Qt Designer

