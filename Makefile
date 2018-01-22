COMPILE = gcc -c
OFILES = ex3_lb.o socket_tools.o
TEST_PATH = ~nimrodav/socket_ex/
TEST_NAME = test.sh
GRANT_PERMISSION = chmod 755
NIMROD_TEST="$(shell $(TEST_PATH)$(TEST_NAME))"

ex3_lb: ex3_lb.o socket_tools.o
	@gcc $(OFILES) -o ex3_lb

all : ex3_lb $(OFILES)
.PHONY : all

clean :
	@rm $(OFILES)
.PHONY : clean

test:
	@echo $(NIMROD_TEST)
.PHONY : test

ex3_lb.o : socket_tools.h
	@$(COMPILE) ex3_lb.c
	@$(GRANT_PERMISSION) ex3_lb.o

socket_tools.o : socket_tools.h
	@$(COMPILE) socket_tools.c
	@$(GRANT_PERMISSION) socket_tools.o

