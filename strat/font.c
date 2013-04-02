
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

bool font_init (font font, const char * name, int size)
{
   if (! (font->atlas = texture_atlas_new (512, 512, 1)))
   {
      fprintf (stderr, "Failed to create font texture atlas\n");
      return false;
   }

   char filename [strat_max_path];
   snprintf (filename, sizeof (filename), "game/font/%s.ttf", name);

   if (! (font->font = texture_font_new (font->atlas, filename, size)))
   {
      fprintf (stderr, "texture_font_new failed for %s\n", name);
      return false;
   }

   texture_font_load_glyphs (font->font,
         L" !\"#$%&'()*+,-./0123456789:;<=>?"
         L"@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
         L"`abcdefghijklmnopqrstuvwxyz{|}~");

   return true;
}

void font_cleanup (font font)
{
   texture_font_delete (font->font);
   texture_atlas_delete (font->atlas);
}

void text_draw (font font,
                int x,
                int y,
                int box_width,
                int box_height,
                const char * s,
                int flags)
{
   size_t length = strlen (s);

   glBindTexture (GL_TEXTURE_2D, font->atlas->id);
   glColor4f (1, 1, 1, 1);

   size_t width = 0;

   for (size_t i = 0; i < length; ++ i)
   {
      texture_glyph_t * glyph = texture_font_get_glyph (font->font, s [i]);

      if (!glyph)
      {
         glyph = texture_font_get_glyph (font->font, '?');
         assert (glyph);
      }

      if (i > 0)
         width += texture_glyph_get_kerning (glyph, s [i - 1]);

      width += glyph->advance_x;
   }

   y += font->font->height;

   if (flags & text_draw_center_v)
      y += box_height / 2 - font->font->height / 2;

   if (flags & text_draw_center_h)
      x += box_width / 2 - width / 2;

   /* TODO: Draw the entire string at once */

   for (size_t i = 0; i < length; ++ i)
   {
      texture_glyph_t * glyph = texture_font_get_glyph (font->font, s [i]);

      if (!glyph)
         glyph = texture_font_get_glyph (font->font, '?');

      if (i > 0)
         x += texture_glyph_get_kerning (glyph, s [i - 1]);

      int x0 = x + glyph->offset_x;
      int y0 = y - glyph->offset_y;
      int x1 = x0 + glyph->width;
      int y1 = y0 + glyph->height;

      GLfloat vertices [] =
      {
         x0,   y0,
         x1,   y0,
         x0,   y1,
         x1,   y1
      };

      GLfloat texCoords [] =
      {
         glyph->s0,     glyph->t0,
         glyph->s1,     glyph->t0,
         glyph->s0,     glyph->t1,
         glyph->s1,     glyph->t1
      };

      glVertexPointer (2, GL_FLOAT, 0, vertices);
      glTexCoordPointer (2, GL_FLOAT, 0, texCoords);
      glDrawArrays (GL_TRIANGLE_STRIP, 0, 4);

      x += glyph->advance_x;
   }
}



