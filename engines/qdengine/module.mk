MODULE := engines/qdengine

MODULE_OBJS = \
	qdengine.o \
	console.o \
	metaengine.o \
	core/qd_precomp.o

# This module can be built as a plugin
ifeq ($(ENABLE_QDENGINE), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk

# Detection objects
DETECT_OBJS += $(MODULE)/detection.o
