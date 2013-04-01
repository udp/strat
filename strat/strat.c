
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

#include "common.h"

strat_ctx strat_init (int argc, char * argv [])
{
   strat_ctx ctx = (strat_ctx) calloc (sizeof (*ctx), 1);

   if (!ctx)
   {
      trace ("Error allocating engine context");
      return 0;
   }

   if (! (ctx->config = sut_json_load ("config.json")))
   {
      trace ("Engine: Unable to load config.json\n");
      return 0;
   }

   ctx->win_width = sut_json_int (ctx->config, "winWidth", 800);
   ctx->win_height = sut_json_int (ctx->config, "winHeight", 600);
   ctx->fullscreen = sut_json_bool (ctx->config, "fullscreen", false);

   if (! (ctx->game_def = sut_json_load ("game/game.json")))
   {
      trace ("Engine: Unable to load game definition file\n");
      return 0;
   }

   ctx->game_title = sut_json_string (ctx->game_def, "title", strat_version);
   ctx->tick_rate = sut_json_int (ctx->game_def, "tickRate", 60);

   ctx->is_editor = (argc >= 2 && !strcasecmp (argv [1], "editor"));

   return ctx;
}

void strat_init_gfx (strat_ctx ctx)
{
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();

   glMatrixMode (GL_MODELVIEW);
   glLoadIdentity ();

   glViewport (0, 0, ctx->win_width, ctx->win_height);
   glOrtho (0, ctx->win_width, ctx->win_height, 0, 1.0f, -1.0f);

   glDisable (GL_CULL_FACE);
   glEnable (GL_TEXTURE_2D);

   glEnable (GL_BLEND);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   glEnableClientState (GL_VERTEX_ARRAY);
   glEnableClientState (GL_TEXTURE_COORD_ARRAY);

   font_init (&ctx->font, "Vera", 9);

   if (ctx->is_editor)
   {
      ctx->mode = editor_start (ctx);
   }
   else
   {
      ctx->mode = game_start (ctx);
   }
}

void strat_free (strat_ctx ctx)
{
   game_end (ctx, ctx->mode);

   free (ctx);
}

void strat_draw (strat_ctx ctx)
{
   glClearColor (0.5f, 0.5f, 0.5f, 1.0f);
   glClear (GL_COLOR_BUFFER_BIT);

   ctx->mode->draw (ctx, ctx->mode);

   GLenum error = glGetError();

   if (error != GL_NO_ERROR)
   {
      fprintf (stderr, "OpenGL error: %d\n", error);
   }
}

bool strat_tick (strat_ctx ctx)
{
   ctx->mode->tick (ctx, ctx->mode);

   return true;
}


