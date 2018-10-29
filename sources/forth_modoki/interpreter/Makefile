TARGET := interp
SRCS   := cl_getc.c dict.c eval.c parser.c stack.c clesson.h

CC     := gcc

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) -g $^ -o $@

dict: cl_getc.c dict.c parser.c clesson.h
	$(CC) -g -DTEST_DICT $^ -o $@


memcheck:
	valgrind --leak-check=full ./$(TARGET)

clean:
	-rm -f $(TARGET) dict
