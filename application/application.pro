# This version number is shown in the Application, it should be
# updated before each release!
VERSION = 2021.1
#DEFINES += VERSION #= \\\"2021.1\\\"

QMAKE_TARGET_COMPANY = "KAYA Instruments"
QMAKE_TARGET_PRODUCT = "SDI Control Point"
QMAKE_TARGET_DESCRIPTION = "GUI for controlling KAYA SDI cameras"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2020, KAYA Instruments"

RC_ICONS = ../resource/icons/flaticon.com-camera/004-camera-2_128.ico

CONFIG  += c++11
QT      += core serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SDIControlPoint
TEMPLATE = app

# Installation setup defines
TARGET_EXT = .exe
INSTALLER_NAME = SDIControlPoint_Setup_$$VERSION
QIF_DESTDIR = ../__DIST/QIF
QIF_DIR = $${PWD}/QIF
DOCS_DIR = ../doc
ICONS_DIR = ../resource/icons/dir_icon
KAYA_VS2019_PATH = $$(KAYA_3RD_PARTY_SW_ROOT)\VS2019\vcredist_msvc2019_x86.exe
WIN_DEPLOY_QT_PATH = $$(QTHOME)\5.12.9\msvc2017\bin
BINARY_CREATOR_PATH = $$(QTHOME)\Tools\QtInstallerFramework\4.0\bin

# Platform specific tweaks
install_binaries.path = $$OUT_PWD/tools_and_configs
install_binaries.files = ""
unix {
    FLASH_LOADER_APPLICATION="flashloader"
    install_binaries.files += ./tools_and_configs/flashloader
    install_binaries.files += ./tools_and_configs/SupportedLenses.txt
#    XCOPY_COMMAND = cp -r -a -n
}
win32 {
    FLASH_LOADER_APPLICATION="flashloader.exe"
    install_binaries.files += ./tools_and_configs/flashloader.exe
    install_binaries.files += ./tools_and_configs/SupportedLenses.txt
    XCOPY_COMMAND = xcopy /i /e /y /j /d
    DEPLOY_COMMAND = $${WIN_DEPLOY_QT_PATH}\windeployqt
}
osx {
    QMAKE_INFO_PLIST = osx/Info.plist
}

CONFIG( debug, debug|release ) {
    # debug
    TARGET_FULL_PATH = $$shell_quote($$shell_path($${OUT_PWD}/debug/$${TARGET}$${TARGET_EXT}))
} else {
    # release
    TARGET_FULL_PATH = $$shell_quote($$shell_path($${OUT_PWD}/release/$${TARGET}$${TARGET_EXT}))
}

# Copy needed binary files during make install step
INSTALLS += install_binaries

# Copy files to __DIST folder for installation
for(FILE,QIF_DIR){
            win32: FILE ~= s,/,\\,g
            QMAKE_POST_LINK += '$${XCOPY_COMMAND} "$${FILE}" "$${QIF_DESTDIR}" $$escape_expand(\n\t)'
}

for(FILE,DOCS_DIR){
            win32: FILE ~= s,/,\\,g
            QMAKE_POST_LINK += '$${XCOPY_COMMAND} "$${FILE}\*.pdf" "$${QIF_DESTDIR}/packages/Application/data/doc" $$escape_expand(\n\t)'
}

for(FILE,ICONS_DIR){
            win32: FILE ~= s,/,\\,g
            QMAKE_POST_LINK += '$${XCOPY_COMMAND} "$${FILE}\*.ico" "$${QIF_DESTDIR}/packages/Application/data/icons" $$escape_expand(\n\t)'
}

QMAKE_POST_LINK += 'copy /b "$${TARGET_FULL_PATH}" "$${QIF_DESTDIR}/packages/Application/data" $$escape_expand(\n\t)'

# Call windeployqt
APP_DESTDIR = $${QIF_DESTDIR}/packages/Application/data
QMAKE_POST_LINK += 'copy /b "$${TARGET_FULL_PATH}" "$${QIF_DESTDIR}/packages/Application/data" $$escape_expand(\n\t)'
QMAKE_POST_LINK += '$${DEPLOY_COMMAND} --verbose 2 $${APP_DESTDIR}/$${TARGET}$${TARGET_EXT} $$escape_expand(\n\t)'

