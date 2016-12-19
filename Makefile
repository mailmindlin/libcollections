LIB_NAME := collections

rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

CFLAGS += -Wall -Wpointer-arith -Wextra -Wmissing-prototypes -Wstrict-prototypes -Wconversion -Wunused-function
CFLAGS += -fPIC -ftree-vectorize -fvisibility=hidden -flto -std=c11 -I. -Os -ggdb
LDFLAGS += -lm -lc -shared -flto=3

SRC := $(call rwildcard, ./src/, *.c)
OBJ := $(patsubst %.c,%.o, $(SRC))

LIB = $(LIB_NAME).so

all: $(LIB)

$(LIB): $(OBJ)
	$(CC) $(LDFLAGS) -Wl,-soname.,$(LIB) $(OBJ) -o $(LIB)

