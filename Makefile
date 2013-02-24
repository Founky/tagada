CC = gcc
LD = ld
CFLAGS = -Wall -g -Werror
LDFLAGS =
INCLUDES = src
SOURCES = src/main.c src/core.c src/instructions.c src/ram.c
TSOURCES = 
OBJECTS = $(call SRC_2_OBJ, $(SOURCES))
dir	:= tests
include	$(dir)/rules.mk
TOBJECTS = $(call TESTS_2_OBJ, $(TSOURCES)) $(OBJECTS:build/main.o=)

define TESTS_2_OBJ
    $(foreach src,$(1),$(patsubst tests/%.c,build/%.o,$(src)))
endef

define SRC_2_OBJ
    $(foreach src,$(1),$(patsubst src/%.c,build/%.o,$(src)))
endef

all: tagada

tagada: $(OBJECTS)
	@echo "  [LD]    $^ -> $@"
	@mkdir -p bin
	@$(CC) $^ -o bin/$@ $(CFLAGS) 

test: $(TOBJECTS)
	@echo "  [LD]    $^ -> $@"
	@mkdir -p bin
	@$(CC) -lcunit $^ -o bin/$@ $(CFLAGS)

runtest: test
	@bin/test

build/%.o: src/%.c
	@mkdir -p $(dir $@)
	@echo "  [CC]    $< -> $@"
	@$(CC) -I$(INCLUDES) $(CFLAGS) -o $@ -c $<

build/%.o: tests/%.c
	@mkdir -p $(dir $@)
	@echo "  [CC]    $< -> $@"
	@$(CC) -I$(INCLUDES) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf $(OBJECTS)
	rm -rf $(TOBJECTS)
	rm -f bin/tagada

info:
	@echo $(SOURCES)
	@echo $(OBJECTS)
	@echo $(TOBJECTS)
