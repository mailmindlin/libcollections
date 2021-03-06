LIBDIR := /usr/local/lib
LIB_NAME := libcollections

#Installation directory prefix
PREFIX := /usr
#Directory where libvideo.so will be installed
LIB_INSTALL_DIR := $(PREFIX)/lib
MANDIR := $(PREFIX)/share/man
#Directory where the include files will be installed
INC_INSTALL_DIR := $(PREFIX)/include/collections
USER_INCLUDES:= ./src/collections.h ./src/queue/queue.h

CFLAGS += -Wall -Wpointer-arith -Wextra -Wmissing-prototypes -Wstrict-prototypes -Wconversion -Wunused-function
CFLAGS += -fPIC -ftree-vectorize -fvisibility=hidden -I. -Os -ggdb -std=gnu11
LDFLAGS += -lm -lc

# Detect GCC extensions by trial & exception (try to compile an empty file with the given flag)
testccflag=$(shell (echo | $(CC) -E - $1 > /dev/null 2>&1 && echo yep) || echo nope)
SUPPORT_LTO = $(call testccflag, -flto)
$(info LTO: $(SUPPORT_LTO))
ifeq ($(SUPPORT_LTO),yep)
	# Clang's LTO doesn't seem to be working.
	ifeq ($(shell $(CC) -v 2>&1 | grep -c "clang"),)
		CFLAGS += -flto
		LDFLAGS += -flto
	endif
endif
SUPPORT_PRETTY_OUT = $(call testccflag, -fdiagnostics-color=auto)
$(info color: $(SUPPORT_PRETTY_OUT))
ifeq ($(SUPPORT_PRETTY_OUT),yep)
	CFLAGS += -fdiagnostics-color=auto
	LDFLAGS += -fdiagnostics-color=auto
endif

rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))
SRC   = $(call rwildcard, ./src/, *.c)
OBJ   = $(patsubst %.c,%.o, $(SRC))
TESTS = $(call rwildcard, ./test/, *.c)

ifeq ($(LINKTYPE),static)
	LIB = $(LIB_NAME).a
else
	LIB = $(LIB_NAME).so
endif

all: $(LIB)

install: all
	mkdir -p $(DESTDIR)$(INC_INSTALL_DIR)
	rsync -zarv --prune-empty-dirs --chmod=644,+X --exclude "*-priv.h" --include "*/" --include "*.h" --exclude "*" "./src/" "$(DESTDIR)$(INC_INSTALL_DIR)"
	mkdir -p $(DESTDIR)$(LIB_INSTALL_DIR)
ifeq ($(LINKTYPE),static)
		install -m 644 $(LIB) $(DESTDIR)$(LIB_INSTALL_DIR)
else
		install -m 755 $(LIB) $(DESTDIR)$(LIB_INSTALL_DIR)
endif

includes: ./include $(patsubst ./src/%,./include/%,$(USER_INCLUDES))


uninstall:
	rm -rf $(DESTDIR)$(INC_INSTALL_DIR)
	rm -f $(DESTDIR)$(LIB_INSTALL_DIR)/$(LIB)

test: $(TESTS)

clean:
	rm -rvf $(OBJ) $(LIB) $(TESTS) $(LIB_NAME).so $(LIB_NAME).a ./include

$(LIB): $(OBJ)

./include:
	mkdir -p ./include

$(patsubst %.c,%,$(TESTS)): %: $(patsubst %, %.o, $(%))
	$(CC) $(LDFLAGS)

$(patsubst ./src/%,./include/%,$(USER_INCLUDES)): %:
	mkdir -p $(shell dirname $@)
	echo "Include " $@
	cp $(patsubst include/%,src/%,$@) $(shell dirname $@)
%.a:
	$(AR) cqs --plugin=$(shell gcc --print-file-name=liblto_plugin.so) $@ $^

%.so:
	$(CC) -shared $(LDFLAGS) -Wl,-soname,$(LIB) -o $(LIB) $^
