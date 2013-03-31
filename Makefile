
CFLAGS = -std=gnu11 -O0 -g -Istrat

CFLAGS += -Ideps/json-parser
CFLAGS += -Ideps/libpng
CFLAGS += -Ideps/list
CFLAGS += -Ideps/freetype/include
CFLAGS += -Ideps/freetype-gl
CFLAGS += -Ideps/glfw/include

LDFLAGS += -lz

PLATFORM = $(shell uname -s | tr '[A-Z]' '[a-z]')

ifeq ($(PLATFORM), darwin)
	LDFLAGS += -framework OpenGL
	LDFLAGS += -framework CoreServices
	LDFLAGS += -framework Cocoa
	LDFLAGS += -framework IOKit
	LDFLAGS += -lbz2
	GLFW_TARGET = cocoa
endif

ifeq ($(PLATFORM), linux)
	LDFLAGS += -lGL -lm -lX11 -lpthread -lrt -lXrandr
	GLFW_TARGET = x11
endif

OBJECTS = \
build/json.o \
build/list.o \
build/strat.o \
build/util.o \
build/tile.o \
build/map.o \
build/image.o \
build/font.o \
build/unit-type.o \
build/unit.o \
build/matrix.o \
build/camera.o \
build/game.o \
build/editor.o \
build/game-ui.o

OBJECTS += \
build/ftgl-texture-font.o \
build/ftgl-texture-atlas.o \
build/ftgl-vector.o \
build/ftgl-add-text.o \
build/ftgl-vertex-buffer.o \
build/ftgl-vertex-attribute.o \

all: dist/run

clean:
	@rm -f build/*

dist/run: build/glfw.a build/libpng.a build/libfreetype.a build/libglfw.a $(OBJECTS)
	$(CC) $(OBJECTS) build/*.a $(LDFLAGS) -o $@

dist/editor: build/glfw.a build/libpng.a build/libfreetype.a $(OBJECTS)
	$(CC) $(LDFLAGS) build/*.a $(OBJECTS) -o $@


## global
##########

build/json.o: deps/json-parser/json.c
	$(CC) $(CFLAGS) deps/json-parser/json.c -c -o $@

build/list.o: deps/list/list.c
	$(CC) $(CFLAGS) deps/list/list.c -c -o $@

build/strat.o: strat/strat.c
	$(CC) $(CFLAGS) strat/strat.c -c -o $@

build/util.o: strat/util.c
	$(CC) $(CFLAGS) strat/util.c -c -o $@

build/tile.o: strat/tile.c
	$(CC) $(CFLAGS) strat/tile.c -c -o $@

build/map.o: strat/map.c
	$(CC) $(CFLAGS) strat/map.c -c -o $@

build/image.o: strat/image.c
	$(CC) $(CFLAGS) strat/image.c -c -o $@

build/font.o: strat/font.c
	$(CC) $(CFLAGS) strat/font.c -c -o $@

build/unit-type.o: strat/unit-type.c
	$(CC) $(CFLAGS) strat/unit-type.c -c -o $@

build/unit.o: strat/unit.c
	$(CC) $(CFLAGS) strat/unit.c -c -o $@

build/camera.o: strat/camera.c
	$(CC) $(CFLAGS) strat/camera.c -c -o $@

build/matrix.o: strat/matrix.c
	$(CC) $(CFLAGS) strat/matrix.c -c -o $@

build/game.o: strat/mode/game/game.c
	$(CC) $(CFLAGS) strat/mode/game/game.c -c -o $@

build/game-ui.o: strat/mode/game/ui.c
	$(CC) $(CFLAGS) strat/mode/game/ui.c -c -o $@

build/editor.o: strat/mode/editor/editor.c
	$(CC) $(CFLAGS) strat/mode/editor/editor.c -c -o $@


## glfw
##########

GLFW_FLAGS = $(CFLAGS) -Ipdeps/glfw/include 

build/glfw.a: build/glfw-main.o
	ar rcs $@ build/glfw-*

build/libglfw.a:
	make -C deps/glfw $(GLFW_TARGET)
	cp -f deps/glfw/lib/$(GLFW_TARGET)/libglfw.a build/

build/glfw-main.o: platform/glfw/main.c
	$(CC) $(GLFW_FLAGS) platform/glfw/main.c -c -o $@


## libpng
##########

build/libpng.a: deps/libpng/libpng.a
	cp -f deps/libpng/libpng.a build/

deps/libpng/libpng.a:
	make -C deps/libpng -f scripts/makefile.$(PLATFORM);


## freetype
############

build/libfreetype.a: deps/freetype/objs/.libs/libfreetype.a
	cp -f deps/freetype/objs/.libs/libfreetype.a build/

deps/freetype/objs/.libs/libfreetype.a:
	make -C deps/freetype


## freetype-gl
#############

build/ftgl-texture-font.o: deps/freetype-gl/texture-font.c
	$(CC) $(CFLAGS) deps/freetype-gl/texture-font.c -c -o $@

build/ftgl-texture-atlas.o: deps/freetype-gl/texture-atlas.c
	$(CC) $(CFLAGS) deps/freetype-gl/texture-atlas.c -c -o $@

build/ftgl-vector.o: deps/freetype-gl/vector.c
	$(CC) $(CFLAGS) deps/freetype-gl/vector.c -c -o $@

build/ftgl-add-text.o: deps/freetype-gl/add-text.c
	$(CC) $(CFLAGS) deps/freetype-gl/add-text.c -c -o $@

build/ftgl-vertex-buffer.o: deps/freetype-gl/vertex-buffer.c
	$(CC) $(CFLAGS) deps/freetype-gl/vertex-buffer.c -c -o $@

build/ftgl-vertex-attribute.o: deps/freetype-gl/vertex-attribute.c
	$(CC) $(CFLAGS) deps/freetype-gl/vertex-attribute.c -c -o $@


.PHONY: clean all


