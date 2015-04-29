BIN=chmac
OBJS=chmac.o

${BIN}: ${OBJS}
	${CC} -o ${BIN} ${OBJS}

clean:
	rm -f ${BIN} ${OBJS}
