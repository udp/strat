
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

void editor_toolbar_init (strat_ctx ctx, editor_toolbar toolbar)
{
   font_init (&toolbar->font, "Vera", 12);
}

void editor_toolbar_cleanup (strat_ctx ctx, editor_toolbar toolbar)
{
   font_cleanup (&toolbar->font);
}

void editor_toolbar_tick (strat_ctx ctx, editor_toolbar toolbar)
{

}

void editor_toolbar_draw (strat_ctx ctx, editor_toolbar toolbar)
{
   mode_editor editor = (mode_editor) ctx->mode;

   char status [128];
   *status = 0;

   /* Hovering over a tile?  Set status to the coordinates and tile type. */

   if (editor->state & editor_state_hovering)
   {
      int x = (int) editor->map_hover.x;
      int y = (int) editor->map_hover.y;

      tile tile = editor->map.tiles [y * editor->map.width + x];
      int elevation = editor->map.elevation [y * editor->map.width + x];

      char elevation_str [128];
      *elevation_str = 0;

      if (elevation > 0)
         sprintf (elevation_str, " (elevation %d up)", abs (elevation));
      else if (elevation < 0)
         sprintf (elevation_str, " (elevation %d down)", abs (elevation));

      sprintf (status, "Tile (%d, %d): %s%s", x, y, tile->name, elevation_str);

   }

   if (*status)
      text_draw (&toolbar->font, 0, 0, 128, 64, status, 0);
}


