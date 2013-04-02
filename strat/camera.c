
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
#include "matrix.h"

void camera_center (strat_ctx ctx, camera camera, int x, int y)
{
   camera->pos.x = x - (ctx->win_width / 2);
   camera->pos.y = y - (ctx->win_height / 2);
}

vec2f screenspace_to_mapspace (camera camera, int x, int y)
{
	mat3f inv = mat3fInvert (camera->matrix);
	return mat3fTransformPoint (inv, x, y);
}

vec2f mapspace_to_screenspace (camera camera, int x, int y)
{
	return mat3fTransformPoint (camera->matrix, x, y);
}

void handle_accel (float * accel, int direction)
{
   if (direction > 0)
   {
      if (*accel <= 0)
         *accel = 1.0f;
      else
      {
         *accel += 0.1f;
         *accel *= 1.5f;

         if (*accel > 6.0f)
            *accel = 6.0f;
      }
   }
   else if (direction < 0)
   {
      if (*accel >= 0)
         *accel = -1.0f;
      else
      {
         *accel -= 0.1f;
         *accel *= 1.5f;

         if (*accel < -6.0f)
            *accel = -6.0f;
      }
   }
   else
   {
      if (*accel > 0)
      {
         *accel -= 0.2f;

         if (*accel < 0)
            *accel = 0;
      }
      else if (*accel < 0)
      {
         *accel += 0.2f;

         if (*accel > 0)
            *accel = 0;
      }
   }
}

void camera_tick (strat_ctx ctx, map map, camera camera)
{
   int x_direction = 0;

   if (key_down (key_left_arrow))
      x_direction = -1;

   if (key_down (key_right_arrow))
      x_direction = 1;

   handle_accel (&camera->accel_x, x_direction);
   camera->pos.x += camera->accel_x;

   int y_direction = 0;

   if (key_down (key_up_arrow))
      y_direction = -1;

   if (key_down (key_down_arrow))
      y_direction = 1;

   handle_accel (&camera->accel_y, y_direction);
   camera->pos.y += camera->accel_y;
	
	
	//Update camera matrix
	float scale = map->tile_width / sqrt(2);
	mat3f mat = mat3fMakeRotate(M_PI/4);
	mat = mat3fScale(mat, scale, scale/2);
	mat = mat3fTranslate(mat, -camera->pos.x, -camera->pos.y);
	camera->matrix = mat;
}



