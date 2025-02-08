# dwm - dynamic window manager
# See LICENSE file for copyright and license details.

include config.mk

TARGET = bin/dwm
SRC = src/dwm.c src/drw.c src/util.c
OBJ = $(addprefix $(BUILD)/,$(SRC:.c=.o))
DEP = $(OBJ:.o=.d)
BUILD ?= build

override CPPFLAGS += -I.
override CPPFLAGS += -MMD -MP

COLOR_OFF = \e[0m
COLOR_CC  = \e[32m
COLOR_LD  = \e[1;32m

override quiet =
ifeq ($(findstring s,$(firstword -$(MAKEFLAGS))),s)
    override quiet = 1
endif

override Q =
ifneq ($(VERBOSE),1)
    override Q = @
endif

-include $(DEP)

.DEFAULT_GOAL := all
.PHONY: all
all: $(BUILD)/$(TARGET)

$(OBJ): $(BUILD)/%.o: %.c
	$(if $(quiet),,@echo -e "[CC] $(COLOR_CC)Compiling $(subst $(BUILD)/,,$@)$(COLOR_OFF)")
	@mkdir -p $(@D)
	$(Q)$(CC) $< -c $(CPPFLAGS) $(CFLAGS) -o $@

$(BUILD)/$(TARGET): $(OBJ)
	$(if $(quiet),,@echo -e "[LD] $(COLOR_LD)Linking $(subst $(BUILD)/,,$@) (executable)$(COLOR_OFF)")
	@mkdir -p $(@D)
	$(Q)$(CC) $^ $(LDFLAGS) $(LDLIBS) -o $@

.PHONY: clean
clean:
	$(if $(quiet),,@echo "[RM] Removing build artifacts")
	$(Q)$(RM) -r $(BUILD)

.PHONY: install
install: all
	$(if $(quiet),,@echo "[IN] Installing to $(DESTDIR)$(PREFIX)")
	$(Q)install -Dm755 $(BUILD)/$(TARGET) -t $(DESTDIR)$(PREFIX)/bin
	$(Q)install -Dm755 audiobrightctl.sh -t $(DESTDIR)$(PREFIX)/bin
	$(Q)install -Dm644 docs/dwm.1 -t $(DESTDIR)$(MANPREFIX)/man1
	$(Q)sed -i "s/VERSION/$(VERSION)/g" $(DESTDIR)$(MANPREFIX)/man1/dwm.1

.PHONY: uninstall
uninstall:
	$(if $(quiet),,@echo "[UN] Removing installation")
	$(Q)$(RM) $(DESTDIR)$(PREFIX)/bin/dwm
	$(Q)$(RM) $(DESTDIR)$(PREFIX)/bin/audiobrightctl.sh
	$(Q)$(RM) $(DESTDIR)$(MANPREFIX)/man1/dwm.1

.PHONY: help
help:

	@echo "Targets:"
	@echo "    make all              -- Build $(TARGET)"
	@echo "    make install          -- Build and install $(TARGET)"
	@echo "    make uninstall        -- Uninstall $(TARGET)"
	@echo "    make clean            -- Remove build artifacts"
	@echo "    make help             -- Display this message"
	@echo ""
	@echo "Options:"
	@echo "    make BUILD=<...>      -- Specify build directory (default: build)"
	@echo "    make VERBOSE=1 <...>  -- Enable verbose output"
	@echo "    make -s <...>         -- Silent mode"
