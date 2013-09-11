
attribute vec4 position;
uniform mat4 projectionMatrix;

uniform sampler2D texture;

void main()
{
    gl_Position = position * projectionMatrix;
}

