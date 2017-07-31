# This define is needed to compile the Infobox widget,
# the version number is just a dummy in this case
VERSION = DCT_Widget_Build
DEFINES += VERSION_STRING=\\\"DCT_Widget_Build\\\"

CONFIG      += plugin
TEMPLATE     = lib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets designer

target.path  = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS    += target

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
               com_ctrl                 \
               csvwrapper               \
               textviewer               \
               fmtspinbox               \
               skinneddial              \
               switchbutton             \
               singlechannelknobbox     \
               tripplechannelsliderbox  \ 
               tripplechannelknobbox    \
               btnarraybox              \
               piesegmentselect         \
               huesegmentselect         \
               mccslider                \
               qcustomplot              \
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
               dpccbox

HEADERS      = fmtspinbox/fmtspinbox.h                              \
               skinneddial/skinneddial.h                            \
               switchbutton/switchbutton.h                          \ 
               singlechannelknobbox/singlechannelknobbox.h          \
               tripplechannelsliderbox/tripplechannelsliderbox.h    \
               tripplechannelknobbox/tripplechannelknobbox.h        \
               btnarraybox/btnarraybox.h                            \
               piesegmentselect/piesegmentselect.h                  \
               huesegmentselect/huesegmentselect.h                  \
               mccslider/mccslider.h                                \
               qcustomplot/qcustomplot.h                            \
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
               com_ctrl/FpncData.h                                  \
               csvwrapper/csvwrapper.h                              \
               textviewer/textviewer.h                              \
               dct_widgets_base.h                                   \
               dct_widgets.h

SOURCES      = fmtspinbox/fmtspinbox.cpp                            \
               skinneddial/skinneddial.cpp                          \
               switchbutton/switchbutton.cpp                        \
               singlechannelknobbox/singlechannelknobbox.cpp        \
               tripplechannelsliderbox/tripplechannelsliderbox.cpp  \
               tripplechannelknobbox/tripplechannelknobbox.cpp      \
               btnarraybox/btnarraybox.cpp                          \
               piesegmentselect/piesegmentselect.cpp                \
               huesegmentselect/huesegmentselect.cpp                \
               mccslider/mccslider.cpp                              \
               qcustomplot/qcustomplot.cpp                          \
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
               ../libraries/simple_math/rgb2ycbcr.c                 \
               ../libraries/simple_math/xyz2ct.c                    \
               ../libraries/simple_math/cubic.c                     \
               ../libraries/simple_math/knee.c                      \
               com_ctrl/FpncData.cpp                                \
               com_ctrl/common.cpp                                  \
               com_ctrl/defines.cpp                                 \
               csvwrapper/csvwrapper.cpp                            \
               textviewer/textviewer.cpp                            \
               dct_widgets.cpp

FORMS       += singlechannelknobbox/singlechannelknobbox.ui         \
               tripplechannelsliderbox/tripplechannelsliderbox.ui   \
               tripplechannelknobbox/tripplechannelknobbox.ui       \
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
               textviewer/textviewer.ui

RESOURCES    = ../ressource/ressource.qrc \
