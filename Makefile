BIN=chmac
OBJS=chmac.o
CFLAGS=-Wall -Werror

${BIN}: ${OBJS}
	${CC} -o ${BIN} ${OBJS}

clean:
	rm -f ${BIN} ${OBJS}

install: ${BIN}
	mkdir -p ${DESTDIR}/usr/sbin
	install -m 755 ${BIN} ${DESTDIR}/usr/sbin/
