TARGET := interp
SRCS   := cl_getc.c dict.c eval.c parser.c stack.c clesson.h

CC     := gcc

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) -g $^ -o $@

memcheck:
	valgrind --leak-check=full ./$(TARGET)

clean:
	-rm -f $(TARGET)
