# Makefile for gxconsole module
#
# Copyright (C) 2007 Beihang Unversity.
# Written by Zhu Like, zlike@cse.buaa.edu.cn

# ========== Configuration =============

CROSS_COMPILE := /opt/eldk/usr/bin/mips_4KC-
CC			  := $(CROSS_COMPILE)gcc
CFLAGS		  := -O -G 0 -mno-abicalls -fno-builtin -Wall

# ========= End of configuration =======

drivers		  := gxconsole

.PHONY:	all $(drivers) 

all: $(drivers)

$(drivers):
	$(MAKE) --directory=$@

clean: 
	for d in $(drivers); \
		do				\
			$(MAKE) --directory=$$d clean; \
		done
