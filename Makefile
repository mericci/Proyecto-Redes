FILES1 := $(wildcard cliente-servidor/client/src/*.c)

program1: $(FILES1)
	gcc -o client $(FILES1) -I./cliente-servidor/client/src/

FILES2 := $(wildcard cliente-servidor/server/src/*.c)

program2: $(FILES2)
	gcc -o server $(FILES2) -I./cliente-servidor/server/src/
