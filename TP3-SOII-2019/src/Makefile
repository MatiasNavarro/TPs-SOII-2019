all: ksamp formulario modulos

ksamp: ksamp.c
	gcc -Wall -Werror -pedantic -o ksamp.cgi ksamp.c

formulario: formulario.c 
	gcc -Wall -Werror -pedantic -o formulario.cgi formulario.c 

modulos: modulos.c
	gcc -Wall -Werror -pedantic -o modulos.cgi modulos.c 

cppcheck:
	cppcheck --enable=all .

clean: 
	rm ksamp.cgi formulario.cgi modulos.cgi 
