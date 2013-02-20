CC = gcc
LD = ld
CFLAGS = -Wall -W -g -Werror
LDFLAGS = -lc --entry main
SOURCES = src/main.c
OBJECTS = $(call SRC_2_OBJ, $(SOURCES))

define SRC_2_OBJ
    $(foreach src,$(1),$(patsubst src/%.c,build/%.o,$(src)))
endef

all: tagada

tagada: $(OBJECTS)
	@echo "  [LD]    $^ -> $@"
	@mkdir -p bin
	@$(LD) $(LDFLAGS) $^ -o bin/$@

build/%.o: src/%.c
	@mkdir -p $(dir $@)
	@echo "  [CC]    $< -> $@"
	@$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf $(OBJECTS)

info:
	@echo $(SOURCES)
	@echo $(OBJECTS)
