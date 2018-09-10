OBJS = common.o

INCS = common.h

LIBS = -lpthread

CFLAGS = -O3 -Werror -Wfatal-errors -Wall -Wextra

multisum: $(OBJS)
	cc -o multisum multisum_thread.c $(OBJS) $(LIBS) $(CFLAGS)

$(OBJS): $(INCS)

.PHONY: clean install uninstall
clean:
	rm -f multisum $(OBJS) output.txt
