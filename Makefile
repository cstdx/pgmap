CFLAGS         += -Wall -pedantic -Werror -Ideps -I.
DIRDEPS         = deps
SRC             = $(wildcard pgmap/*.c) $(wildcard tests/*.c)
DEPS            = $(wildcard $(DIRDEPS)/*/*.c)
OBJS            = $(DEPS:.c=.o) $(SRC:.c=.o)
DIRCMOCKERY     = $(DIRDEPS)/cmockery
CMOCKERYCONFIGH = $(DIRCMOCKERY)/config.h
TEST            = runtests

default: test

$(CMOCKERYCONFIGH):
	cd $(DIRCMOCKERY) && chmod u+x configure && ./configure

$(DIRCMOCKERY)/cmockery.o: CFLAGS += -w

%.o: %.c
	$(CC) $< -c -o $@ $(CFLAGS)

$(TEST): $(CMOCKERYCONFIGH) $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

# TODO: remove
test: CFLAGS += -w
test: $(TEST)
	@./$(TEST)

clean:
	$(foreach c, $(OBJS), rm -f $(c))
	rm -f $(TEST)
