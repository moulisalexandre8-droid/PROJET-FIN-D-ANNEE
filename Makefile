CC = gcc

SRC = code/main.c \
      code/game/game.c \
      code/game/board.c \
      code/players/player.c \
      code/ui/renderer.c \
      code/ui/window.c \
	code/ui/buttons.c \
	code/ui/text.c \
      code/game/de.c \
      code/game/turn_manager.c \
      code/entities/room.c \
      code/game/cards.c \
      code/utils/loader.c

LIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image

EXEC = cluelau.exe

all:
	$(CC) $(SRC) $(LIBS) -o $(EXEC)

run:
	./$(EXEC)

clean:
	rm -f $(EXEC)