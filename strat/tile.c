
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

bool tile_init (strat_ctx ctx, tile tile, const char * name)
{
   if (! (tile->name = strdup (name)))
      return false;

   char filename [strat_max_path];
   snprintf (filename, sizeof (filename), "game/tile/%s.png", name);

   if (!image_init (&tile->image, filename))
      return false;

   tile->image.offset_x = (tile->image.width / 2);

   return true;
}

void tile_cleanup (tile tile)
{
   image_cleanup (&tile->image);

   free (tile->name);
}

bool tiles_load (strat_ctx ctx, tile * tiles)
{
   DIR * tile_dir = opendir ("game/tile");

   if (!tile_dir)
   {
      trace ("Couldn't open tile dir");
      return false;
   }

   struct dirent * entry;

   while ((entry = readdir (tile_dir)))
   {
      if (entry->d_type != DT_REG)
         continue;

      if (*entry->d_name == '.')
         continue;


      /* Remove file extension */

      char * ext = entry->d_name;

      while (*ext != '.' && *ext != '\0')
         ++ ext;

      *ext = 0;
      

      tile tile = malloc (sizeof (*tile));
      tile_init (ctx, tile, entry->d_name);

      HASH_ADD_KEYPTR (hh,
                       *tiles,
                       tile->name,
                       strlen (tile->name),
                       tile);
   }

   closedir (tile_dir);

   return true;
}

void tiles_unload (strat_ctx ctx, tile * tiles)
{
   tile tile, tmp;

   HASH_ITER (hh, (*tiles), tile, tmp)
   {
      HASH_DEL (*tiles, tile);

      tile_cleanup (tile);
      free (tile);
   }
}


