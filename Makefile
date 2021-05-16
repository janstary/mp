PREFIX	= $(HOME)
BINDIR	= $(PREFIX)/bin
MANDIR	= $(PREFIX)/man/man1/
CFLAGS	= -Wall -pedantic

BIN = mp
MAN = mp.1

all: $(BIN)

lint: $(MAN)
	mandoc -Tlint -Wstyle $(MAN)

test: $(BIN)
	./$(BIN) < test.in | diff -uw - test.out

install: $(BIN) $(MAN) test
	install -d -m 755 $(BINDIR) && install -m 755 $(BIN) $(BINDIR)
	install -d -m 755 $(MANDIR) && install -m 644 $(MAN) $(MANDIR)

uninstall:
	rm -f $(BINDIR)/$(BIN)
	rm -f $(MANDIR)/$(MAN)

clean:
	rm -f $(BIN) *.core *~
