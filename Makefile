# Compiler and flags
CC = gcc
CFLAGS = -std=c11 -Wall -Werror

# Object files
OBJS = cmd_parser.o dir_traverse.o file_ops.o filesync.o hashtable.o list.o mysync.o utils.o task_queue.o glob2regex.o pattern_match.o

# Target executable
TARGET = mysync

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

cmd_parser.o: cmd_parser.c cmd_parser.h utils.h glob2regex.h
	$(CC) $(CFLAGS) -c cmd_parser.c

dir_traverse.o: dir_traverse.c dir_traverse.h utils.h mysync.h cmd_parser.h pattern_match.h
	$(CC) $(CFLAGS) -c dir_traverse.c

file_ops.o: file_ops.c mysync.h task_queue.h
	$(CC) $(CFLAGS) -c file_ops.c

filesync.o: filesync.c mysync.h
	$(CC) $(CFLAGS) -c filesync.c

hashtable.o: hashtable.c hashtable.h list.h utils.h
	$(CC) $(CFLAGS) -c hashtable.c

list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c

mysync.o: mysync.c mysync.h cmd_parser.h dir_traverse.h utils.h
	$(CC) $(CFLAGS) -c mysync.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

task_queue.o: task_queue.c task_queue.h
	$(CC) $(CFLAGS) -c task_queue.c

glob2regex.o: glob2regex.c glob2regex.h
	$(CC) $(CFLAGS) -c glob2regex.c

pattern_match.o: pattern_match.c pattern_match.h
	$(CC) $(CFLAGS) -c pattern_match.c

clean:
	rm -f $(OBJS) $(TARGET)
