
CFLAGS = -Wall -Wextra -g
LDFLAGS = -ldl -lpthread

all: crashme crashme.O0.strip crashme.O2.strip crashme.O0.dbg crashme.O2.dbg

crashme: crashme.O0.dbg
	ln -s $^ $@

crashme.O0.dbg: crashme.O0.o
	$(CC) $^ $(LDFLAGS) -o $@
	
crashme.O2.dbg: crashme.O2.o
	$(CC) $^ $(LDFLAGS) -o $@
	
%.strip: %.dbg
	cp $< $@
	strip $@

%.O0.o: %.c
	$(CC) $^ $(CFLAGS) -c -O0 -o $@

%.O2.o: %.c
	$(CC) $^ $(CFLAGS) -c -O2 -o $@

.PHONY: clean
clean: coreclean
	rm -f crashme.O0.strip
	rm -f crashme.O2.strip
	rm -f crashme.O0.dbg
	rm -f crashme.O2.dbg
	rm -f crashme.O0.o
	rm -f crashme.O2.o
	rm -f crashme

.PHONY: coreclean
coreclean: 
	rm -f core.*

