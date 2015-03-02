CFLAGS=-g -Wall $(shell pkg-config --cflags purple glib-2.0)
LDFLAGS=-lldap -llber $(shell pkg-config --libs purple glib-2.0)

default: ldapsync.so ldaptest

ldapsync.so: ldapsync.o
	$(CC) -shared $(LDFLAGS) $< -o $@

# LDFLAGS must be after the object file! (GCC)
ldaptest: ldaptest.o
	$(CC) -o $@ $< $(LDFLAGS)

ldapsync.o: ldapsync.c
	$(CC) -fPIC $(CFLAGS) -c -o $@ $<

install: ldapsync.so
	install -vm 664 $< $(DESTDIR)/usr/lib/purple-2/

uninstall:
	rm -fv $(DESTDIR)/usr/lib/purple-2/ldapsync.so

clean:
	rm -fv ldap*.o ldap*.so ldaptest
