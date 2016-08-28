SOURCE=engine/*.c main.c
EXE=fortress
INCLUDE=./include/engine
all:
	gcc -o $(EXE) $(SOURCE) -I $(INCLUDE)
