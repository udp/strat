
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

bool map_init (strat_ctx ctx,
               map map,
               unit_type unit_types,
               const char * name)
{
   char filename [strat_max_path];
   snprintf (filename, sizeof (filename), "game/map/%s.json", name);

   if (! (map->json = sut_json_load (filename)))
   {
      trace ("Error loading JSON for map %s", name);
      return false;
   }

   map->width = sut_json_int (map->json, "width", 4);
   map->height = sut_json_int (map->json, "height", 4);


   /** Units **/

   json_value * units = sut_json_value (map->json, "units");

   if (units && units->type == json_array)
   {
      for (size_t i = 0; i < units->u.array.length; ++ i)
      {
         struct _unit unit;
         unit_init_json (unit_types, &unit, units->u.array.values [i]);
         list_push (map->units, unit);
      }
   }
   else
   {
      trace ("Map %s has no units?", name);
   }

   trace ("Loaded map %s", name);

   return map;
}

void map_cleanup (map map)
{
}

void map_draw (strat_ctx ctx, camera camera, map map)
{
}

