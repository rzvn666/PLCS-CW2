CC = gcc
CFLAGS = -g -Wall
OBJFILES = source.o
TARGET = scanner

all: $(TARGET)

$(TARGET) : $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES)

clean:
	rm -f $(OBJFILES) $(TARGET) *~
