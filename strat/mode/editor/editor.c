
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

static void draw (strat_ctx ctx, mode mode);
static void tick (strat_ctx ctx, mode mode);
static void cleanup (strat_ctx ctx, mode mode);

mode editor_start (strat_ctx ctx)
{
   mode_editor editor = calloc (sizeof (*editor), 1);

   editor->mode.tick = tick;
   editor->mode.draw = draw;
   editor->mode.cleanup = cleanup;

   unit_types_load (ctx, &editor->unit_types);

   map_init (ctx, &editor->map, editor->unit_types, "grass");
   camera_center (ctx, &editor->camera, 0, 0);

   editor_toolbar_init (ctx, &editor->toolbar);

   return (mode) editor;
}

void cleanup (strat_ctx ctx, mode mode)
{
   mode_editor editor = (mode_editor) mode;

   editor_toolbar_cleanup (ctx, &editor->toolbar);

   unit_types_unload (ctx, &editor->unit_types);

   free (editor);
}

void tick (strat_ctx ctx, mode mode)
{
   mode_editor editor = (mode_editor) mode;

   image_init (&editor->image_tile_hover, "game/ui/editor-tile-hover.png");

   camera_tick (ctx, &editor->map, &editor->camera);


   /* Find out which tile we're hovering over, if any.  The
    * editor_state_hovering flag is set if we're hovering over a tile inside
    * the map (used by the toolbar and when drawing).
    */

   editor->map_hover = screenspace_to_mapspace (&editor->camera,
                                                ctx->cursor.x,
                                                ctx->cursor.y);

   if (editor->map_hover.x >= 0 &&
       editor->map_hover.y >= 0 &&
       editor->map_hover.x < editor->map.width &&
       editor->map_hover.y < editor->map.height)
   {
      editor->state |= editor_state_hovering;
   }
   else
   {
      editor->state &= ~ editor_state_hovering;
   }


   editor_toolbar_tick (ctx, &editor->toolbar);
}

void draw (strat_ctx ctx, mode mode)
{
   mode_editor editor = (mode_editor) mode;


   /* Draw the map */

   map_draw (ctx, &editor->camera, &editor->map);


   /* If we're hovering over a tile, draw the hover highlight image. */

   if (editor->state & editor_state_hovering)
   {
      vec2f p = mapspace_to_screenspace (&editor->camera,
                                         (int) editor->map_hover.x,
                                         (int) editor->map_hover.y);

      image_draw (&editor->image_tile_hover,
                  p.x - (editor->map.tile_width / 2),
                  p.y);
   }


   /* Draw the toolbar */

   editor_toolbar_draw (ctx, &editor->toolbar);
}

