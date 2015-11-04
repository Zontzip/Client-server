Client Server

================
== SSH Login
================

ssh C13451458@147.252.234.34

password: test12345

================
== UNIX Commands
================

-- Create C file 
c13451458@apollo:~/myprogs/intro$ gcc -g -O2 -Wall -c -o daytimeserver.o daytimeserver.c

-- Run program with libunp archive file
c13451458@apollo:~/myprogs/intro$ gcc -g -O2 -Wall -o daytimeserver daytimeserver.o ../libunp.a