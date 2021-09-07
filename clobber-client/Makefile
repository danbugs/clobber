P=clobber-client
OBJECTS=src/main.c
EMCC=emcc
EMCC_CFLAGS=-s MAIN_MODULE=1
BUILDDIR=build
DEPENDENCIES=dependencies/dcw/dcw.o 

$(P): $(OBJECTS)
	$(EMCC) $(EMCC_CFLAGS) $(DEPENDENCIES) src/main.c -o $(BUILDDIR)/$(P).js