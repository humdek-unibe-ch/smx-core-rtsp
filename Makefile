SHELL := /bin/bash
PROJECT = smxrtsp

LOC_INC_DIR = include
LOC_SRC_DIR = src
SIA_LANG_DIR = streamix-sia-lang

include config.mk

LIB_VERSION = $(VMAJ).$(VMIN)
UPSTREAM_VERSION = $(LIB_VERSION).$(VREV)
DEBIAN_REVISION = $(VDEB)
VERSION = $(UPSTREAM_VERSION)-$(DEBIAN_REVISION)

VAPPNAME = $(APPNAME)-$(LIB_VERSION)
VPKGNAME = $(APPNAME)$(LIB_VERSION)

TGT_BIN = $(DESTDIR)/usr/bin
TGT_LIB = $(DESTDIR)/usr/lib/x86_64-linux-gnu
TGT_INCLUDE_BASE = $(DESTDIR)/usr/include/smx
TGT_DOC = $(DESTDIR)/usr/share/doc/$(VPKGNAME)
TGT_CONF = $(DESTDIR)/etc/smx
TGT_TPL = $(TGT_CONF)/tpl
LOCAL_TPL = tpl

SOURCES = main.c \
		  $(LOC_SRC_DIR)/* \
		  $(SIA_LANG_DIR)/$(LOC_SRC_DIR)/*

INCLUDES = $(LOC_INC_DIR)/* \
		   $(SIA_LANG_DIR)/$(LOC_INC_DIR)/*

INCLUDES_DIR = -I/usr/include/igraph \
			   -I/usr/include/smx \
			   -I/usr/include/libbson-1.0 \
			   -I$(LOC_INC_DIR) \
			   -I$(SIA_LANG_DIR)/$(LOC_INC_DIR) \
			   -I$(SIA_LANG_DIR)/uthash/src \
			   $(patsubst %, -I$(TGT_INCLUDE_BASE)/lib%, $(SMX_LIBS)) \
			   -I.

LINK_DIR = -L/usr/local/lib \
	$(patsubst %, -L$(TGT_LIB)/lib%, $(SMX_LIBS)) \
	-L$(TGT_LIB)

LINK_FILE = -ligraph $(patsubst %, -l%, $(SMX_LIBS)) -lbson-1.0

CFLAGS = -Wall \
		 -DSMXRTSP_VERSION_LIB=\"$(LIB_VERSION)\" \
		 -DSMXRTSP_VERSION_UP=\"$(UPSTREAM_VERSION)\"
DEBUG_FLAGS = -g -O0

CC = gcc

all: $(PROJECT)

# compile with dot stuff and debug flags
debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(PROJECT)

# compile project
$(PROJECT): $(SOURCES) $(INCLUDES)
	$(CC) $(CFLAGS) $(SOURCES) $(INCLUDES_DIR) $(LINK_DIR) $(LINK_FILE) -o $(PROJECT)

.PHONY: clean install uninstall dpkg dpkg-test

install:
	mkdir -p $(TGT_BIN) $(TGT_DOC) $(TGT_TPL)
	cp -a $(APPNAME) $(TGT_BIN)/$(VAPPNAME)
	cp -a README.md $(TGT_DOC)/.
	cp -a SMX_CHANGELOG.md $(TGT_DOC)/.
	cp -a utils/smxrtsp_graph2svg.py $(TGT_BIN)/smxrtsp_graph2svg-$(LIB_VERSION).py
	rm -rf  $(TGT_TPL)/smxappgen-$(LIB_VERSION)
	cp -aR $(LOCAL_TPL)/app $(TGT_TPL)/smxappgen-$(LIB_VERSION)

uninstall:
	rm -f $(TGT_BIN)/$(VAPPNAME)
	rm -f $(TGT_BIN)/smxrtsp_graph2svg-$(LIB_VERSION).py
	rm -rf $(TGT_DOC)
	rm -rf $(TGT_TPL)/smxappgen-$(LIB_VERSION)

clean:
	rm -f $(PROJECT)

dpkg:
	/usr/bin/smx_build_bin_package.sh --deb-path tpl/debian

dpkg-test:
	/usr/bin/smx_build_bin_package.sh --deb-path tpl/debian -t
