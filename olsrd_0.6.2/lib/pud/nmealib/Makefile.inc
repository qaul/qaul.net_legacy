######################
#
# Highlevel configuration options for all
#
#

# activate debugging with 1 or deactivate with 0
DEBUG ?= 1


######################
#
# Lowlevel options and rules
#

ifeq ($(DEBUG),0)
STRIP ?=	strip
else
STRIP ?=	:
endif


GITVERSION = $(shell git describe --dirty='-dirty')

# we expect the version to be like 'v0.5.3-27-g0c2727a' and then strip the 'v',
# and the '-27-g0c2727a' parts
VERSION=0.6.8

# protect against no version number
ifeq ($(strip $(VERSION)),)
VERSION=0.0.0
endif

CC ?= gcc
CCFLAGS += -fPIC -O2 -Wall -Wextra -Wformat=2 -Winit-self \
           -Wmissing-include-dirs -Wswitch-default -Wswitch-enum -Werror
