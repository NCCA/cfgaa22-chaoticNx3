#version 420 core

// this is a pointer to the current 2D texture object
layout (binding=0) uniform sampler2D tex;   //binding sets which texture it is
// the vertex UV
in vec2 vertUV;
layout (location=0)out vec4 outColour;
void main ()
{
 // sets the fragment colour to the current texture
 outColour = texture(tex,vertUV);
}