# Call binary creator
QMAKE_POST_LINK += 'copy /b "$${KAYA_VS2019_PATH}" "$${QIF_DESTDIR}/packages/vcredist/data" $$escape_expand(\n\t)'
QMAKE_POST_LINK += '$${BINARY_CREATOR_PATH}/binarycreator -f -c $${QIF_DESTDIR}/config/config.xml -p $${QIF_DESTDIR}/packages ../__DIST/$${INSTALLER_NAME}$${TARGET_EXT}'

DEFINES += FLASH_LOADER_APPLICATION=\\\"$$FLASH_LOADER_APPLICATION\\\"
DEFINES += "QT_NO_PRINTER"
DEFINES += "QT_NO_SVG"
DEFINES += "QT_NO_OPENGL"
DEFINES += "CFG_CSIP_SAMPLE_INT32"

INCLUDEPATH += ../libraries/include ..              \
               ../libraries/qcustomplot/            \
               ../dct_widgets                       \
               ../dct_widgets/com_ctrl              \
               ../dct_widgets/com_ctrl/devices      \
               ../dct_widgets/csvwrapper            \
               ../dct_widgets/connectdialog         \
               ../dct_widgets/settingsdialog        \
               ../dct_widgets/infodialog            \
               ../dct_widgets/aecweightsdialog      \
               ../dct_widgets/btnarraybox           \
               ../dct_widgets/singlechannelknobbox  \
               ../dct_widgets/triplechannelknobbox  \
               ../dct_widgets/fmtspinbox            \
               ../dct_widgets/skinneddial           \
               ../dct_widgets/piesegmentselect      \
               ../dct_widgets/huesegmentselect      \
               ../dct_widgets/mccslider             \
               ../dct_widgets/mcceqbox              \
               ../dct_widgets/wbbox                 \
               ../dct_widgets/infobox               \
               ../dct_widgets/blacklevelbox         \
               ../dct_widgets/lutbox                \
               ../dct_widgets/inoutbox              \
               ../dct_widgets/outbox                \
               ../dct_widgets/fltbox                \
               ../dct_widgets/updatebox             \
               ../dct_widgets/kneebox               \
               ../dct_widgets/roibox                \
               ../dct_widgets/dpccbox               \
               ../dct_widgets/lensdriverbox         \
               ../dct_widgets/textviewer            \
               ../dct_widgets/debugterminal

