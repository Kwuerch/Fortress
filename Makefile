SOURCE=engine/*.c test/*.c main.c
EXE=fortress
INCLUDES += -I ./include \
			   -I ./include/test \
				-I ./include/engine
all:
	gcc -g -o $(EXE) $(SOURCE) $(INCLUDES)
