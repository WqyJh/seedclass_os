OBJS = common.o

INCS = common.h

LIBS = -lpthread

CFLAGS = -std=c99 -O3 -Werror -Wfatal-errors -Wall -Wextra -D_XOPEN_SOURCE

multisum: $(OBJS)
	cc -o multisum multisum_thread.c $(OBJS) $(LIBS) $(CFLAGS)

$(OBJS): %.o: %.c $(INCS)
	cc -std=c99 -c $< -o $@

.PHONY: clean install uninstall
clean:
	rm -f multisum $(OBJS) output.txt
