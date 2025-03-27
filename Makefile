# dwm - dynamic window manager
# See LICENSE file for copyright and license details.

include config.mk

SRC = drw.c dwm.c util.c
BUILD_DIR ?= build
OBJ = $(addprefix $(BUILD_DIR)/, $(SRC:.c=.o))
DEP = $(OBJ:.o=.d)

COLOR_RESET = \033[0m
COLOR_CC    = \033[32m
COLOR_LD    = \033[35m
COLOR_RM    = \033[33m
COLOR_IN    = \033[34m

.PHONY: all
all: dwm

.PHONY: options
options:
	@echo dwm build options:
	@echo "CFLAGS    = $(CFLAGS)"
	@echo "LDFLAGS   = $(LDFLAGS)"
	@echo "CC        = $(CC)"
	@echo "BUILD_DIR = $(BUILD_DIR)"

-include $(DEP)

$(OBJ): $(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@echo -e "$(COLOR_CC)[CC] $(COLOR_RESET)Compiling $<"
	@$(CC) -MMD -MP -c $(CFLAGS) $< -o $@

$(BUILD_DIR):
	@mkdir -p $@

dwm: $(OBJ)
	@echo -e "$(COLOR_LD)[LD] $(COLOR_RESET)Linking $@ (executable)"
	@$(CC) -o $@ $(OBJ) $(LDFLAGS)

.PHONY: clean
clean:
	@echo -e "$(COLOR_RM)[RM] $(COLOR_RESET)Removing build artifacts"
	@$(RM) dwm $(OBJ) $(DEP)

.PHONY: install
install: all
	@echo -e "$(COLOR_IN)[IN] $(COLOR_RESET)Installing to $(DESTDIR)$(PREFIX)"
	@install -Dm755 dwm -t $(DESTDIR)$(PREFIX)/bin
	@install -Dm755 audiobrightctl.sh -t $(DESTDIR)$(PREFIX)/bin
	@install -Dm644 dwm.1 -t $(DESTDIR)$(MANPREFIX)/man1
	@install -Dm644 dwm.desktop -t $(DESTDIR)$(PREFIX)/share/xsessions
	@sed -i "s/VERSION/$(VERSION)/g" $(DESTDIR)$(MANPREFIX)/man1/dwm.1

.PHONY: uninstall
uninstall:
	@echo -e "$(COLOR_RM)[UN] $(COLOR_RESET)Removing installation"
	@$(RM) $(DESTDIR)$(PREFIX)/bin/dwm\
		$(DESTDIR)$(PREFIX)/bin/audiobrightctl.sh\
		$(DESTDIR)$(MANPREFIX)/man1/dwm.1\
		$(DESTDIR)$(PREFIX)/share/xsessions/dwm.desktop

