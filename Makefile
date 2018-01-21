COMPILE = gcc -c
OFILES = main.o socket_tools.o
TEST_PATH = ~nimrodav/socket_ex/
TEST_NAME = test.sh

all : $(OFILES)
.PHONY : all

clean :
	rm $(OFILES)
.PHONY : clean

test:
	$(TEST_PATH)$(TEST_NAME)
.PHONY : test

main.o : socket_tools.h
	$(COMPILE) main.c

socket_tools.o : socket_tools.h
	$(COMPILE) socket_tools.c

