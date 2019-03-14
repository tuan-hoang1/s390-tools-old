# Common definitions
include ../../common.mak

ALL_CPPFLAGS += -I ../../include -I../include -std=gnu99 -Wno-unused-parameter
ALL_LDFLAGS  += -lpthread -lrt
ifneq ($(HAVE_ZLIB),0)
ALL_CPPFLAGS += -DHAVE_ZLIB
ALL_LDFLAGS  += -lz
endif

core_objects = buffer.o dref.o global.o dump.o idcache.o misc.o strarray.o tar.o
libs = $(rootdir)/libutil/libutil.a

check_dep_zlib:
	$(call check_dep, \
			"dump2tar", \
			"zlib.h", \
			"zlib-devel", \
			"HAVE_ZLIB=0")

all: check_dep_zlib dump2tar

dump2tar: $(core_objects) dump2tar.o $(libs)

install: dump2tar
	$(INSTALL) -c dump2tar $(DESTDIR)$(USRBINDIR)

clean:
	@rm -f dump2tar *.o

.PHONY: all install clean
