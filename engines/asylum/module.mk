MODULE := engines/asylum

MODULE_OBJS := \
	asylum.o \
	bundle.o \
	detection.o \
	graphicbundle.o \
	movie.o \
	resman.o \
	screen.o

# This module can be built as a plugin
ifeq ($(ENABLE_ASYLUM), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
