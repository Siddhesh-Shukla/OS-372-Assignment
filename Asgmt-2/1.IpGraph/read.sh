#! /bin/sh

echo Taking Input...
gcc readfile.c -o readfile -lpthread
./readfile
echo Done!
python3 plotting.py