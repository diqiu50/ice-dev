all: hello
	
hello.o : hello.asm
	as -o $@ $<

hello : hello.o
	ld -o $@ $<

clean:
	rm hello hello.o