SOURCES += ../dct_widgets/mcceqbox/mcceqbox.cpp                             \
           ../dct_widgets/com_ctrl/devices/IronSDI_Device.cpp               \
           ../dct_widgets/roibox/roibox.cpp                                 \
           ../dct_widgets/wbbox/wbbox.cpp                                   \
           ../dct_widgets/infobox/infobox.cpp                               \
           ../dct_widgets/blacklevelbox/blacklevelbox.cpp                   \
           ../dct_widgets/lutbox/lutbox.cpp                                 \
           ../dct_widgets/lutbox/cubic_interpolation.cpp                    \
           ../dct_widgets/inoutbox/inoutbox.cpp                             \
           ../dct_widgets/outbox/outbox.cpp                                 \
           ../dct_widgets/updatebox/updatebox.cpp                           \
           ../dct_widgets/updatebox/flashloader.cpp                         \
           ../dct_widgets/fltbox/fltbox.cpp                                 \
           ../dct_widgets/kneebox/kneebox.cpp                               \
           ../dct_widgets/kneebox/knee_interpolation.cpp                    \
           ../dct_widgets/dpccbox/dpccbox.cpp                               \
           ../dct_widgets/lensdriverbox/lensdriverbox.cpp                   \
           ../dct_widgets/connectdialog/connectdialog.cpp                   \
           ../dct_widgets/settingsdialog/settingsdialog.cpp                 \
           ../dct_widgets/infodialog/infodialog.cpp                         \
           ../dct_widgets/aecweightsdialog/aecweightsdialog.cpp             \
           ../dct_widgets/btnarraybox/btnarraybox.cpp                       \
           ../dct_widgets/singlechannelknobbox/singlechannelknobbox.cpp     \
           ../dct_widgets/triplechannelknobbox/triplechannelknobbox.cpp     \
           ../dct_widgets/fmtspinbox/fmtspinbox.cpp                         \
           ../dct_widgets/skinneddial/skinneddial.cpp                       \
           ../dct_widgets/piesegmentselect/piesegmentselect.cpp             \
           ../dct_widgets/huesegmentselect/huesegmentselect.cpp             \
           ../dct_widgets/mccslider/mccslider.cpp                           \
           ../dct_widgets/com_ctrl/common.cpp                               \
           ../dct_widgets/com_ctrl/defines.cpp                              \
           ../dct_widgets/com_ctrl/ComProtocol.cpp                          \
           ../dct_widgets/com_ctrl/ProVideoProtocol.cpp                     \
           ../dct_widgets/com_ctrl/ComChannel.cpp                           \
           ../dct_widgets/com_ctrl/ComChannelRSxxx.cpp                      \
           ../dct_widgets/com_ctrl/ProVideoSystemItf.cpp                    \
           ../dct_widgets/com_ctrl/IspItf.cpp                               \
           ../dct_widgets/com_ctrl/CprocItf.cpp                             \
           ../dct_widgets/com_ctrl/AutoItf.cpp                              \
           ../dct_widgets/com_ctrl/CamItf.cpp                               \
           ../dct_widgets/com_ctrl/MccItf.cpp                               \
           ../dct_widgets/com_ctrl/LutItf.cpp                               \
           ../dct_widgets/com_ctrl/ChainItf.cpp                             \
           ../dct_widgets/com_ctrl/TfltItf.cpp                              \
           ../dct_widgets/com_ctrl/FpncItf.cpp                              \
           ../dct_widgets/com_ctrl/FpncData.cpp                             \
           ../dct_widgets/com_ctrl/PlayBackItf.cpp                          \
           ../dct_widgets/com_ctrl/IrisItf.cpp                              \
           ../dct_widgets/com_ctrl/KneeItf.cpp                              \
           ../dct_widgets/com_ctrl/ROIItf.cpp                               \
           ../dct_widgets/com_ctrl/DpccItf.cpp                              \
           ../dct_widgets/com_ctrl/OsdItf.cpp                               \
           ../dct_widgets/com_ctrl/LensItf.cpp                              \
           ../dct_widgets/com_ctrl/devices/ProVideoDevice.cpp               \
           ../dct_widgets/com_ctrl/devices/XbowDevice.cpp                   \
           ../dct_widgets/com_ctrl/devices/Condor4kDevice.cpp               \
           ../dct_widgets/com_ctrl/devices/CooperDevice.cpp                 \
           ../dct_widgets/csvwrapper/csvwrapper.cpp                         \
           ../dct_widgets/textviewer/textviewer.cpp                         \
           ../dct_widgets/debugterminal/debugterminal.cpp                   \
           ../libraries/ctrl_channel/ctrl_channel.c                         \
           ../libraries/ctrl_protocol/ctrl_protocol.c                       \
           ../libraries/ctrl_protocol/ctrl_protocol_isp.c                   \
           ../libraries/ctrl_protocol/ctrl_protocol_system.c                \
           ../libraries/ctrl_protocol/ctrl_protocol_cproc.c                 \
           ../libraries/ctrl_protocol/ctrl_protocol_auto.c                  \
           ../libraries/ctrl_protocol/ctrl_protocol_chain.c                 \
           ../libraries/ctrl_protocol/ctrl_protocol_cam.c                   \
           ../libraries/ctrl_protocol/ctrl_protocol_mcc.c                   \
           ../libraries/ctrl_protocol/ctrl_protocol_lut.c                   \
           ../libraries/ctrl_protocol/ctrl_protocol_tflt.c                  \
           ../libraries/ctrl_protocol/ctrl_protocol_fpnc.c                  \
           ../libraries/ctrl_protocol/ctrl_protocol_playback.c              \
           ../libraries/ctrl_protocol/ctrl_protocol_osd.c                   \
           ../libraries/ctrl_protocol/ctrl_protocol_iris.c                  \
           ../libraries/ctrl_protocol/ctrl_protocol_knee.c                  \
           ../libraries/ctrl_protocol/ctrl_protocol_roi.c                   \
           ../libraries/ctrl_protocol/ctrl_protocol_dpcc.c                  \
           ../libraries/ctrl_protocol/ctrl_protocol_lens.c                  \
           ../libraries/provideo_protocol/provideo_protocol.c               \
           ../libraries/provideo_protocol/provideo_protocol_common.cpp      \
           ../libraries/provideo_protocol/provideo_protocol_system.c        \
           ../libraries/provideo_protocol/provideo_protocol_isp.c           \
           ../libraries/provideo_protocol/provideo_protocol_cproc.c         \
           ../libraries/provideo_protocol/provideo_protocol_auto.c          \
           ../libraries/provideo_protocol/provideo_protocol_chain.c         \
           ../libraries/provideo_protocol/provideo_protocol_cam.c           \
           ../libraries/provideo_protocol/provideo_protocol_mcc.c           \
           ../libraries/provideo_protocol/provideo_protocol_lut.c           \
           ../libraries/provideo_protocol/provideo_protocol_tflt.c          \
           ../libraries/provideo_protocol/provideo_protocol_fpnc.c          \
           ../libraries/provideo_protocol/provideo_protocol_playback.c      \
           ../libraries/provideo_protocol/provideo_protocol_osd.c           \
           ../libraries/provideo_protocol/provideo_protocol_iris.c          \
           ../libraries/provideo_protocol/provideo_protocol_knee.c          \
           ../libraries/provideo_protocol/provideo_protocol_roi.c           \
           ../libraries/provideo_protocol/provideo_protocol_dpcc.c          \
           ../libraries/provideo_protocol/provideo_protocol_lens.c          \
           ../libraries/simple_math/rgb2ycbcr.c                             \
           ../libraries/simple_math/xyz2ct.c                                \
           ../libraries/simple_math/cubic.c                                 \
           ../libraries/simple_math/knee.c                                  \
           ../libraries/simple_math/gamma.c                                 \
           ../libraries/simple_math/float.c                                 \
           ../libraries/csv/csvparser.c                                     \
           ../libraries/csv/csvwriter.c                                     \
           ../libraries/qcustomplot/qcustomplot.cpp                         \
           ../libraries/xmodem/crc16-xmodem.c                               \
           ../libraries/xmodem/transfer.cpp                                 \
           mainwindow.cpp                                                   \
           main.cpp


