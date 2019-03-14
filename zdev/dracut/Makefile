# Common definitions
include ../../common.mak

MODDIR := /usr/lib/dracut/modules.d/
ZDEVDIR := 95zdev

# HAVE_DRACUT
#
# This install time parameter determines how zdev applies changes to the
# persistent configuration of the root device:
#
# 0: zdev modifies the kernel parameter in the /etc/zipl.conf file to
#    enable the root device.
# 1: zdev installs a dracut module that copies the persistent root device
#    configuration to the initial ram disk created by dracut.
#
ifeq ($(HAVE_DRACUT),1)
install:
	$(INSTALL) -m 755 -d $(DESTDIR)$(MODDIR)
	$(INSTALL) -m 755 -d $(DESTDIR)$(MODDIR)/$(ZDEVDIR)
	$(INSTALL) -m 755 $(DESTDIR)$(ZDEVDIR)/module-setup.sh \
		$(DESTDIR)$(MODDIR)/$(ZDEVDIR)/
endif
