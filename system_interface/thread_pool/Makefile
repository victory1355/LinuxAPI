CC = gcc


TARGET = main


SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))


$(TARGET):$(OBJS)
	$(CC) $(OBJS) -o $(TARGET) -lpthread -DDEBUG

$(OBJS):%.o:%.c
	$(CC) -c $^ -o $@ -lpthread -DDEBUG


clean:
	rm *.o 

