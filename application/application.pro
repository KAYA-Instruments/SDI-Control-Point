#-------------------------------------------------
#
# Project created by QtCreator 2016-01-26T17:25:55
#
#-------------------------------------------------

# This version number is shown in the Application, it should be
# updated before each release!
VERSION = 1.0.2
DEFINES += VERSION_STRING=\\\"1.0.2\\\"

CONFIG  += c++11
# CONFIG  += debug
QT      += core serialport widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProVideo
TEMPLATE = app

# Platform specific tweaks
unix {
    FLASH_LOADER_APPLICATION="flashloader"
}
win32 {
    FLASH_LOADER_APPLICATION="flashloader.exe"
}
osx {
    QMAKE_INFO_PLIST = osx/Info.plist
}

DEFINES += FLASH_LOADER_APPLICATION=\\\"$$FLASH_LOADER_APPLICATION\\\"
DEFINES += "QT_NO_PRINTER"
DEFINES += "QT_NO_SVG"
DEFINES += "QT_NO_OPENGL"
DEFINES += "CFG_CSIP_SAMPLE_INT32"

INCLUDEPATH += ../libraries/include ..              \
               ../dct_widgets                       \
               ../dct_widgets/com_ctrl              \
               ../dct_widgets/csvwrapper            \
               ../dct_widgets/connectdialog         \
               ../dct_widgets/btnarraybox           \
               ../dct_widgets/singlechannelknobbox  \
               ../dct_widgets/tripplechannelknobbox \
               ../dct_widgets/fmtspinbox            \
               ../dct_widgets/skinneddial           \
               ../dct_widgets/piesegmentselect      \
               ../dct_widgets/huesegmentselect      \
               ../dct_widgets/mccslider             \
               ../dct_widgets/qcustomplot           \
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
               ../dct_widgets/dpccbox               \
               ../dct_widgets/textviewer

SOURCES += ../dct_widgets/mcceqbox/mcceqbox.cpp                             \
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
           ../dct_widgets/connectdialog/connectdialog.cpp                   \
           ../dct_widgets/btnarraybox/btnarraybox.cpp                       \
           ../dct_widgets/singlechannelknobbox/singlechannelknobbox.cpp     \
           ../dct_widgets/tripplechannelknobbox/tripplechannelknobbox.cpp   \
           ../dct_widgets/fmtspinbox/fmtspinbox.cpp                         \
           ../dct_widgets/skinneddial/skinneddial.cpp                       \
           ../dct_widgets/piesegmentselect/piesegmentselect.cpp             \
           ../dct_widgets/huesegmentselect/huesegmentselect.cpp             \
           ../dct_widgets/mccslider/mccslider.cpp                           \
           ../dct_widgets/qcustomplot/qcustomplot.cpp                       \
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
           ../dct_widgets/com_ctrl/DpccItf.cpp                              \
           ../dct_widgets/com_ctrl/OsdItf.cpp                               \
           ../dct_widgets/com_ctrl/ProVideoDevice.cpp                       \
           ../dct_widgets/com_ctrl/XbowDevice.cpp                           \
           ../dct_widgets/com_ctrl/AdamDevice.cpp                           \
           ../dct_widgets/com_ctrl/CameleonDevice.cpp                       \
           ../dct_widgets/com_ctrl/Condor4kDevice.cpp                       \
           ../dct_widgets/com_ctrl/CondorZeissDevice.cpp                    \
           ../dct_widgets/csvwrapper/csvwrapper.cpp                         \
           ../dct_widgets/textviewer/textviewer.cpp                         \
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
           ../libraries/ctrl_protocol/ctrl_protocol_dpcc.c                  \
           ../libraries/provideo_protocol/provideo_protocol.c               \
           ../libraries/provideo_protocol/provideo_protocol_common.c        \
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
           ../libraries/provideo_protocol/provideo_protocol_dpcc.c          \
           ../libraries/simple_math/rgb2ycbcr.c                             \
           ../libraries/simple_math/xyz2ct.c                                \
           ../libraries/simple_math/cubic.c                                 \
           ../libraries/simple_math/knee.c                                  \
           ../libraries/csv/csvparser.c                                     \
           ../libraries/csv/csvwriter.c                                     \
           mainwindow.cpp                                                   \
           main.cpp

