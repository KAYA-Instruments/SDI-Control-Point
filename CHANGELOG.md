# Provideo GUI Changelog

All releases are tagged in Git with their version number.

## Newest Release:

Version **1.1.0** - 02.03.2018

* Added a slider to change the maximum allowed gain that can be used by the auto exposure
* Fixed bugs where settings where not saved correctly
* The GUI will now save the settings for all outputs
* Removed support for Cameleon devices
* Added new info dialog which is displayed when the GUI waits for a process to finish
* Added new device scan mechanism which is a lot faster than the old one but needs devices with an updated firmware. The old mechanism is still available after the new scan failed
* Added an online update function to the update tab where device updates can be downloaded and installed automatically
* Added an online check for a new GUI version, but the user has to download the new version manually
* Added device temperature statistics to the info tab

## Older Releases:

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
