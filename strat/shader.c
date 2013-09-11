
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

bool shader_init (shader shader, const char * name)
{
    if ((shader->program = glCreateProgram ()) == 0)
    {
        trace ("Error creating shader program for %s", name); 
        return false;
    }

    char filename [strat_max_path];

    /* Vertex shader
     */
    sprintf (filename, "shader/%s.vsh", name);

    char * vertex_source;

    if (!sut_load_file (filename, &vertex_source, NULL))
    {
        trace ("Error loading vertex shader: %s", filename);
        return false;
    }

    if ((shader->vertex = glCreateShader (GL_VERTEX_SHADER)) == 0)
    {
        trace ("Error creating vertex shader: %s", name);
        return false;
    }

    glShaderSource (shader->vertex, 1, (const char **) &vertex_source, NULL);
    glCompileShader (shader->vertex);

    free (vertex_source);

    /* Fragment shader
     */
    sprintf (filename, "shader/%s.fsh", name);

    char * fragment_source;

    if (!sut_load_file (filename, &fragment_source, NULL))
    {
        trace ("Error loading fragment shader: %s", filename);
        return false;
    }

    if ((shader->fragment = glCreateShader (GL_FRAGMENT_SHADER)) == 0)
    {
        trace ("Error creating fragment shader: %s", name);
        return false;
    }

    glShaderSource (shader->fragment, 1, (const char **) &fragment_source, NULL);
    glCompileShader (shader->fragment);

    free (fragment_source);

    return true;
}

void shader_cleanup (shader shader)
{

}



