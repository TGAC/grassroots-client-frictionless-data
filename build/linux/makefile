NAME := grassroots_frictionless_data_tool
DIR_BUILD :=  $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
DIR_SRC := $(realpath $(DIR_BUILD)/../../src)
DIR_INCLUDE := $(realpath $(DIR_BUILD)/../../include)

ifeq ($(DIR_BUILD_CONFIG),)
export DIR_BUILD_CONFIG = $(realpath $(DIR_BUILD)/../../../../build-config/linux)
endif

include $(DIR_BUILD_CONFIG)/project.properties


FIELD_TRIAL_INC = $(DIR_BUILD_CONFIG)/../../services/field-trials/include


export DIR_INSTALL=$(DIR_GRASSROOTS_INSTALL)/

BUILD		:= debug

VPATH	:= \
	$(DIR_SRC) 
	

INCLUDES := \
	-I$(DIR_INCLUDE) \
	-I$(DIR_GRASSROOTS_UTIL_INC) \
	-I$(DIR_GRASSROOTS_UTIL_INC)/containers \
	-I$(DIR_GRASSROOTS_UTIL_INC)/io \
	-I$(DIR_JANSSON_INC) \
	-I$(DIR_UUID_INC) \
	-I$(DIR_GRASSROOTS_NETWORK_INC) \
	-I$(DIR_GRASSROOTS_SERVER_INC) \
	-I$(DIR_GRASSROOTS_SERVICES_INC) \
	-I$(FIELD_TRIAL_INC)
	
SRCS 	:= \
	fd_tool.c \
	fd_json_util.c \
	html.c \
	printer.c \
	
LDFLAGS += $(DIR_JANSSON_LIB)/libjansson.a

ifeq ($(BUILD),release)
	CFLAGS 	+= -O3 -s
 	LDFLAGS += -s 
else
#	CFLAGS 	+= -g
	CFLAGS 	+= -g -pg
	LDFLAGS += -pg
	CPPFLAGS += -D_DEBUG
endif
	

CPPFLAGS += -DUNIX 

CC := gcc
CXX :=  gcc

all: exe

include $(DIR_BUILD_CONFIG)/generic_makefiles/executable.makefile



