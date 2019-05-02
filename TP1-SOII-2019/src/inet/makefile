all: servidor cliente cppcheck

servidor : servidor_i_cc.c
	gcc -Wall -Werror -pedantic -o servidor_i servidor_i_cc.c

cliente : cliente_i_cc.c 
	gcc -g -Wall -Werror -pedantic -o cliente_i cliente_i_cc.c 

cppcheck:
	cppcheck --enable=all .

clean : 
	rm servidor_i cliente_i earth.jpg cliente_i_cc_v1_1.c\
