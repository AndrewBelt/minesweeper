
CFLAGS = -Wall -g
INCLUDE = \
	-I/usr/local/opt/ncurses/include
LIBS = \
	-L/usr/local/opt/ncurses/lib \
	-lncurses

SRCS = \
	main.c \
	game.c \
	grid.c \
	help.c \
	hud.c

OBJS = $(SRCS:%.c=%.o)

# Targets

all: minesweeper

minesweeper: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $^ $(INCLUDE)

clean:
	rm -f *.o minesweeper

run: minesweeper
	./minesweeper
