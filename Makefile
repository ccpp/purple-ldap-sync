CFLAGS=-Wall $(shell pkg-config --cflags purple)
LDFLAGS=$(shell pkg-config --libs purple) -lldap

default: ldapsync.so ldaptest

ldapsync.so: ldapsync.o
	$(CC) -shared $(LDFLAGS) $< -o $@

ldaptest: ldaptest.o

ldapsync.o: ldapsync.c
	$(CC) -fPIC $(CFLAGS) -c -o $@ $<
