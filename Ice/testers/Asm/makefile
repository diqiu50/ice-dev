
#all: test.out

#test.out:main.cpp
#	gcc -g -std=c++0x main.cpp -o test.out


all: hello.out

hello.out:hello.o
	ld -s -o hello.out hello.o 

hello.o:hello.s
	as -o hello.o hello.s 

#hello.o:hello.asm
#	nasm -f elf64 hello.asm



clean:
	rm test.out hello.out hello.o
