# Should be updated before each release!
VERSION = 1.1.11
DEFINES += VERSION_STRING=\\\"1.1.11\\\"

QMAKE_TARGET_COMPANY = "Dream Chip Technologies GmbH"
QMAKE_TARGET_PRODUCT = "ProVideo Widget Collection"
QMAKE_TARGET_DESCRIPTION = "Collection of Widgets used for the ProVideo GUI"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2019, Dream Chip Technologies GmbH"

QT += network
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets designer
}

CONFIG  += c++11
lessThan(QT_MAJOR_VERSION, 5) {
    CONFIG += plugin
}

TEMPLATE = lib
TARGET = $$qtLibraryTarget($$TARGET)
target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS += target

# Platform specific tweaks
unix {
    FLASH_LOADER_APPLICATION="flashloader"
}
win32 {
    FLASH_LOADER_APPLICATION="flashloader.exe"
}

DEFINES += "QT_NO_PRINTER"
DEFINES += "CFG_CSIP_SAMPLE_INT32"
DEFINES += FLASH_LOADER_APPLICATION=\\\"$$FLASH_LOADER_APPLICATION\\\"

INCLUDEPATH += ../libraries/include     \
               ../libraries/qcustomplot \
               com_ctrl                 \
               csvwrapper               \
               textviewer               \
               infodialog               \
               aecweightsdialog         \
               fmtspinbox               \
               skinneddial              \
               switchbutton             \
               singlechannelknobbox     \
               triplechannelsliderbox   \
               triplechannelknobbox     \
               btnarraybox              \
               piesegmentselect         \
               huesegmentselect         \
               mccslider                \
               wbbox                    \
               mccbox                   \
               mcceqbox                 \
               infobox                  \ 
               blacklevelbox            \
               gammabox                 \
               lutbox                   \
               inoutbox                 \
               outbox                   \
               fltbox                   \
               updatebox                \
               playbackbox              \
               fpncbox                  \
               kneebox                  \
               dpccbox                  \
               lensdriverbox

HEADERS      = fmtspinbox/fmtspinbox.h                              \
               skinneddial/skinneddial.h                            \
               switchbutton/switchbutton.h                          \ 
               singlechannelknobbox/singlechannelknobbox.h          \
               triplechannelsliderbox/triplechannelsliderbox.h      \
               triplechannelknobbox/triplechannelknobbox.h          \
               btnarraybox/btnarraybox.h                            \
               piesegmentselect/piesegmentselect.h                  \
               huesegmentselect/huesegmentselect.h                  \
               mccslider/mccslider.h                                \
               wbbox/wbbox.h                                        \
               mccbox/mccbox.h                                      \
               mcceqbox/mcceqbox.h                                  \
               infobox/infobox.h                                    \
               blacklevelbox/blacklevelbox.h                        \
               gammabox/gammabox.h                                  \
               lutbox/lutbox.h                                      \
               lutbox/cubic_interpolation.h                         \
               inoutbox/inoutbox.h                                  \
               outbox/outbox.h                                      \
               fltbox/fltbox.h                                      \
               updatebox/updatebox.h                                \
               updatebox/flashloader.h                              \
               playbackbox/playbackbox.h                            \
               fpncbox/fpncbox.h                                    \
               kneebox/kneebox.h                                    \
               kneebox/knee_interpolation.h                         \
               dpccbox/dpccbox.h                                    \
               lensdriverbox/lensdriverbox.h                        \
               ../libraries/include/csv/csvparser.h                 \
               ../libraries/include/csv/csvwriter.h                 \
               ../libraries/include/simple_math/cubic.h             \
               ../libraries/include/simple_math/knee.h              \
               ../libraries/include/simple_math/rgb2ycbcr.h         \
               ../libraries/include/simple_math/xyz2ct.h            \
               ../libraries/include/simple_math/gamma.h             \
               ../libraries/include/simple_math/float.h             \
               ../libraries/qcustomplot/qcustomplot.h               \
               com_ctrl/FpncData.h                                  \
               csvwrapper/csvwrapper.h                              \
               textviewer/textviewer.h                              \
               infodialog/infodialog.h                              \
               aecweightsdialog/aecweightsdialog.h                  \
               dct_widgets_base.h                                   \
               dct_widgets.h

