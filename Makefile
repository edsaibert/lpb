CC = gcc
CFLAGS = -Wall -g -lm
IO = ./io
DESC = ./descriptor

TARGET =  LBP
c_files = lbp.c $(IO)/io.c $(DESC)/descriptor.c
objects = $(c_files:.c=.o) 

default: $(TARGET)

$(TARGET): $(objects) 
		$(CC) $(CFLAGS) -o $@ $^ 

%.o: %.c
		$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm $(TARGET) $(objects)