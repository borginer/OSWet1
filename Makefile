# Makefile for the smash program
CC = gcc
CFLAGS = -g -Wall
CCLINK = $(CC)
OBJS = smash.o commands.o signals.o job_list.o
RM = rm -f
# Creating the  executable
smash: $(OBJS)
	$(CCLINK) -o smash $(OBJS)
# Creating the object files
commands.o: commands.c commands.h
smash.o: smash.c commands.h
signals.o: signals.c signals.h
job_list.o: job_list.c job_list.h

# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ "#"* core.*

