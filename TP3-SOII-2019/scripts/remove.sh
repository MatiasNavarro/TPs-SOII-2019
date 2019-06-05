#!/bin/bash

find /var/www/html/uploads/ -type f | while read line; do
    name=$(basename "$line" ".ko")
	sudo rmmod $name
	rm $line
done

echo Location: modules.cgi
echo
echo
