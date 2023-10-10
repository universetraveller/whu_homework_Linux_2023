this-makefile := $(lastword $(MAKEFILE_LIST))
abs_srctree := $(realpath $(dir $(this-makefile)))
srctree := $(realpath $(abs_srctree)/../)
VPATH = $(srctree)/include
CC = gcc

objects = main

PHONY = all

all: $(objects)

#if we write 'main.o: main.c' or adding 'main.o: main.c', make will consider we make this rule empty so the implicit rule would not make effect

#main.o: main.c 

#main: main.o
#	$(CC) -g -o main main.o

	
# main.o: main.c
#	$(CC) -g -c main.c

PHONY += clean
clean:
	rm -f $(abs_srctree)/*.o
	rm -f $(abs_srctree)/main

PHONY += print_srctree
print_srctree:
	@echo 'srctree: '$(srctree)
	@echo 'VPATH: '$(VPATH)

PHONY += run
run:
	@$(abs_srctree)/main

.PHONY : $(PHONY)
