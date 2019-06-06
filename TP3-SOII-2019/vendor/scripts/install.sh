#!/bin/bash

find /var/www/html/uploads/ -type f | while read line; do
    name=$(basename "$line")
	sudo insmod $line
done

echo Location: modulos.cgi
echo
echo