HEADERS  += ../libraries/include                                                \
            ../dct_widgets/mcceqbox/mcceqbox.h                                  \
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
            ../dct_widgets/btnarraybox/btnarraybox.h                            \
            ../dct_widgets/singlechannelknobbox/singlechannelknobbox.h          \
            ../dct_widgets/tripplechannelknobbox/tripplechannelknobbox.h        \
            ../dct_widgets/fmtspinbox/fmtspinbox.h                              \
            ../dct_widgets/skinneddial/skinneddial.h                            \
            ../dct_widgets/piesegmentselect/piesegmentselect.h                  \
            ../dct_widgets/huesegmentselect/huesegmentselect.h                  \
            ../dct_widgets/mccslider/mccslider.h                                \
            ../dct_widgets/qcustomplot/qcustomplot.h                            \
            ../dct_widgets/connectdialog/connectdialog.h                        \
            ../dct_widgets/com_ctrl/ProVideoDevice.h                            \
            ../dct_widgets/com_ctrl/XbowDevice.h                                \
            ../dct_widgets/com_ctrl/AdamDevice.h                                \
            ../dct_widgets/com_ctrl/Condor4kDevice.h                            \
            ../dct_widgets/com_ctrl/CameleonDevice.h                            \
            ../dct_widgets/com_ctrl/CondorZeissDevice.h                         \
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
            ../dct_widgets/com_ctrl/DpccItf.h                                   \
            ../dct_widgets/com_ctrl/OsdItf.h                                    \
            ../dct_widgets/com_ctrl/ComChannel.h                                \
            ../dct_widgets/com_ctrl/ComChannelRSxxx.h                           \
            ../dct_widgets/com_ctrl/ComProtocol.h                               \
            ../dct_widgets/com_ctrl/ProVideoProtocol.h                          \
            ../dct_widgets/com_ctrl/common.h                                    \
            ../dct_widgets/com_ctrl/defines.h                                   \
            ../dct_widgets/csvwrapper/csvwrapper.h                              \
            ../dct_widgets/textviewer/textviewer.h                              \
            ../dct_widgets/dct_widgets_base.h                                   \
            ../libraries/include/csv/csvparser.h                                \
            ../libraries/include/csv/csvwriter.h                                \
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
            ../libraries/include/ctrl_protocol/ctrl_protocol_lut.h              \
            ../libraries/include/ctrl_protocol/ctrl_protocol_mcc.h              \
            ../libraries/include/ctrl_protocol/ctrl_protocol_osd.h              \
            ../libraries/include/ctrl_protocol/ctrl_protocol_playback.h         \
            ../libraries/include/ctrl_protocol/ctrl_protocol_system.h           \
            ../libraries/include/ctrl_protocol/ctrl_protocol_tflt.h             \
            ../libraries/include/provideo_protocol/provideo_protocol.h          \
            ../libraries/include/simple_math/cubic.h                            \
            ../libraries/include/simple_math/knee.h                             \
            ../libraries/include/simple_math/rgb2ycbcr.h                        \
            ../libraries/include/simple_math/xyz2ct.h                           \
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
            ../libraries/include/provideo_protocol/provideo_protocol_lut.h      \
            ../libraries/include/provideo_protocol/provideo_protocol_mcc.h      \
            ../libraries/include/provideo_protocol/provideo_protocol_osd.h      \
            ../libraries/include/provideo_protocol/provideo_protocol_playback.h \
            ../libraries/include/provideo_protocol/provideo_protocol_system.h   \
            ../libraries/include/provideo_protocol/provideo_protocol_tflt.h     \
            mainwindow.h

FORMS    += ../dct_widgets/mcceqbox/mcceqbox.ui                             \
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
            ../dct_widgets/mccslider/mccslider.ui                           \
            ../dct_widgets/singlechannelknobbox/singlechannelknobbox.ui     \
            ../dct_widgets/tripplechannelknobbox/tripplechannelknobbox.ui   \
            ../dct_widgets/connectdialog/connectdialog.ui                   \
            ../dct_widgets/textviewer/textviewer.ui                         \
            mainwindow.ui

RESOURCES = ../ressource/ressource.qrc

DISTFILES += ../ressource/doc/third-party-licenses.txt
