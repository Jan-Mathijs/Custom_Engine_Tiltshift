#version 460 core
in vec2 v_TexCoord;
uniform sampler2D u_TextureSlot;

out vec4 v_Colour;

void main()
{
	vec4 texColour = texture(u_TextureSlot, v_TexCoord);
	v_Colour = texColour;
}