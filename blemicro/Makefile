.PHONY: all connect tree

MAC_BTL  = 96:90:16:63:2D:25
PATH_BTL = /org/bluez/hci0/dev_96_90_16_63_2D_25

CC = gcc
CFLAGS  = -Wall -Wextra -Werror -std=c11

all: sys

gatt:
	sudo gatttool -b $(MAC_BTL) -I

connect:
	busctl call org.bluez $(PATH_BTL) org.bluez.Device1 Connect

intro:
	busctl introspect org.bluez $(PATH_BTL)

tree:
	busctl tree org.bluez

dbus: PFLAGS = $(shell pkgconf --cflags --libs dbus-1)
dbus: dbus.c
	$(CC) $(CFLAGS) $^ $(PFLAGS) -o $@.run 

hci: PFLAGS = -lbluetooth
hci: hci.c
	$(CC) $(CFLAGS) $^ $(PFLAGS) -o $@.run 

gattl: PFLAGS = -lgattlib
gattl: gattl.c
	$(CC) $(CFLAGS) $^ $(PFLAGS) -o $@.run
	
sys: PFLAGS = 
sys: sys.c
	$(CC) $(CFLAGS) $^ $(PFLAGS) -o $@.run

pipe: PFLAGS = -lc 
pipe: pipe.c
	$(CC) $(CFLAGS) $^ $(PFLAGS) -o $@.run