HEADERS  += ../libraries/include                                                \
            ../dct_widgets/com_ctrl/devices/IronSDI_Device.h                    \
            ../dct_widgets/mcceqbox/mcceqbox.h                                  \
            ../dct_widgets/roibox/roibox.h                                      \
            ../dct_widgets/wbbox/wbbox.h                                        \
            ../dct_widgets/infobox/infobox.h                                    \
            ../dct_widgets/blacklevelbox/blacklevelbox.h                        \
            ../dct_widgets/lutbox/lutbox.h                                      \
            ../dct_widgets/lutbox/cubic_interpolation.h                         \
            ../dct_widgets/inoutbox/inoutbox.h                                  \
            ../dct_widgets/outbox/outbox.h                                      \
            ../dct_widgets/fltbox/fltbox.h                                      \
            ../dct_widgets/updatebox/updatebox.h                                \
            ../dct_widgets/updatebox/flashloader.h                              \
            ../dct_widgets/updatebox/id.h                                       \
            ../dct_widgets/kneebox/kneebox.h                                    \
            ../dct_widgets/kneebox/knee_interpolation.h                         \
            ../dct_widgets/dpccbox/dpccbox.h                                    \
            ../dct_widgets/lensdriverbox/lensdriverbox.h                        \
            ../dct_widgets/btnarraybox/btnarraybox.h                            \
            ../dct_widgets/singlechannelknobbox/singlechannelknobbox.h          \
            ../dct_widgets/triplechannelknobbox/triplechannelknobbox.h          \
            ../dct_widgets/fmtspinbox/fmtspinbox.h                              \
            ../dct_widgets/skinneddial/skinneddial.h                            \
            ../dct_widgets/piesegmentselect/piesegmentselect.h                  \
            ../dct_widgets/huesegmentselect/huesegmentselect.h                  \
            ../dct_widgets/mccslider/mccslider.h                                \
            ../dct_widgets/connectdialog/connectdialog.h                        \
            ../dct_widgets/settingsdialog/settingsdialog.h                      \
            ../dct_widgets/infodialog/infodialog.h                              \
            ../dct_widgets/aecweightsdialog/aecweightsdialog.h                  \
            ../dct_widgets/com_ctrl/devices/ProVideoDevice.h                    \
            ../dct_widgets/com_ctrl/devices/XbowDevice.h                        \
            ../dct_widgets/com_ctrl/devices/Condor4kDevice.h                    \
            ../dct_widgets/com_ctrl/devices/CooperDevice.h                      \
            ../dct_widgets/com_ctrl/ProVideoItf.h                               \
            ../dct_widgets/com_ctrl/ProVideoSystemItf.h                         \
            ../dct_widgets/com_ctrl/IspItf.h                                    \
            ../dct_widgets/com_ctrl/CprocItf.h                                  \
            ../dct_widgets/com_ctrl/AutoItf.h                                   \
            ../dct_widgets/com_ctrl/CamItf.h                                    \
            ../dct_widgets/com_ctrl/MccItf.h                                    \
            ../dct_widgets/com_ctrl/LutItf.h                                    \
            ../dct_widgets/com_ctrl/ChainItf.h                                  \
            ../dct_widgets/com_ctrl/TfltItf.h                                   \
            ../dct_widgets/com_ctrl/FpncItf.h                                   \
            ../dct_widgets/com_ctrl/FpncData.h                                  \
            ../dct_widgets/com_ctrl/PlayBackItf.h                               \
            ../dct_widgets/com_ctrl/IrisItf.h                                   \
            ../dct_widgets/com_ctrl/KneeItf.h                                   \
            ../dct_widgets/com_ctrl/ROIItf.h                                    \
            ../dct_widgets/com_ctrl/DpccItf.h                                   \
            ../dct_widgets/com_ctrl/OsdItf.h                                    \
            ../dct_widgets/com_ctrl/LensItf.h                                   \
            ../dct_widgets/com_ctrl/ComChannel.h                                \
            ../dct_widgets/com_ctrl/ComChannelRSxxx.h                           \
            ../dct_widgets/com_ctrl/ComProtocol.h                               \
            ../dct_widgets/com_ctrl/ProVideoProtocol.h                          \
            ../dct_widgets/com_ctrl/common.h                                    \
            ../dct_widgets/com_ctrl/defines.h                                   \
            ../dct_widgets/csvwrapper/csvwrapper.h                              \
            ../dct_widgets/textviewer/textviewer.h                              \
            ../dct_widgets/debugterminal/debugterminal.h                        \
            ../dct_widgets/dct_widgets_base.h                                   \
            ../libraries/include/csv/csvparser.h                                \
            ../libraries/include/csv/csvwriter.h                                \
            ../libraries/include/xmodem/crc16-xmodem.h                          \
            ../libraries/include/xmodem/transfer.h                              \
            ../libraries/qcustomplot/qcustomplot.h                              \
            ../libraries/include/ctrl_channel/ctrl_channel.h                    \
            ../libraries/include/ctrl_channel/ctrl_channel_generic.h            \
            ../libraries/include/ctrl_channel/ctrl_channel_serial.h             \
            ../libraries/include/ctrl_protocol/ctrl_protocol.h                  \
            ../libraries/include/ctrl_protocol/ctrl_protocol_auto.h             \
            ../libraries/include/ctrl_protocol/ctrl_protocol_cam.h              \
            ../libraries/include/ctrl_protocol/ctrl_protocol_chain.h            \
            ../libraries/include/ctrl_protocol/ctrl_protocol_cproc.h            \
            ../libraries/include/ctrl_protocol/ctrl_protocol_dpcc.h             \
            ../libraries/include/ctrl_protocol/ctrl_protocol_fpnc.h             \
            ../libraries/include/ctrl_protocol/ctrl_protocol_iris.h             \
            ../libraries/include/ctrl_protocol/ctrl_protocol_isp.h              \
            ../libraries/include/ctrl_protocol/ctrl_protocol_knee.h             \
            ../libraries/include/ctrl_protocol/ctrl_protocol_roi.h              \
            ../libraries/include/ctrl_protocol/ctrl_protocol_lut.h              \
            ../libraries/include/ctrl_protocol/ctrl_protocol_mcc.h              \
            ../libraries/include/ctrl_protocol/ctrl_protocol_osd.h              \
            ../libraries/include/ctrl_protocol/ctrl_protocol_playback.h         \
            ../libraries/include/ctrl_protocol/ctrl_protocol_system.h           \
            ../libraries/include/ctrl_protocol/ctrl_protocol_tflt.h             \
            ../libraries/include/ctrl_protocol/ctrl_protocol_lens.h             \
            ../libraries/include/provideo_protocol/provideo_protocol.h          \
            ../libraries/include/simple_math/cubic.h                            \
            ../libraries/include/simple_math/knee.h                             \
            ../libraries/include/simple_math/rgb2ycbcr.h                        \
            ../libraries/include/simple_math/xyz2ct.h                           \
            ../libraries/include/simple_math/gamma.h                            \
            ../libraries/include/simple_math/float.h                            \
            ../libraries/include/provideo_protocol/provideo_protocol_auto.h     \
            ../libraries/include/provideo_protocol/provideo_protocol_cam.h      \
            ../libraries/include/provideo_protocol/provideo_protocol_chain.h    \
            ../libraries/include/provideo_protocol/provideo_protocol_common.h   \
            ../libraries/include/provideo_protocol/provideo_protocol_cproc.h    \
            ../libraries/include/provideo_protocol/provideo_protocol_dpcc.h     \
            ../libraries/include/provideo_protocol/provideo_protocol_fpnc.h     \
            ../libraries/include/provideo_protocol/provideo_protocol_iris.h     \
            ../libraries/include/provideo_protocol/provideo_protocol_isp.h      \
            ../libraries/include/provideo_protocol/provideo_protocol_knee.h     \
            ../libraries/include/provideo_protocol/provideo_protocol_roi.h      \
            ../libraries/include/provideo_protocol/provideo_protocol_lut.h      \
            ../libraries/include/provideo_protocol/provideo_protocol_mcc.h      \
            ../libraries/include/provideo_protocol/provideo_protocol_osd.h      \
            ../libraries/include/provideo_protocol/provideo_protocol_playback.h \
            ../libraries/include/provideo_protocol/provideo_protocol_system.h   \
            ../libraries/include/provideo_protocol/provideo_protocol_tflt.h     \
            ../libraries/include/provideo_protocol/provideo_protocol_lens.h     \
            mainwindow.h

