# dwm - dynamic window manager
# See LICENSE file for copyright and license details.

include config.mk

SRC = drw.c dwm.c util.c
OBJ = ${SRC:.c=.o}

COLOR_RESET = \033[0m
COLOR_CC    = \033[32m
COLOR_LD    = \033[35m
COLOR_INST  = \033[34m
COLOR_CLEAN = \033[33m
COLOR_DIST  = \033[36m

.PHONY: all
all: dwm

.PHONY: options
options:
	@echo dwm build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

%.o: %.c
	@echo -e "${COLOR_CC}[CC] ${COLOR_RESET}Compiling $<"
	@${CC} -c ${CFLAGS} $<

dwm: ${OBJ}
	@echo -e "${COLOR_LD}[LD] ${COLOR_RESET}Linking $@ (executable)"
	@${CC} -o $@ ${OBJ} ${LDFLAGS}

.PHONY: clean
clean:
	@echo -e "${COLOR_CLEAN}[RM] ${COLOR_RESET}Removing build artifacts"
	@${RM} dwm ${OBJ} dwm-${VERSION}.tar.gz

.PHONY: dist
dist: clean
	@echo -e "${COLOR_DIST}[DI] ${COLOR_RESET}Creating package"
	@mkdir -p dwm-${VERSION}
	@cp -R LICENSE Makefile README config.def.h config.mk\
		dwm.1 drw.h util.h ${SRC} dwm.png transient.c dwm-${VERSION}
	@tar -cf dwm-${VERSION}.tar dwm-${VERSION}
	@gzip dwm-${VERSION}.tar
	@${RM} -r dwm-${VERSION}

.PHONY: install
install: all
	@echo -e "${COLOR_INST}[IN] ${COLOR_RESET}Installing to ${DESTDIR}${PREFIX}"
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@cp -f dwm ${DESTDIR}${PREFIX}/bin
	@chmod 755 ${DESTDIR}${PREFIX}/bin/dwm
	@mkdir -p ${DESTDIR}${MANPREFIX}/man1
	@sed "s/VERSION/${VERSION}/g" < dwm.1 > ${DESTDIR}${MANPREFIX}/man1/dwm.1
	@chmod 644 ${DESTDIR}${MANPREFIX}/man1/dwm.1
	@mkdir -p ${DESTDIR}${PREFIX}/share/xsessions
	@test -f ${DESTDIR}${PREFIX}/share/xsessions/dwm.desktop || cp -n dwm.desktop ${DESTDIR}${PREFIX}/share/xsessions
	@chmod 644 ${DESTDIR}${PREFIX}/share/xsessions/dwm.desktop

.PHONY: uninstall
uninstall:
	@echo -e "${COLOR_CLEAN}[UN] ${COLOR_RESET}Removing installation"
	@${RM} ${DESTDIR}${PREFIX}/bin/dwm\
		${DESTDIR}${MANPREFIX}/man1/dwm.1\
		${DESTDIR}${PREFIX}/share/xsessions/dwm.desktop

