CFLAGS = -DDEBUG -g -c -Wall -Wextra -Wwrite-strings
CFLAGS = -g -c -Wall -Wextra -Wwrite-strings
LFlags = -g

default: encDec

clean:
	rm -f *.o
	rm -f encDec
	
encDec.o: encDec.c
	gcc ${CFLAGS} encDec.c

encDec_funcs.o: encDec_funcs.c
	gcc ${CFLAGS} encDec_funcs.c
	
encDec: encDec.o encDec_funcs.o
	gcc ${LFLAGS} -o encDec encDec.o encDec_funcs.o