FORMS    += ../dct_widgets/mcceqbox/mcceqbox.ui                             \
            ../dct_widgets/roibox/roibox.ui                                 \
            ../dct_widgets/wbbox/wbbox.ui                                   \
            ../dct_widgets/infobox/infobox.ui                               \
            ../dct_widgets/blacklevelbox/blacklevelbox.ui                   \
            ../dct_widgets/lutbox/lutbox.ui                                 \
            ../dct_widgets/inoutbox/inoutbox.ui                             \
            ../dct_widgets/outbox/outbox.ui                                 \
            ../dct_widgets/fltbox/fltbox.ui                                 \
            ../dct_widgets/updatebox/updatebox.ui                           \
            ../dct_widgets/kneebox/kneebox.ui                               \
            ../dct_widgets/dpccbox/dpccbox.ui                               \
            ../dct_widgets/lensdriverbox/lensdriverbox.ui                   \
            ../dct_widgets/mccslider/mccslider.ui                           \
            ../dct_widgets/singlechannelknobbox/singlechannelknobbox.ui     \
            ../dct_widgets/triplechannelknobbox/triplechannelknobbox.ui     \
            ../dct_widgets/connectdialog/connectdialog.ui                   \
            ../dct_widgets/settingsdialog/settingsdialog.ui                 \
            ../dct_widgets/infodialog/infodialog.ui                         \
            ../dct_widgets/aecweightsdialog/aecweightsdialog.ui             \
            ../dct_widgets/textviewer/textviewer.ui                         \
            ../dct_widgets/debugterminal/debugterminal.ui                   \
            mainwindow.ui

RESOURCES += ../resource/resource.qrc

DISTFILES += ../resource/doc/third-party-licenses.txt \
    ../CHANGELOG.md \
    ../README.md \
    ../third-party-licenses.txt \
    ../LICENSE
