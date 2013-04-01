 
/* vim: set et ts=3 sw=3 ft=c:
 *
 * Copyright (C) 2013 James McLaughlin & Anders Riggelsen.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "matrix.h"

#ifdef _MSC_VER
   #include "../deps/glew/include/GL/glew.h"
   #include "../msvc/typeof.h"
   #include "../msvc/dirent.h"
   #define snprintf sprintf_s
   #define strcasecmp stricmp
#else
   #include <dirent.h>
   #include <stdbool.h>
   #ifdef __APPLE__
      #include <opengl/gl.h>
   #else
      #include <GL/gl.h>
   #endif
#endif

#include "../deps/libpng/png.h"

#include "../deps/json-parser/json.h"
#include "../deps/uthash/uthash.h"
#include "../deps/list/list.h"

#include "../deps/freetype-gl/vertex-buffer.h"
#include "../deps/freetype-gl/texture-font.h"

typedef struct _strat_ctx * strat_ctx;
typedef struct _camera * camera;

#include "util.h"
#include "image.h"
#include "tile.h"
#include "font.h"
#include "unit-type.h"
#include "unit.h"
#include "map.h"
#include "camera.h"

#include "mode/mode.h"
#include "mode/game/game.h"
#include "mode/editor/editor.h"

#define strat_version "Strat 0.1.0"
#define strat_max_path 512

struct _strat_ctx
{
   json_value * game_def, * config;

   long win_width, win_height;
   bool fullscreen;

   vec2i cursor;

   struct _strat_font font;

   const char * game_title;
   int tick_rate;

   mode mode;

   bool is_editor;
};

strat_ctx strat_init (int argc, char * argv []);
void strat_free (strat_ctx);

bool strat_tick (strat_ctx);

void strat_init_gfx (strat_ctx ctx);
void strat_draw (strat_ctx);

#define trace(...) do { \
    fprintf(stderr, __VA_ARGS__); \
    fprintf(stderr, "\n"); \
} while(0);

#define key_left_arrow   1
#define key_right_arrow  2
#define key_up_arrow     3
#define key_down_arrow   4
#define key_left_mouse   5
#define key_right_mouse  6

bool key_down (int key);


