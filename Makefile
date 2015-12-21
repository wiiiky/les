

all: les.so

CFLAGS = `pkg-config --cflags sph python3`
LDFLAGS = `pkg-config --libs sph python3`

les.so: les.c les-sph.c
	$(CC) $(CFLAGS) $(LDFLAGS) -shared -fPIC $? -o $@

clean:
	-astyle --style=google -n *.c
	-rm -rf *~ *.so

distclean: clean
