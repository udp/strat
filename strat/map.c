
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
               strat_map map,
               unit_type unit_types,
               tile tiles,
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


   /** Tiles **/

   map->tiles = (tile *) malloc (sizeof (tile) * map->width * map->height);
   map->elevation = (int8_t *) malloc (sizeof (int8_t) * map->width * map->height);

   if (!tile_init (ctx, &map->default_tile, sut_json_string (map->json, "defaultTile", "empty")))
   {
      trace ("Error loading default tile for map %s", name);
      return false;
   }

   map->tile_width = map->default_tile.image.width;
   map->tile_height = map->default_tile.image.height;

   json_value * tiles_json = sut_json_value (map->json, "tiles");

   if (tiles_json->type != json_array)
      tiles_json = 0;

   for (long y = 0; y < map->height; ++ y)
   {
      json_value * row_json = tiles_json ? tiles_json->u.array.values [y] : 0;

      if (row_json->type != json_array)
         row_json = 0;

      for (long x = 0; x < map->width; ++ x)
      {
         tile tile = 0;
         int8_t elevation = 0;

         if (row_json && row_json->u.array.length > x)
         {
            json_value * tile_json = row_json->u.array.values [x];

            if (tile_json->type == json_string)
            {
               const char * elevation_str = strchr (tile_json->u.string.ptr, ':');

               size_t name_length;
              
               if (elevation_str)
               {
                  name_length = (elevation_str - tile_json->u.string.ptr);
                  elevation = atoi (elevation_str + 1);
               }
               else
               {
                  name_length = tile_json->u.string.length;
               }

               HASH_FIND (hh, tiles, tile_json->u.string.ptr, name_length, tile);
            }
         }
         
         if (!tile)
            tile = &map->default_tile;

         map->tiles [y * map->width + x] = tile;
         map->elevation [y * map->width + x] = elevation;
      }
   }


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

   trace ("Loaded map %s (tile width = %d, tile height = %d)",
             name, map->tile_width, map->tile_height);

   return map;
}

void map_cleanup (strat_map map)
{
   tile_cleanup (&map->default_tile);
}

void map_draw (strat_ctx ctx, camera camera, strat_map map)
{
   int num_drawn = 0;

   for (int i = 0; i < map->height; ++ i)
   {
      for (int j = map->width - 1; j >= 0; -- j)
      {
         tile tile = map->tiles [i * map->width + j];
         int8_t elevation = map->elevation [i * map->width + j];

         vec2f p = mapspace_to_screenspace (camera, i, j);

         //Adjust for tile hotspot
         p.x -= map->tile_width/2;
		  
         image_draw (&tile->image, p.x, p.y);
      }
   }
}

