# dwm - dynamic window manager
# See LICENSE file for copyright and license details.

include config.mk

TARGET = bin/dwm
SRC = dwm.c drw.c util.c
OBJ = $(addprefix $(BUILD)/,$(SRC:.c=.o))
DEP = $(OBJ:.o=.d)
BUILD ?= build

override CPPFLAGS += -MMD -MP

COLOR_OFF = \e[0m
COLOR_CC = \e[32m
COLOR_LD = \e[1;32m

.DEFAULT_GOAL := all

ifneq ($(findstring s,$(firstword -$(MAKEFLAGS))),)
    override quiet = 1
else
    override quiet =
endif

ifneq ($(VERBOSE),1)
    override Q = @
else
    override Q =
endif

-include $(DEP)

.PHONY: all
all: $(BUILD)/$(TARGET)

$(OBJ): $(BUILD)/%.o: %.c
	$(if $(quiet),,$(Q)echo -e "[CC] $(COLOR_CC)Compiling $(subst $(BUILD)/,,$@)$(COLOR_OFF)")
	$(Q)mkdir -p $(@D)
	$(Q)$(CC) $< -c $(CPPFLAGS) $(CFLAGS) -o $@

$(BUILD)/$(TARGET): $(OBJ)
	$(if $(quiet),,$(Q)echo -e "[LD] $(COLOR_LD)Linking $(subst $(BUILD)/,,$@) (executable)$(COLOR_OFF)")
	$(Q)mkdir -p $(@D)
	$(Q)$(CC) $^ $(LDFLAGS) $(LDLIBS) -o $@

.PHONY: clean
clean:
	$(if $(quiet),,$(Q)echo -e "[RM] Removing build artifacts")
	$(Q)$(RM) $(BUILD)/$(TARGET) $(OBJ) $(DEP)

.PHONY: distclean
distclean: clean
	$(if $(quiet),,$(Q)echo -e "[RM] Removing $(BUILD) (directory)")
	$(Q)$(RM) -r $(BUILD)

.PHONY: install
install: all
	$(if $(quiet),,$(Q)echo -e "$(COLOR_IN)[IN] $(COLOR_RESET)Installing to $(DESTDIR)$(PREFIX)")
	$(Q)install -Dm755 $(BUILD)/$(TARGET) -t $(DESTDIR)$(PREFIX)/bin
	$(Q)install -Dm755 audiobrightctl.sh -t $(DESTDIR)$(PREFIX)/bin
	$(Q)install -Dm644 dwm.1 -t $(DESTDIR)$(MANPREFIX)/man1
	$(Q)install -Dm644 dwm.desktop -t $(DESTDIR)$(PREFIX)/share/xsessions
	$(Q)sed -i "s/VERSION/$(VERSION)/g" $(DESTDIR)$(MANPREFIX)/man1/dwm.1

.PHONY: uninstall
uninstall:
	$(if $(quiet),,$(Q)echo -e "$(COLOR_RM)[UN] $(COLOR_RESET)Removing installation")
	$(Q)$(RM) $(DESTDIR)$(PREFIX)/bin/dwm\
		$(DESTDIR)$(PREFIX)/bin/audiobrightctl.sh\
		$(DESTDIR)$(MANPREFIX)/man1/dwm.1\
		$(DESTDIR)$(PREFIX)/share/xsessions/dwm.desktop

.PHONY: options
options:
	$(info CC         = $(CC))
	$(info CPPFLAGS  = $(CPPFLAGS))
	$(info CFLAGS    = $(CFLAGS))
	$(info LDFLAGS   = $(LDFLAGS))
	$(info LDLIBS    = $(LDLIBS))
	$(info BUILD     = $(BUILD))
	$(info TARGET    = $(TARGET))
	$(info SRC       = $(SRC))
	$(info OBJ       = $(OBJ))
	$(info DEP       = $(DEP))
	$(Q):

.PHONY: help
help:
	$(info make all              -- Build $(TARGET))
	$(info make install          -- Build and install $(TARGET))
	$(info make uninstall        -- Uninstall $(TARGET))
	$(info make clean            -- Remove build artifacts)
	$(info make distclean        -- Delete entire build directory)
	$(info make options          -- Show build configuration)
	$(info make help             -- Display this message)
	$(info make VERBOSE=1 <...>  -- Enable verbose output)
	$(info make -s <...>         -- Silent mode)
	$(Q):
