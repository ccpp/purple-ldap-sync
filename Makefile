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

clean:
	rm -fv ldap*.o ldap*.so ldaptest
