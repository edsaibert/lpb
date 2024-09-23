CC = gcc
CFLAGS = -Wall -g 
IO = ./io

TARGET = lbp 
c_files = lbp.c $(IO)/io.c
objects = $(c_files:.c=.o) 

default: $(TARGET)

$(TARGET): $(objects) 
		$(CC) $(CFLAGS) -o $@ $^ 

%.o: %.c
		$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm $(TARGET) $(objects)