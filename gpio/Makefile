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

ifeq ($(PLATFORM),)
  #PLATFORM = OrangePi_2G-IOT
  #PLATFORM = OrangePi_PC2
  #PLATFORM = OrangePi_PRIME
  #PLATFORM = OrangePi_ZEROPLUS
  #PLATFORM = OrangePi_WIN
  #PLATFORM = OrangePi_H5
  #PLATFORM = OrangePi_ZERO
  #PLATFORM = OrangePi_R1
  #PLATFORM = OrangePi_H3
  #PLATFORM = OrangePi_LITE2
  PLATFORM = OrangePi_ONEPLUS
  #PLATFORM = OrangePi_ZEROPLUS2_H3
  #PLATFORM = OrangePi_ZEROPLUS2_H5
  #PLATFORM = OrangePi_3
  #PLATFORM = OrangePi_RK3399
endif

ifeq ($(PLATFORM), OrangePi_2G-IOT)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_2G_IOT
EXTRA_CFLAGS += -DCONFIG_ORANGEPI
endif

ifneq ($(findstring $(PLATFORM), "OrangePi_ONE_H3" "OrangePi_LITE_H3" "OrangePi_PC" "OrangePi_PLUS" "OrangePi_PCPLUS" "OrangePi_PLUS2E"),)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_H3
EXTRA_CFLAGS += -DCONFIG_ORANGEPI
endif

ifeq ($(PLATFORM), OrangePi_PC2)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_PC2
EXTRA_CFLAGS += -DCONFIG_ORANGEPI
endif

ifeq ($(PLATFORM), OrangePi_PRIME)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_PRIME
EXTRA_CFLAGS += -DCONFIG_ORANGEPI
endif

ifeq ($(PLATFORM), OrangePi_ZEROPLUS)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_ZEROPLUS
EXTRA_CFLAGS += -DCONFIG_ORANGEPI
endif

ifneq ($(findstring $(PLATFORM), "OrangePi_WIN" "OrangePi_WINPLUS"),)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_WIN
EXTRA_CFLAGS += -DCONFIG_ORANGEPI
endif

ifneq ($(findstring $(PLATFORM), "OrangePi_ZERO" "OrangePi_R1"),)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_ZERO
EXTRA_CFLAGS += -DCONFIG_ORANGEPI
endif

ifneq ($(findstring $(PLATFORM), "OrangePi_ONEPLUS" "OrangePi_LITE2"),)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_LITE2
EXTRA_CFLAGS += -DCONFIG_ORANGEPI
endif

ifeq ($(PLATFORM), OrangePi_3)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_3
EXTRA_CFLAGS += -DCONFIG_ORANGEPI
endif

ifeq ($(PLATFORM), OrangePi_ZEROPLUS2_H3)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_ZEROPLUS2_H3
EXTRA_CFLAGS += -DCONFIG_ORANGEPI
endif

ifeq ($(PLATFORM), OrangePi_ZEROPLUS2_H5)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_ZEROPLUS2_H5
EXTRA_CFLAGS += -DCONFIG_ORANGEPI
endif

ifeq ($(PLATFORM), OrangePi_RK3399)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_RK3399
EXTRA_CFLAGS += -DCONFIG_ORANGEPI
endif

ifeq ($(PLATFORM), OrangePi_4)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_4
EXTRA_CFLAGS += -DCONFIG_ORANGEPI
endif

# May not need to  alter anything below this line
###############################################################################

SRC	=	gpio.c readall.c

ifeq ($(PLATFORM), OrangePi_2G-IOT)
  SRC += OrangePi.c
endif

ifeq ($(PLATFORM), OrangePi_PC2)
  SRC += OrangePi.c
endif

ifeq ($(PLATFORM), OrangePi_PRIME)
  SRC += OrangePi.c
endif

ifeq ($(PLATFORM), OrangePi_ZEROPLUS)
  SRC += OrangePi.c
endif

ifneq ($(findstring $(PLATFORM), "OrangePi_WIN" "OrangePi_WINPLUS"),)
  SRC += OrangePi.c
endif

ifneq ($(findstring $(PLATFORM), "OrangePi_ONE_H3" "OrangePi_LITE_H3" "OrangePi_PC" "OrangePi_PLUS" "OrangePi_PCPLUS" "OrangePi_PLUS2E"),)
  SRC += OrangePi.c
endif

ifneq ($(findstring $(PLATFORM), "OrangePi_ZERO" "OrangePi_R1"),)
  SRC += OrangePi.c
endif

ifneq ($(findstring $(PLATFORM), "OrangePi_ONEPLUS" "OrangePi_LITE2"),)
  SRC += OrangePi.c
endif

ifeq ($(PLATFORM), OrangePi_3)
  SRC += OrangePi.c
endif

ifeq ($(PLATFORM), OrangePi_ZEROPLUS2_H3)
  SRC += OrangePi.c
endif

ifeq ($(PLATFORM), OrangePi_ZEROPLUS2_H5)
  SRC += OrangePi.c
endif

ifeq ($(PLATFORM), OrangePi_RK3399)
  SRC += OrangePi.c
endif

ifeq ($(PLATFORM), OrangePi_4)
  SRC += OrangePi.c
endif

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
