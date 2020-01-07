@echo off
echo 写入.rc文件
echo A ICON "./icon.ico" > res.rc
echo 生成.res文件
windres -i res.rc --input-format=rc -o my.res -O coff
echo 编译
gcc -c -o main.o main.c
gcc -c -o rainbow_CORE.o rainbow_CORE.c
gcc -c -o rainbow_UI.o rainbow_UI.c
gcc -c -o rainbow_IO.o rainbow_IO.c
echo 链接
gcc main.o rainbow_CORE.o rainbow_UI.o rainbow_IO.o my.res -o Rainbow.exe
echo 删除临时文件
del res.rc
del my.res
del main.o
del rainbow_CORE.o
del rainbow_UI.o
del rainbow_IO.o