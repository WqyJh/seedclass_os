
INCS = ../thread/common.h

CFLAGS = -O3 -Werror -Wfatal-errors -Wall -Wextra

multisum:
	cc -o multisum multisum_process.c ../thread/common.c ../thread/common.h $(CFLAGS)

.PHONY: clean install uninstall
clean:
	rm -f multisum output.txt
