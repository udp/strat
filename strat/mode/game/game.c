
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

mode game_start (strat_ctx ctx)
{
   mode_game game = calloc (sizeof (*game), 1);

   ui_init (ctx, &game->ui);

   game->mode.tick = tick;
   game->mode.draw = draw;
   game->mode.cleanup = cleanup;

   unit_types_load (ctx, &game->unit_types);

   map_init (ctx, &game->map, game->unit_types, "grass");

   list_each_elem (game->map.units, unit)
   {
      list_push (game->units, *unit);
   }

   /*camera_center (game, (game->map.width * game->map.tile_width) / 2,
                       (game->map.height * game->map.tile_height) / 2);*/

   camera_center (ctx, &game->camera, 0, 0);

   return (mode) game;
}

void cleanup (strat_ctx ctx, mode mode)
{
   mode_game game = (mode_game) mode;

   unit_types_unload (ctx, &game->unit_types);

   ui_cleanup (ctx, &game->ui);

   free (game);
}

static void tick (strat_ctx ctx, mode mode)
{
   mode_game game = (mode_game) mode;

   if (key_down (key_left_mouse))
   {
      if (game->selection.start.x == 0)
      {
         game->selection.start.x = ctx->cursor.x + game->camera.pos.x;
         game->selection.start.y = ctx->cursor.y + game->camera.pos.y;
      }

      game->selection.end.x = ctx->cursor.x + game->camera.pos.x;
      game->selection.end.y = ctx->cursor.y + game->camera.pos.y;
   }
   else
   {
      if (game->selection.start.x)
      {
         vec2f start = game->selection.start,
               end = game->selection.end;

         if (end.x < start.x)
         {
            int x = end.x;
            end.x = start.x;
            start.x = x;
         }

         if (end.y < start.y)
         {
            int y = end.y;
            end.y = start.y;
            start.y = y;
         }

         list_each_elem (game->units, unit)
         {
            vec2f unit_pos = mapspace_to_screenspace (&game->camera, unit->x, unit->y);

            if (unit->x > start.x &&
                (unit->y - unit->type->height) > start.y &&
                (unit->x + unit->type->width) < end.x &&
                unit->y < end.y)
            {
               if (!unit->selected)
               {
                  trace ("%s selected", unit->type->name);

                  unit->selected = true;
                  list_push (game->selected_units, unit);
               }
            }
            else
            {
               if (unit->selected)
               {
                  trace ("%s deselected", unit->type->name);

                  unit->selected = false;
                  list_remove (game->selected_units, unit);
               }
            }
         }

         game->selection.start.x = 0;
      }
   }

   camera_tick (ctx, &game->map, &game->camera);

   ui_tick (ctx, &game->ui);
}

static void draw (strat_ctx ctx, mode mode)
{
   mode_game game = (mode_game) mode;

   map_draw (ctx, &game->camera, &game->map);

   list_each_elem (game->units, unit)
   {
      unit_draw (ctx, &game->camera, unit);
   }

   if (game->selection.start.x != 0)
   {
      glTranslatef (0.5f - game->camera.pos.x, 0.5f - game->camera.pos.y, 0);

      glDisable (GL_TEXTURE_2D);
      glColor4f (0.8f, 0.8f, 0.8f, 0.8f);

      int x0 = game->selection.start.x,
          y0 = game->selection.start.y,
          x1 = game->selection.end.x,
          y1 = game->selection.end.y;

      for (int i = 0; i < 2; ++ i)
      {
         GLfloat vertices [] =
         {
            x0,  y0,
            x1,  y0,
            x1,  y1,
            x0,  y1
         };

         glVertexPointer (2, GL_FLOAT, 0, vertices);
         glDrawArrays (GL_LINE_LOOP, 0, 4);

         x0 += 2;
         y0 += 2; 
           
         x1 -= 2;
         y1 -= 2;
      }

      glTranslatef (-0.5f + game->camera.pos.x, -0.5f + game->camera.pos.y, 0);

      glEnable (GL_TEXTURE_2D);
      glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
   }

   ui_draw (ctx, &game->ui);
}




