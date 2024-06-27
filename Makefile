CC = clang
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lSDL2 -lSDL2_mixer -lSDL2_ttf 

SRC = game.c
OBJ = game.o functions.o LetsTest.o
EXEC = game

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

game.o: game.c
	$(CC) $(CFLAGS) -c $<

functgame.o: functions.c
	$(CC) $(CFLAGS) -c $<
	
LetsTest.o: LetsTest.c 
	$(CC) $(CFLAGS) -c $<

run: $(EXEC)
	./$(EXEC)

clean:
	rm -f $(OBJ) $(EXEC)

