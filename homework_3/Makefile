this-makefile := $(lastword $(MAKEFILE_LIST))
abs_srctree := $(realpath $(dir $(this-makefile)))
srctree := $(realpath $(abs_srctree)/../)
INCLUDE = $(srctree)/include
VPATH = $(abs_srctree):$(INCLUDE)
CC = gcc

objects = test_print_proc.o test_kill_proc.o

PHONY = all

all: $(objects)


$(objects): %.o : %.c
	$(CC) -g -I$(INCLUDE) $< -o $@

PHONY += clean
clean:
	rm -f $(abs_srctree)/*.o

PHONY += print_srctree
print_srctree:
	@echo 'srctree: '$(srctree)
	@echo 'VPATH: '$(VPATH)

PHONY += run
run:
	@$(abs_srctree)/test_print_proc.o
	@$(abs_srctree)/test_kill_proc.o

.PHONY : $(PHONY)
