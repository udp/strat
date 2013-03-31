
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

void ui_init (strat_ctx ctx, strat_ui ui)
{

}

void ui_tick (strat_ctx ctx, strat_ui ui)
{

}

void ui_draw (strat_ctx ctx, strat_ui ui)
{
   mode_game game = (mode_game) ctx->mode;


   char status [128];
   sprintf (status, "Camera: %f, %f", game->camera.pos.x, game->camera.pos.y);
   text_draw (&ctx->ui_font, 0, 40, 0, 0, status, 0);

   vec2f m = screenspace_to_mapspace (&game->camera, ctx->cursor.x, ctx->cursor.y);
   sprintf (status, "Mouse: %f, %f", m.x, m.y);
	
   text_draw (&ctx->ui_font, 0, 0, 0, 0, status, 0);


   char selected [128];
   sprintf (selected, "Selected: %d", (int) list_length (game->selected_units));
   text_draw (&ctx->ui_font, 0, ctx->win_height - 40, 0, 0, selected, 0);

}

void ui_done (strat_ctx ctx, strat_ui ui)
{

}


