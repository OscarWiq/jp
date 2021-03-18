PREFIX = /usr

CC = cc
CFLAGS 	= -std=c99 -pedantic -Wall -Werror

SRC = jp.c
OBJ = $(SRC:.c=.o)

jp: $(OBJ)
	$(CC) -o $@ $(OBJ)

$(OBJ):

.c.o:
	$(CC) -c $(CFLAGS) $<

install: jp
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f jp $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/jp

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/jp

clean:
	rm -f jp $(OBJ)

.PHONY: install uninstall clean