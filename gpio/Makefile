#
# Makefile:
#	The gpio command:
#	  A swiss-army knige of GPIO shenanigans.
#	https://projects.drogon.net/wiring-pi
#
#	Copyright (c) 2012-2016 Gordon Henderson
#################################################################################
# This file is part of wiringPi:
#	A "wiring" library for the Raspberry Pi
#
#    wiringPi is free software: you can redistribute it and/or modify
#    it under the terms of the GNU Lesser General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    wiringPi is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU Lesser General Public License for more details.
#
#    You should have received a copy of the GNU Lesser General Public License
#    along with wiringPi.  If not, see <http://www.gnu.org/licenses/>.
#################################################################################

DESTDIR?=/usr
PREFIX?=/local

ifneq ($V,1)
Q ?= @
endif

#DEBUG	= -g -O0
DEBUG	= -O2
CC	= gcc
INCLUDE	= -I$(DESTDIR)$(PREFIX)/include
CFLAGS	= $(DEBUG) -Wall -Wextra $(INCLUDE) -Winline -pipe

LDFLAGS	= -L$(DESTDIR)$(PREFIX)/lib
LIBS    = -lwiringPi -lwiringPiDev -lpthread -lrt -lm -lcrypt

ifeq ($(BOARD),)
  BOARD = orangepioneplus-h6
endif

ifeq ($(BOARD), orangepi2giot)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_2G_IOT
endif

ifneq ($(findstring $(BOARD), "orangepione-h3" "orangepilite-h3" "orangepipc-h3" "orangepiplus-h3" "orangepipcplus-h3" "orangepiplus2e-h3"),)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_H3
endif

ifeq ($(BOARD), orangepipc2-h5)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_PC2
endif

ifeq ($(BOARD), orangepiprime-h5)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_PRIME
endif

ifeq ($(BOARD), orangepizeroplus-h5)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_ZEROPLUS
endif

ifneq ($(findstring $(BOARD), "orangepiwin-a64" "orangepiwinplus-a64"),)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_WIN
endif

ifneq ($(findstring $(BOARD), "orangepizero-h2" "orangepir1-h2"),)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_ZERO
endif

ifneq ($(findstring $(BOARD), "orangepioneplus-h6" "orangepilite2-h6"),)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_LITE2
endif

ifeq ($(BOARD), orangepi3-h6)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_3
endif

ifeq ($(BOARD), orangepizero2-h616)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_ZERO2
endif

ifeq ($(BOARD), orangepizeroplus2h3)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_ZEROPLUS2_H3
endif

ifeq ($(BOARD), orangepizeroplus2h5)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_ZEROPLUS2_H5
endif

ifeq ($(BOARD), orangepirk3399)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_RK3399
endif

ifeq ($(BOARD), orangepi4)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_4
endif

ifeq ($(BOARD), orangepi4-lts)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_4_LTS
endif

ifeq ($(BOARD), orangepi800)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_800
endif

ifeq ($(BOARD), orangepir1plus-rk3328)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_R1PLUS
endif

EXTRA_CFLAGS += -DCONFIG_ORANGEPI

# May not need to  alter anything below this line
###############################################################################

SRC	=	gpio.c readall.c OrangePi.c

OBJ	=	$(SRC:.c=.o)

all:		gpio

version.h:	../VERSION
	$Q echo Need to run newVersion above.

gpio:	$(OBJ)
	$Q echo [Link]
	$Q $(CC) -o $@ $(OBJ) $(LDFLAGS) $(LIBS)

.c.o:
	$Q echo [Compile] $<
	$Q $(CC) -c $(CFLAGS) $(EXTRA_CFLAGS) $< -o $@

.PHONY:	clean
clean:
	$Q echo "[Clean]"
	$Q rm -f $(OBJ) gpio *~ core tags *.bak

.PHONY:	tags
tags:	$(SRC)
	$Q echo [ctags]
	$Q ctags $(SRC)

.PHONY:	install
install: gpio
	$Q echo "[Install]"
	$Q cp gpio		$(DESTDIR)$(PREFIX)/bin/
ifneq ($(WIRINGPI_SUID),0)
	$Q chown root.root	$(DESTDIR)$(PREFIX)/bin/gpio
	$Q chmod 4755		$(DESTDIR)$(PREFIX)/bin/gpio
endif
	$Q mkdir -p		$(DESTDIR)$(PREFIX)/share/man/man1
	$Q cp gpio.1		$(DESTDIR)$(PREFIX)/share/man/man1/

.PHONY:	install-deb
install-deb:	gpio
	$Q echo "[Install: deb]"
	$Q install -m 0755 -d							$(CURDIR)/../debian-template/wiringPi/usr/bin
	$Q install -m 0755 gpio							$(CURDIR)/../debian-template/wiringPi/usr/bin
	$Q install -m 0755 -d							$(CURDIR)/../debian-template/wiringPi/usr/share/man/man1
	$Q install -m 0644 gpio.1						$(CURDIR)/../debian-template/wiringPi/usr/share/man/man1

.PHONY:	uninstall
uninstall:
	$Q echo "[UnInstall]"
	$Q rm -f $(DESTDIR)$(PREFIX)/bin/gpio
	$Q rm -f $(DESTDIR)$(PREFIX)/share/man/man1/gpio.1

.PHONY:	depend
depend:
	makedepend -Y $(SRC)

# DO NOT DELETE

gpio.o: ../version.h
