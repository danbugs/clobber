P=clobber
OBJECTS=src/main.c
EMCC=emcc
EMCC_CFLAGS=-s MAIN_MODULE=1 -lwebsocket.js --js-library dependencies/dcw/dcw.js
BUILDDIR=build
DEPENDENCIES=dependencies/dcw/dcw.o 

$(P): $(OBJECTS)
	$(EMCC) $(EMCC_CFLAGS) $(DEPENDENCIES) src/main.c -o $(BUILDDIR)/$(P).js