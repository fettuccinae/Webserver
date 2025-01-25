CC = gcc
CFLAGS = -lws2_32
SOURCES = only.c
OBJECTS = $(SOURCES:.c=.o)
EXEC = yn.exe

$(EXEC): $(OBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(CFLAGS)
	./$(EXEC)

.c.o:
	$(CC) -c $<

clean:
	del /f /q $(OBJECTS) $(EXEC)
