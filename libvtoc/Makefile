include ../common.mak

ALL_CPPFLAGS += -I../include

lib = libvtoc.a

all: $(lib)

objects = vtoc.o

$(lib): $(objects)

install: all

clean:
	rm -f *.o $(lib)
