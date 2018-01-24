#version 430

//
//	WindDemo.glsl by Chris Allen

//	This file is provided "as-is", for the sole purpose of a demonstration of my
//	work.  It is not intended to be copied or used in an external or third-party
//	project, and no support will be given for that use.

//	You may not use or copy this file, in whole or in part, to use in your own, or
//	other projects.  All rights reserved over this file.
//


// VERTEX SHADER
#ifdef COMPILE_VS

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Vertex Displacement Map sampler
uniform sampler2D image;

// Height of the vertex, pass to fragment shader for colouring the grass based on height
out float delta;

void main()
{
	mat4 mvp = projection * view * model;

	// Take a sample from our vertex dispacement map
	vec4 colour = texture(image, uv);

	// Record our vertex height to pass to the fragment shader
	delta = vertex.y;

	// If sample colour is not transparent
	if(colour.a > 0.0)
	{
	// Alpha value becomes our power
		float power = colour.a;

	// Square of the height to give each blade a curve when affected by the wind
		float deltaS = delta * delta;

	// Minus 0.5 from each component to convert it to a suitable range, then apply our wind strength
		vec2 displacement = vec2(	power * (colour.r - 0.5),
									power * (colour.b - 0.5));

	// Displace the vertex by the calculated value, factored by the square of the vertex's height
		vec3 position = vec3(	vertex.x + displacement.x * deltaS,
								vertex.y,
								vertex.z - displacement.y * deltaS); 

		gl_Position = mvp * vec4(position, 1.0);
	}
	else
		gl_Position = mvp * vec4(vertex, 1.0);	
}

#endif

// FRAGMENT SHADER
#ifdef COMPILE_FS

uniform vec4 colour = vec4(0, 0.75, 0, 1);
uniform vec4 transMask = vec4(0, 0, 0, 0);

in float delta;

out vec4 FragColour;

void main()
{
	// Colour the fragment based on the delta value
	FragColour = vec4(colour.r, colour.g * delta * 0.5f, colour.b, colour.a);
}

#endif
