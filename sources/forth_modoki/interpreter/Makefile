TARGET := interp
SRCS   := cl_getc.c dict.c eval.c parser.c stack.c

CC     := gcc

all: $(TARGET)

$(TARGET): $(SRCS) clesson.h
	$(CC) -g -Wall -I. -o $@ $(filter %.c, $^)

dict: cl_getc.c dict.c parser.c clesson.h
	$(CC) -g -Wall -I. -DTEST_DICT -o $@ $(filter %.c, $^)

parser: cl_getc.c dict.c parser.c clesson.h
	$(CC) -g -Wall -I. -DTEST_PARSER -o $@ $(filter %.c, $^)

memcheck:
	valgrind --leak-check=full ./$(TARGET)

clean:
	-rm -f $(TARGET) dict parser
