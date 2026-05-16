CC = gcc

SRC = code/main.c \
      code/game/game.c \
      code/ui/renderer.c

LIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf

EXEC = cluelau.exe

all:
	$(CC) $(SRC) $(LIBS) -o $(EXEC)

run:
	./$(EXEC)

clean:
	rm -f $(EXEC)