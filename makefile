CC = gcc
OBJS = Party.o PartySystem.o
OBJS_DEBUG = Party_debug.o PartySystem_debug.o
ALL_O = $(OBJS) $(OBJS_DEBUG) test1.o test2.o test2_debug.o test.o
ND = -DNDEBUG
TEST1 = test1.exe
TEST2 = test2.exe
TEST2D = test2_debug.exe
TEST = test.exe
LIBFILE = -L. -lmtm
CFLAGS = -std=c99 -Wall -Werror -pedantic-errors

# Need to explain

$(TEST1) : $(OBJS) $*.o
	$(CC) -o $@ $(ND) $(OBJS) $(LIBFILE)
test1.o : test1.c Party.h PartySystem.h
	$(CC) -c $(ND) $(CFLAGS) $*.c
Party.o : Party.c Party.h list.h
	$(CC) -c $(ND) $(CFLAGS) $*.c
PartySystem.o : PartySystem.c PartySystem.h set.h
	$(CC) -c $(ND) $(CFLAGS) $*.c

$(TEST2) : $(OBJS) $*.o
	$(CC) -o $@ $(ND) $(OBJS) $(LIBFILE)

$(TEST2D) : $(OBJS_DEBUG) $*.o
	$(CC) -o $@ $(OBJS) $(LIBFILE)
test2_debug.o : test2.c Party.h PartySystem.h
	$(CC) -c $(CFLAGS) $*.c -o test2_debug.o
Party_debug.o : Party.c Party.h list.h
	$(CC) -c $(CFLAGS) $*.c -o Party_debug.o
PartySystem_debug.o : PartySystem.c PartySystem.h set.h
	$(CC) -c $(CFLAGS) $*.c -o PartySystem_debug.o

$(TEST) : $(OBJS) $*.o
	$(CC) -o $@ $(ND) $(OBJS) $(LIBFILE)

clean :
	rm -f $(ALL_O) $(TEST1) $(TEST2) $(TEST2D) $(TEST)
