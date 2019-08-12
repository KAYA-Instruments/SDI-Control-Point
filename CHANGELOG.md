# Provideo GUI Changelog

All releases are tagged in Git with their version number.

## Newest Release:

Version **1.1.15** - 02.08.2019

* Added new tab for the lens drive. The new features ar available for the Atom One and the Atom 4k mini cameras.
* Added lens drive Inteface and UI elements, to configure a lens drive.
* 
* Updated Handbook

## Older Releases:

Version **1.1.14** - 08.07.2019

* Added new UI elements to setup the audio gain from the inout tab.
* Disable genlock controls when genlock is in master mode.
* Replaced functions which are deprecated in Qt 5.13 to compile without warnings.
* Fixed a bug where the genlock offset would be changed by two lines instead of one when the increase or decrease button was pressed. This was due to the auto repeat threshold of the spin box being to short.

Version **1.1.13** - 17.06.2019

* The "LOG" checkbox on the inout tab has been replaced with a combo box. The user can now select between "Off", "HLG" and "PQ".
* Added new UI elements to setup the "max PQ brightness" from the inout tab. The elements are only active when the log mode is set to "PQ".
* The advanced color conversion features in the out tab are now only shown in engineering mode.

Version **1.1.12** - 29.05.2019

* The RS485 termination can now be setup from the settings dialog.
* When reopening the connect dialog it will now automatically search for new com ports. This fixes issues where com ports that are non existent are still in the list which causes all kinds of unexpected behaviour, in the worst case an application crash.

Version **1.1.11** - 23.04.2019

* Added genlock crosslock controls.
* Fixed an issue where the text of the debug terminal would not be "scrolled down" after changing it's size.
* Experimental: Added support for lens control. This is not officially supported by the cameras yet and will be enabled with future releases.
* Fixed a few more Clang warnings (this is an ongoing task)

Version **1.1.10** - 10.12.2018

* Fixed an issue where the "LOG mode" would not be saved (restored) by the save (load) settings function.

Version **1.1.9** - 16.11.2018

* Fixed a bug where an illegal device list would be passed to the connect dialog
* Fixed a rare segmentation fault when reconnecting to a device
* Added a periodic connection check. The GUI will now check every 2 seconds if the connected device is still available. If a device is disconnected the GUI offers to open the connect dialog. This setting is disabled by default, it can be enabled in the Settings Dialog
* The Engineering Mode, Debug Terminal and the new periodic Connection Check are now stored in the applications .ini file in the users home folder (like the connection settings of the Connection Dialog). This ensures that the GUI opens with the same config like it was closed by the user
* Added Options to set the new "LOG mode" setting. The in-out-box now has a check box toggle LOG mode. In LOG mode the gamma curve settings in the lookup table tab are disabled
* Fixed a few more Clang warnings (this is an ongoing task)

Version **1.1.8** - 02.10.2018

* Updated QCustomPlot library to version 2.0.1 which officially supports Qt 5.11
* The runtime counter in the info tab will now be automatically updated every second (like the temperature values)
* Fixed more Clang warnings (this is an ongoing task)
* Fixed some potential memory leaks reported by Cppcheck

Version **1.1.7** - 26.09.2018

* The maximum temperature readout command now reads both the persistent and the user reset-able maximum temperature
* Added readout of current fan speed and added a spin box to setup the target temperature of the fan control for supported devices
* Fixed a rare crash that occured when a device reported a device name that did contain non-ASCII characters
* The info tab now has a timer to update the temperature / fan values every second. This is only done when the info tab is visible. To make sure no errors occur due to this automatic update, the com channel now has a lock on the read / write functions to make it thread safe
* Fixed a bug where the automatic detection would detect devices double and / or with incomplete names
* Fixed potential bugs where the same could have happened in dpc and and aec weight transmision
* Fixed potential bugs where the timeout for dpc and aec weight transmision could be shorter than 1s (since the timer only had 1 second resolution)
* Made the device dropdown field expanding to make device names better readable
* Fixed some Clang warnings (this is an ongoing task)

