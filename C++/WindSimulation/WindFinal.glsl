#version 430

/*
 *		WindFinal.glsl
 *
 *	Author: Chris Allen
 *	Copyright Chris Allen 2018, all rights reserved
 *
 *	This file forms part of the Wind Simulation Project.
 *	It is intended to form part of my portfolio, for demonstration purposes ONLY.
 *
 *	You may NOT edit/alter this file in any way.
 *	You may NOT make any copies of this file for purposes other than its original intention (i.e. for demonstration purposes).
 *	You may NOT use or claim this file as your own work, either partially or wholly
 *
 *	This file is provided as-is.  No support will be provided for editing or using this file beyond its original intention.
 */

#ifdef COMPILE_VS

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 uv;

uniform mat4 model;

layout( std140 ) uniform Camera
{
	mat4 view;
	mat4 projection;
};

uniform sampler2D image;

out vec2 texCoord;
out float delta;

void main()
{
	mat4 mvp = projection * view * model;

	vec4 colour = texture(image, uv);

	texCoord = uv;

	delta = vertex.y / 2.0;

	if(colour.a > 0.0)
	{
		float power = colour.a * 2.0;

		float deltaS = vertex.y * vertex.y * 0.5;

		vec3 displacement = vec3(	power * (colour.r - 0.5),
									power * (colour.g - 0.5),
									power * (colour.b - 0.5));

		vec3 position = vec3(	vertex.x + (displacement.x * deltaS),
								vertex.y,
								vertex.z - (displacement.z * deltaS)); 

		gl_Position = mvp * vec4(position, 1.0);
	}
	else
		gl_Position = mvp * vec4(vertex, 1.0);	
}

#endif

#ifdef COMPILE_FS


uniform vec4 colour = vec4(0, 0.75, 0, 1);
uniform vec4 transMask = vec4(0, 0, 0, 0);

in vec2 texCoord;
in float delta;

out vec4 FragColour;

void main()
{
	FragColour = vec4(colour.r, colour.g * delta, colour.b, colour.a);
}

#endif