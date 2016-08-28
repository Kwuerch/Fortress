SOURCE=engine/*.c main.c
EXE=fortress
INCLUDE=./include/engine
all:
	gcc -g -o $(EXE) $(SOURCE) -I $(INCLUDE)
