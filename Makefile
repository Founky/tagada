CC = gcc
LD = ld
CFLAGS = -Wall -g -Werror
LDFLAGS =
SOURCES = src/main.c src/core.c src/instructions.c
OBJECTS = $(call SRC_2_OBJ, $(SOURCES))

define SRC_2_OBJ
    $(foreach src,$(1),$(patsubst src/%.c,build/%.o,$(src)))
endef

all: tagada

tagada: $(OBJECTS)
	@echo "  [LD]    $^ -> $@"
	@mkdir -p bin
	@$(CC) $(CFLAGS) $^ -o bin/$@

build/%.o: src/%.c
	@mkdir -p $(dir $@)
	@echo "  [CC]    $< -> $@"
	@$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf $(OBJECTS)
	rm -f bin/tagada

info:
	@echo $(SOURCES)
	@echo $(OBJECTS)
