#!/bin/make
# encoding: UTF-8, break: linux, indet-mode: 4 spaces, lang: GNUMake/eng */
################################################################################
#
# This file is part of PERSwitch.
# Copyright © 2010 con-f-use.
# Use under GNU General Public License.
#
################################################################################

GUI = PERgui
COM = PERScom

GUIOBJECTS = opendevice.o cmdline.o $(GUI).o
COMOBJECTS = opendevice.o cmdline.o $(COM).o
PKG_CFLAGS = $(shell pkg-config --cflags --libs gtk+-2.0)
PKG_LIBS = $(shell pkg-config --libs gtk+-2.0)
CFLAGS = -Wall -Wextra -O3 -lusb
GUIFLAGS = $(CFLAGS) $(PKG_CFLAGS) $(PKG_LIBS)

# This is for compiling on nixos without packaging or shell
#CC = gcc
#INCLUDES = -I/nix/store/aiqxjxjapl9dhi5b21pi16kv5hcvw3q1-libusb-compat-0.1.7-dev/include


# default rule
.PHONY: all
all:$(COM) $(GUI)

#build PERgui system tray icon
$(GUI):$(GUIOBJECTS)
	$(CC) $(GUIFLAGS) -o $@ $^

$(GUI).o:
	$(CC) $(GUIFLAGS) -c $*.c -o $@

#build PERScom command line client
$(COM):$(COMOBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(INCLUDES)

# symbolic targets:
help:
	@echo "foo"
	@echo "bar"

# rule for deleting dependent files (those which can be built by Make):
.PHONY: clean
clean:
	$(RM) -f *.o

.PHONY: cleanall
cleanall:
	$(RM) -f *.o $(GUI) $(COM) PERgui.cfg

# Generic rule for compiling C files:
.c.o:
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)
