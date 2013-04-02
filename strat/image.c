
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

static GLuint load_texture (const char * filename,
                            int * out_image_width,
                            int * out_image_height,
                            int * out_texture_width,
                            int * out_texture_height);

bool image_init (image image, const char * filename)
{
   memset (image, 0, sizeof (*image));

   if ((image->texture = load_texture
            (filename,
             &image->width,
             &image->height,
             &image->texture_width,
             &image->texture_height)) == -1)
   {
      return false;
   }
        
   image->triangleTexCoords [2] = image->triangleTexCoords [6]
      = ((GLfloat) image->width) / image->texture_width;

   image->triangleTexCoords [5] = image->triangleTexCoords [7]
      = ((GLfloat) image->height) / image->texture_height;

   return true;
}

void image_cleanup (image image)
{
   glDeleteTextures (1, &image->texture);
}

GLuint load_texture (const char * filename,
                     int * out_image_width,
                     int * out_image_height,
                     int * out_texture_width,
                     int * out_texture_height)
{
   FILE * file = fopen (filename, "rb");

   if (!file)
      return -1;

   png_byte sig [8];

   if (fread (sig, 1, sizeof (sig), file) != sizeof (sig))
      return -1;

   if (png_sig_cmp (sig, 0, 8))
      return -1;

   png_structp png;

   if (! (png = png_create_read_struct (PNG_LIBPNG_VER_STRING, 0, 0, 0)))
      return -1;

   png_infop info;

   if (! (info = png_create_info_struct (png)))
      return -1;

   if (setjmp (png_jmpbuf (png)))
   {
      fprintf (stderr, "Error loading image (libpng): %s\n", filename);
      return -1;
   }

   png_init_io (png, file);
   png_set_sig_bytes (png, sizeof (sig));

   png_read_info (png, info);

   int image_width = png_get_image_width (png, info),
       image_height = png_get_image_height (png, info),
       color_type = png_get_color_type (png, info),
       bit_depth = png_get_bit_depth (png, info);

   if (color_type == PNG_COLOR_TYPE_PALETTE)
      png_set_palette_to_rgb (png);

   if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
      png_set_gray_1_2_4_to_8 (png);

   if (bit_depth < 8)
      png_set_packing (png);

   if (png_get_valid (png, info, PNG_INFO_tRNS))
      png_set_tRNS_to_alpha (png);

   png_read_update_info (png, info);

   bit_depth = png_get_bit_depth (png, info);
   color_type = png_get_color_type (png, info);

   int bytes_per_pixel = png_get_rowbytes (png, info) / image_width;

   int texture_width = 1,
       texture_height = 1;

   while (texture_width < image_width)
      texture_width *= 2;

   while (texture_height < image_height)
      texture_height *= 2;

   int bytes_per_row = texture_width * bytes_per_pixel;

   bytes_per_row += 3 - (bytes_per_row - 1) % 4;

   png_bytep data = (png_bytep) calloc (bytes_per_row * texture_height, 1);

   if (!data)
      return -1;

   png_bytepp row_pointers = (png_bytepp) alloca (sizeof (png_bytep) * image_height);

   for (int y = 0; y < image_height; ++ y)
      row_pointers [y] = data + (y * bytes_per_row);

   png_read_image (png, row_pointers);

   fclose (file);

   png_destroy_read_struct (&png, &info, png_infopp_NULL);

   GLuint texture;

   glGenTextures (1, &texture);
   glBindTexture (GL_TEXTURE_2D, texture);

   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

   glTexImage2D (GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 texture_width,
                 texture_height,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 data);
        
   free (data);

   *out_image_width = image_width;
   *out_image_height = image_height;

   *out_texture_width = texture_width;
   *out_texture_height = texture_height;

   return texture;
}

void image_draw (image image, int x, int y)
{
   x -= image->offset_x;
   y -= image->offset_y;

   GLfloat vertices [] =
   {
      x,                  y,
      x + image->width,   y,
      x,                  y + image->height,
      x + image->width,   y + image->height
   };

   glBindTexture (GL_TEXTURE_2D, image->texture);
   glVertexPointer (2, GL_FLOAT, 0, vertices);
   glTexCoordPointer (2, GL_FLOAT, 0, image->triangleTexCoords);

   glDrawArrays (GL_TRIANGLE_STRIP, 0, 4);
}