SOURCES      = fmtspinbox/fmtspinbox.cpp                            \
               skinneddial/skinneddial.cpp                          \
               switchbutton/switchbutton.cpp                        \
               singlechannelknobbox/singlechannelknobbox.cpp        \
               triplechannelsliderbox/triplechannelsliderbox.cpp    \
               triplechannelknobbox/triplechannelknobbox.cpp        \
               btnarraybox/btnarraybox.cpp                          \
               piesegmentselect/piesegmentselect.cpp                \
               huesegmentselect/huesegmentselect.cpp                \
               mccslider/mccslider.cpp                              \
               wbbox/wbbox.cpp                                      \
               mccbox/mccbox.cpp                                    \
               mcceqbox/mcceqbox.cpp                                \
               infobox/infobox.cpp                                  \
               blacklevelbox/blacklevelbox.cpp                      \
               gammabox/gammabox.cpp                                \
               lutbox/lutbox.cpp                                    \
               lutbox/cubic_interpolation.cpp                       \
               inoutbox/inoutbox.cpp                                \
               outbox/outbox.cpp                                    \
               fltbox/fltbox.cpp                                    \
               updatebox/updatebox.cpp                              \
               updatebox/flashloader.cpp                            \
               playbackbox/playbackbox.cpp                          \
               fpncbox/fpncbox.cpp                                  \
               kneebox/kneebox.cpp                                  \
               kneebox/knee_interpolation.cpp                       \
               dpccbox/dpccbox.cpp                                  \
               lensdriverbox/lensdriverbox.cpp                      \
               ../libraries/csv/csvparser.c                         \
               ../libraries/csv/csvwriter.c                         \
               ../libraries/simple_math/rgb2ycbcr.c                 \
               ../libraries/simple_math/xyz2ct.c                    \
               ../libraries/simple_math/cubic.c                     \
               ../libraries/simple_math/knee.c                      \
               ../libraries/simple_math/gamma.c                     \
               ../libraries/simple_math/float.c                     \
               ../libraries/qcustomplot/qcustomplot.cpp             \
               com_ctrl/FpncData.cpp                                \
               com_ctrl/common.cpp                                  \
               com_ctrl/defines.cpp                                 \
               csvwrapper/csvwrapper.cpp                            \
               textviewer/textviewer.cpp                            \
               infodialog/infodialog.cpp                            \
               aecweightsdialog/aecweightsdialog.cpp                \
               dct_widgets.cpp

FORMS       += singlechannelknobbox/singlechannelknobbox.ui         \
               triplechannelsliderbox/triplechannelsliderbox.ui     \
               triplechannelknobbox/triplechannelknobbox.ui         \
               mccslider/mccslider.ui                               \
               wbbox/wbbox.ui                                       \
               mccbox/mccbox.ui                                     \
               mcceqbox/mcceqbox.ui                                 \
               infobox/infobox.ui                                   \
               blacklevelbox/blacklevelbox.ui                       \
               gammabox/gammabox.ui                                 \
               lutbox/lutbox.ui                                     \
               inoutbox/inoutbox.ui                                 \
               outbox/outbox.ui                                     \
               fltbox/fltbox.ui                                     \
               updatebox/updatebox.ui                               \
               playbackbox/playbackbox.ui                           \
               fpncbox/fpncbox.ui                                   \
               kneebox/kneebox.ui                                   \
               dpccbox/dpccbox.ui                                   \
               lensdriverbox/lensdriverbox.ui                       \
               textviewer/textviewer.ui                             \
               infodialog/infodialog.ui                             \
               aecweightsdialog/aecweightsdialog.ui

RESOURCES   += ../resource/resource.qrc
