CC = gcc
CFLAGS = -O3

TARGET = wcs

prefix = /usr/local

INSTALL = install
RM = rm

all: $(TARGET)

wcs: wcs.c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

install: wcs

	$(INSTALL) -d -m 0755 $(prefix)/bin
	$(INSTALL) -m 0755 $(TARGET) $(prefix)/bin/
	
clean:
	$(RM) $(TARGET)