CFLAGS=-fPIC $(shell pkg-config --cflags purple)
LDFLAGS=-shared $(shell pkg-config --libs purple)

ldapsync.so: ldapsync.o
	$(CC) $(LDFLAGS) $< -o $@

