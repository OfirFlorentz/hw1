CC = gcc
OBJS = Party.o PartySystem.o Person.o
OBJS_DEBUG = Party_debug.o PartySystem_debug.o Person_debug.o
ND = -DNDEBUG
D = -g
TEST1 = test1.exe
TEST2 = test2.exe
TEST2D = test2_debug.exe
TEST = test.exe
LIBFILE = -L. -lmtm
CFLAGS = -std=c99 -Wall -Werror -pedantic-errors
ALL_O_FILES = $(OBJS) $(OBJS_DEBUG) test1.o test2.o test2_debug.o test.o
ALL_EXE_FILES = $(TEST1) $(TEST2) $(TEST2D) $(TEST)

#For the command make:
$(TEST1) : $(OBJS) $*.o
	$(CC) -o $@ $(ND) $(OBJS) $(LIBFILE)

#For the command make test2.exe
$(TEST2) : $(OBJS) $*.o
	$(CC) -o $@ $(ND) $(OBJS) $(LIBFILE)

#For the command test2_debug.exe
$(TEST2D) : $(OBJS_DEBUG) $*.o
	$(CC) -o $@ $(D) $(OBJS) $(LIBFILE)

#For the command make test
$(TEST) : $(OBJS) $*.o
	$(CC) -o $@ $(ND) $(OBJS) $(LIBFILE)

#All of the object files in release mode
test1.o : test1.c Party.h PartySystem.h
	$(CC) -c $(ND) $(CFLAGS) $*.c
Party.o : Party.c Party.h list.h Person.h
	$(CC) -c $(ND) $(CFLAGS) $*.c
PartySystem.o : PartySystem.c PartySystem.h set.h
	$(CC) -c $(ND) $(CFLAGS) $*.c
Person.o : Person.h
	$(CC) -c $(ND) $(CFLAGS) $*.c

#All of the object files in debug mode
test2_debug.o : test2.c Party.h PartySystem.h
	$(CC) -c $(D) $(CFLAGS) $*.c -o $@
Party_debug.o : Party.c Party.h list.h Person.h
	$(CC) -c $(D) $(CFLAGS) $*.c -o $@
PartySystem_debug.o : PartySystem.c PartySystem.h set.h
	$(CC) -c $(D) $(CFLAGS) $*.c -o $@
Person_debug.o : Person.h
	$(CC) -c $(D) $(CFLAGS) $*.c -o $@

#For the command make clean, erase all of the .o and .exe files
clean :
	rm -f $(ALL_O_FILES) $(ALL_EXE_FILES)