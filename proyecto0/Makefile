# ─────────────────────────────────────────────────────────────────────────────
# Makefile  —  Proyecto 0 · CI3825 Sistemas de Operación I
# ─────────────────────────────────────────────────────────────────────────────

CC      = gcc
CFLAGS  = -Wall -Wextra -g -Iinclude

TARGET  = rr_sim
SRCS    = main.c src/rr.c
OBJS    = $(SRCS:.c=.o)

# ── Compilación ───────────────────────────────────────────────────────────────
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c include/rr.h
	$(CC) $(CFLAGS) -c -o $@ $<

src/rr.o: src/rr.c include/rr.h
	$(CC) $(CFLAGS) -c -o $@ $<

# ── Pruebas automáticas ───────────────────────────────────────────────────────
test: $(TARGET)
	@bash tests/evaluar.sh

# ── Limpieza ──────────────────────────────────────────────────────────────────
clean:
	rm -f $(TARGET) main.o src/rr.o

.PHONY: all test clean
