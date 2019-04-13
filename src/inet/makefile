all: servidor cliente

servidor : servidor_i_cc.c
	gcc -Wall -Werror -pedantic -o servidor_i servidor_i_cc.c

cliente : cliente_i_cc.c 
	gcc -g -Wall -Werror -pedantic -o cliente_i cliente_i_cc.c 

clean : 
	rm servidor_i cliente_i earth.jpg\
