# version 330
out vec4 color;

in vec2 uv0;

uniform sampler2D mytexture;

void main()
{
 //color = vec4(1.0, 0.0, 0.0, 1.0);
 color = texture2D(mytexture, uv0);
}
