
INCLUDEPATH += ../../../include \
               ../../com_ctrl

SOURCES += ../../com_ctrl/common.cpp \
           ../../com_ctrl/defines.cpp \
           ../../com_ctrl/ComProtocol.cpp \
           ../../com_ctrl/ProVideoProtocol.cpp \
           ../../com_ctrl/ComChannel.cpp \
           ../../com_ctrl/ComChannelRSxxx.cpp \
           ../../com_ctrl/ProVideoSystemItf.cpp \
           ../../com_ctrl/IspItf.cpp \
           ../../com_ctrl/CprocItf.cpp \
           ../../com_ctrl/AutoItf.cpp \
           ../../com_ctrl/CamItf.cpp \
           ../../com_ctrl/MccItf.cpp \
           ../../com_ctrl/LutItf.cpp \
           ../../com_ctrl/ChainItf.cpp \
           ../../com_ctrl/TfltItf.cpp \
           ../../com_ctrl/FpncItf.cpp \
           ../../com_ctrl/FpncData.cpp \
           ../../com_ctrl/PlayBackItf.cpp \
           ../../com_ctrl/IrisItf.cpp \
           ../../com_ctrl/KneeItf.cpp \
           ../../com_ctrl/DpccItf.cpp \
           ../../com_ctrl/ProVideoDevice.cpp \
           ../../com_ctrl/XbowDevice.cpp

HEADERS +=  ../../com_ctrl/ProVideoDevice.h \
			../../com_ctrl/ProVideoItf.h \
            ../../com_ctrl/ProVideoSystemItf.h \
            ../../com_ctrl/IspItf.h \
            ../../com_ctrl/CprocItf.h \
            ../../com_ctrl/AutoItf.h \
            ../../com_ctrl/CamItf.h \
            ../../com_ctrl/MccItf.h \
            ../../com_ctrl/LutItf.h \
            ../../com_ctrl/ChainItf.h \
            ../../com_ctrl/TfltItf.h \
            ../../com_ctrl/FpncItf.h \
            ../../com_ctrl/FpncData.h \
            ../../com_ctrl/PlayBackItf.h \
            ../../com_ctrl/IrisItf.h \
            ../../com_ctrl/KneeItf.h \
            ../../com_ctrl/DpccItf.h

SOURCES += ../../../ctrl_channel/ctrl_channel.c \
           ../../../ctrl_protocol/ctrl_protocol.c \
           ../../../ctrl_protocol/ctrl_protocol_isp.c \
           ../../../ctrl_protocol/ctrl_protocol_system.c \
           ../../../ctrl_protocol/ctrl_protocol_cproc.c \
           ../../../ctrl_protocol/ctrl_protocol_auto.c \
           ../../../ctrl_protocol/ctrl_protocol_chain.c \
           ../../../ctrl_protocol/ctrl_protocol_cam.c \
           ../../../ctrl_protocol/ctrl_protocol_mcc.c \
           ../../../ctrl_protocol/ctrl_protocol_lut.c \
           ../../../ctrl_protocol/ctrl_protocol_tflt.c \
           ../../../ctrl_protocol/ctrl_protocol_fpnc.c \
           ../../../ctrl_protocol/ctrl_protocol_playback.c \
           ../../../ctrl_protocol/ctrl_protocol_osd.c \
           ../../../ctrl_protocol/ctrl_protocol_iris.c \
           ../../../ctrl_protocol/ctrl_protocol_knee.c \
           ../../../ctrl_protocol/ctrl_protocol_dpcc.c \
           ../../../provideo_protocol/provideo_protocol.c \
           ../../../provideo_protocol/provideo_protocol_common.c \
           ../../../provideo_protocol/provideo_protocol_system.c \
           ../../../provideo_protocol/provideo_protocol_isp.c \
           ../../../provideo_protocol/provideo_protocol_cproc.c \
           ../../../provideo_protocol/provideo_protocol_auto.c \
           ../../../provideo_protocol/provideo_protocol_chain.c \
           ../../../provideo_protocol/provideo_protocol_cam.c \
           ../../../provideo_protocol/provideo_protocol_mcc.c \
           ../../../provideo_protocol/provideo_protocol_lut.c \
           ../../../provideo_protocol/provideo_protocol_tflt.c \
           ../../../provideo_protocol/provideo_protocol_fpnc.c \
           ../../../provideo_protocol/provideo_protocol_playback.c \
           ../../../provideo_protocol/provideo_protocol_osd.c \
           ../../../provideo_protocol/provideo_protocol_iris.c \
           ../../../provideo_protocol/provideo_protocol_knee.c \
           ../../../provideo_protocol/provideo_protocol_dpcc.c \
           ../../../simple_math/rgb2ycbcr.c \
           ../../../simple_math/xyz2ct.c \
           ../../../simple_math/cubic.c \
           ../../../simple_math/knee.c