Version **1.1.6** - 14.08.2018

* Reduced maximum wait time for a reconnect after a device update from 2 minutes to 30 seconds
* Added drag and drop to the update box, the user can now drag a binary file from his file explorer and drop it anywhere on the update box tab to load the file
* Improved the way a connection is established with devices making it more robust which should lead to less errors when loading the device settings
* Small improvements to the update box and the connect dialog
* Fixed the occurence of multiple "device not locked" messages when genlock is enabled on the device but no genlock signal is present

Version **1.1.5** - 28.06.2018

* The update box is now more consistent in its behaviour when connecting to a new device, during device reboot and when an empty folder is selected
* When closing the Connect Dialog the GUI does no longer close completely. This is only necesarry, when the GUI is not connected to a device, if that is the case a message will pop up
* Added Scrollbars to the GUI which are automatically shown when the screen size is too small to fit the GUI completely
* Made autodetection and device connection more robust
* Fixed a bug where the GUI could crash while downloading a new device firmware from the server
* The update box now does not delete an update after it was applied, this makes it more easy to flash the same update to multiple devices
* Fixed a bug where the firmware update could not be restarted after failure
* Changed display of the runtime counter in the info box, it now also displays the days (hours are not enough)

Version **1.1.4** - 08.04.2018

* Added a dialog to the in-out-box to change the weights of the auto exposure control (new AEC feature)
* Added display of the shut down temperature to the info box (requires latest device firmware)
* Fixed a bug where the "verify" checkbox in the update-box could be checked / unchecked during a running flash procedure
* Fixed a bug where the command history of the debug terminal would jump to the wrong history entry after a new command was send
* Fixed a bug where the GUI would not correctly reconnect after updating the device firmware on Windows

Version **1.1.3** - 02.04.2018

* Added a new Debug Terminal which can be used to monitor the commands send by the GUI and manually send commands to the device, the Debug Terminal can be opened from the Settings Dialog
* Updated the QCustomPlot version from 1.3.2 to 2.0.0
* Small bug fixes to DPCC and MCC commands
* Added combo boxes to the in-out-tab to setup the new downscale feature of the ATOM one 4K mini

Version **1.1.2** - This release was skipped and not officially released to public

* Improved update downloader which is now more robust
* Small changes to the Settings Dialog
* Added combo boxes and "+ / -" buttons to switch between common used values for exposure time and ISO value to the in-out-box

Version **1.1.1** - 20.03.2018

* The timeout for detecting devices using the identfy command is now variable, depending on the baudrate of the COM port
* Fixed a bug where the resync of the auto exposure parameters could be triggered multiple times after switching between different devices
* Fixed a bug in the MCC widget that made the programm crash when no phase was selected and the slider ranges were changed
* Fixed a bug in the update widget where the abort button had to be pressed twice to cancel an update
* Added a dialog to the update widget which asks the user to re-install an update if the server version is equal to or older than the device version

Version **1.1.0** - 07.03.2018

* Added a slider to change the maximum allowed gain that can be used by the auto exposure
* Fixed bugs where settings where not saved correctly
* The GUI will now save the settings for all outputs
* Removed support for Cameleon devices
* Added new info dialog which is displayed when the GUI waits for a process to finish
* Added new device scan mechanism which is a lot faster than the old one but needs devices with an updated firmware. The old mechanism is still available after the new scan failed
* Added an online update function to the update tab where device updates can be downloaded and installed automatically
* Added an online check for a new GUI version, but the user has to download the new version manually
* Added device temperature statistics to the info tab
* Removed deprecated BT.601 and SMPTE 240M color conversion matrix presets from the out tab, added BT.2020 matrix preset for UHD and HDR content

Version **1.0.11** - 26.01.2018

