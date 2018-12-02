TARGET := interp
SRCS   := cl_getc.c dict.c eval.c parser.c stack.c clesson.h

CC     := gcc

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) -g -Wall $^ -o $@

dict: cl_getc.c dict.c parser.c clesson.h
	$(CC) -g -Wall -DTEST_DICT $^ -o $@

parser: cl_getc.c dict.c parser.c clesson.h
	$(CC) -g -Wall -DTEST_PARSER $^ -o $@

memcheck:
	valgrind --leak-check=full ./$(TARGET)

clean:
	-rm -f $(TARGET) dict parser
