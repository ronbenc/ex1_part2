CC = gcc
OBJS = election.o electionTestsExample.o
EXEC = election
#DEBUG_FLAG = #Currently none
COMP_FLAG = -std=c99 -Wall -pedantic-errors -Werror -DNDEBUG

$(EXEC) : $(OBJS)
	$(CC) $(DEBUG_FLAG) $(OBJS) -o $@ -L. -lmap

election.o: election.c election.h mtm_map/map.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
	
main.o: main.c election.h mtm_map/map.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
	
electionTestsExample.o: tests/electionTestsExample.c election.h mtm_map/map.h test_utilities.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) tests/$*.c

clean:
	rm -f $(OBJS) $(EXEC)