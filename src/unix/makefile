all: servidor cliente

servidor : servidor_u_cc.c
	gcc -Wall -Werror -pedantic -o servidor_u servidor_u_cc.c 

cliente : cliente_u_cc.c 
	gcc -g -Wall -Werror -pedantic -o cliente_u cliente_u_cc.c 

clean : 
	rm servidor_u cliente_u earth.jpg\
