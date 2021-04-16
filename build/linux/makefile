NAME := grassroots_frictionless_data_tool
DIR_BUILD :=  $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
DIR_SRC := $(realpath $(DIR_BUILD)/../../src)
DIR_INCLUDE := $(realpath $(DIR_BUILD)/../../include)

ifeq ($(DIR_BUILD_CONFIG),)
export DIR_BUILD_CONFIG = $(realpath $(DIR_BUILD)/../../../../build-config/linux)
endif

include $(DIR_BUILD_CONFIG)/project.properties


FIELD_TRIAL_INC = $(DIR_BUILD_CONFIG)/../../services/field-trials/include

DIR_CURL_LIB := /usr/lib/x86_64-linux-gnu


export DIR_INSTALL=$(DIR_GRASSROOTS_INSTALL)/

BUILD		:= debug

PLATFORM := unix

VPATH	:= \
	$(DIR_SRC) \
	$(DIR_SRC)/os
	

INCLUDES := \
	-I$(DIR_INCLUDE) \
	-I$(DIR_GRASSROOTS_UTIL_INC) \
	-I$(DIR_GRASSROOTS_UTIL_INC)/containers \
	-I$(DIR_GRASSROOTS_UTIL_INC)/io \
	-I$(DIR_JANSSON_INC) \
	-I$(DIR_UUID_INC) \
	-I$(DIR_BSON_INC) \
	-I$(DIR_GRASSROOTS_NETWORK_INC) \
	-I$(DIR_GRASSROOTS_SERVER_INC) \
	-I$(DIR_GRASSROOTS_SERVICES_INC) \
	-I$(DIR_GRASSROOTS_FRICTIONLESS_INC) \
	
SRCS 	:= \
	fd_byte_buffer.c \
	fd_curl_util.c \
	fd_filesystem_util.c \
	fd_json_util.c \
	fd_string_util.c \
	fd_tool.c \
	html_printer.c \
	markdown_printer.c \
	printer.c \

ifeq ($(PLATFORM),unix)
	SRCS += unix_filesystem.c 
else ifeq ($(PLATFORM),windows)
	SRCS += windows_filesystem.c 
else ifeq ($(PLATFORM),amiga)
	SRCS += amiga_filesystem.c 	
endif

LDFLAGS += $(DIR_JANSSON_LIB)/libjansson.a -lcurl # $(DIR_CURL_LIB)/libcurl.a

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



