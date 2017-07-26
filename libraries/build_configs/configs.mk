###############################################################################
# Debug configuration
###############################################################################
ifeq ($(CONFIG),debug)
    CFLAGS += -g -O0 -DDEBUG
else
    CFLAGS += -O3
endif

###############################################################################
# Build configuration for simple_math
###############################################################################

###############################################################################
# Cubic spline interpolation
# - CFG_HOLD_SAMPLES: hold sample in interpolation context (reserve memory for it)
# - CFG_CONV_SAMPLE_INT32: interpolation sample type = int32_t
# - CONFIG_CONV_SAMPLE_INT32: convolution sample type = int32_t
###############################################################################
CFLAGS += -DCFG_HOLD_SAMPLES
CFLAGS += -DCFG_CSIP_SAMPLE_INT32
CFLAGS += -DCFG_CONV_SAMPLE_INT32


