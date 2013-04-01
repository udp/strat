
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

bool unit_init_json (unit_type types, unit unit, json_value * json)
{
   if (json->type != json_object)
      return false;

   const char * type_id = sut_json_string (json, "type", "");

   unit_type type = 0;
   HASH_FIND (hh, types, type_id, strlen (type_id), type);

   if (!type)
   {
      trace ("Unknown unit type: %s", type_id);
      return false;
   }

   unit_type_load (type);
   unit->type = type;

   unit->x = sut_json_int (json, "x", 0);
   unit->y = sut_json_int (json, "y", 0);

   unit->selected = false;

   trace ("Loaded unit from JSON: %s", type->name);

   return true;
}

bool unit_init (unit_type types, unit unit, unit_type type)
{
   assert (false);

   return true;
}

void unit_cleanup (unit unit)
{

}

void unit_draw (strat_ctx ctx, camera camera, unit unit)
{
   vec2f p = mapspace_to_screenspace (camera, unit->x, unit->y);

   image image = &unit->type->image.stand;

   if (unit->selected)
   {
      glDisable (GL_TEXTURE_2D);
      glColor4f (0.8f, 0.8f, 0.8f, 0.8f);

      int x0 = p.x - (image->width / 2),
          x1 = x0 + image->width,
          y0 = p.y + 4,
          y1 = y0 + 1;

      GLfloat vertices [] =
      {
         x0,  y0,
         x1,  y0,
         x1,  y1,
         x0,  y1
      };

      glVertexPointer (2, GL_FLOAT, 0, vertices);
      glDrawArrays (GL_LINE_LOOP, 0, 4);

      glEnable (GL_TEXTURE_2D);
      glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
   }

   image_draw (image, p.x - image->width / 2, p.y - image->height);
}