* Fixed a bug where the device ID was not correctly displayed in the info tab (the third and fourth id field were identical)
* Fixed a bug where the settings dialog could trigger events like setting the device name multiple times if the GUI had been reconnected before
* Fixed a bug where load / store of the device settings did not work because the device name did not match. We now check whether the platform ID matches

Version **1.0.10** - 11.01.2018

* Fixed a bug where a firmware update file for the software would overwrite the FPGA bitstream (only occured for debug builds of the software)
* The update tab now has an additional field which contains information on the type of update which is currently being flashed
* Some of the UI elements in the update tab have been renamed to make their meaning and usage more clear
* ATOM one mini devices (cooper) now support horizontal image flip

Version **1.0.9** - 15.12.2017

* GUI now fully supports the new ATOM one mini (cooper)
* Resync after firmware updates is now more stable
* Moved system settings from info page to new dialog which can be accessed from the main window
* The device name can now be changed from the new settins dialog
* The lut box now works with variable lut bit-widths

Version **1.0.8** - 07.11.2017

* Use new flashloader tool with automatic baudrate detection
* In case of a genlock error after changing genlock settings, an error message is displayed
* Implemented a "fast gamma" knob which controls the new fast gamma function which allows easy gamma changes on the fly

Version **1.0.7** - 06.10.2017

* Fixed crash during device detection on Windows 10 based systems
* Fixed "DPCC table invalid" message when switching between devices with different resolutions
* Fixed a bug that caused the "dct_widgets" project to be not compile-able on Windows
* Minor UI changes (typos, label arrangement, added link to repository in Info Tab)

Version **1.0.6** - 25.09.2017

* Started implementing a new settings dialog which will contain system wide settings which are at the moment placed on the info tab (not finished yet!)
* Fixed a bug that made firmware updates impossible since cooper device support was added
* The ISO value of the devices which is shown in the In-Out-Tab is now correctly converted from gain to ISO. This requires up to date devices due to changes in the "cam_info" command which now additionally reports the ISO of the device at gain 1.

Version **1.0.5** - 08.09.2017

* Changes to support MXE crosscompile toolchain under Linux: MXE had problems with the clock_gettime functions, so they were replaced under _WIN32 targets.
* Using new flashloader applications, the windows application does not need cygwin1.dll anymore and they got reduced CPU usage
* Fix for devices being recognized as not being identical in broadcast mode (incorrectly checked for name, not for platform)
* Added support for ATOM one Mini (cooper) device

Version **1.0.4** - 24.08.2017

* Fixed a bug where the GUI would request a deletion of the DPCC table on video mode change, although the resolution of the new video mode was identical to the old one
* Fixed a bug where the GUI would crash when no Com-Port was available

Version **1.0.3** - 16.08.2017

* Fixed an out of memory error by increasing communication buffer size
* Increased dpcc table loading timeout
* RAW 10 and 12 video modes were swapped
* MCC Sliders in the MCC Eq Box can now be reset by double clicking them
* Min and Max values of the MCC Sliders are now displayed in the MCC Eq Box
* Added button to execute "dpc_auto_load" command from the DPCC Box
* Fix for DPCC coordinates: Valid range is now 0 to resolution-1
* Added "reset to defaults" button for master black and flare level settings

Version **1.0.2** - 21.06.2017

* Support lens shading correction on the In/Out Tab
* Changes to support new device name feature
* Changes to the DPC tab for easier usage and new test mode functionality
* Minor changes to the info tab (now shows release / build date)

Version **1.0.0** - Date 01.06.2017

* Made changes to support negative genlock offsets
* Added GUI version number in info tab

## Previous Changes:

09.05.2017

* Added device auto detection and device switcher
* Added RS485 broadcast mode
* Fixed Update tab

10.04.2017

* New MCC control widget (MCC Equalizer)

01.02.2017

* More options for AWB handling
* New settings tab for defect pixel detection configuration
