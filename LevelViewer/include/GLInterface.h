/*
 *	GLInterface.h by Chris Allen

 *	This file is provided "as-is", for the sole purpose of a demonstration of my
	work.  It is not intended to be copied or used in an external or third-party
	project, and no support will be given for that use.

 *	You may not use or copy this file, in whole or in part, to use for your own
	projects.  All rights reserved over this file.
 */

#pragma once

#include <glew\include\GL\glew.h>
#include <freeglut\include\GL\freeglut.h>

#include <vector>
#include <map>
#include <functional>

#include "Model.h"
#include "Matrix4.h"

using std::map;
using std::vector;

#define MAX_TEXTURE_NAMES 6
#define MAX_MODELS 10

/**
 *	Class to act as a wrapper around OpenGL, simplifying access and functionality
 */
class GLInterface
{
public:

	/**
	 *	Returns the instance of GLInterface in this program.  If an instance does not already exist,
		a new one is created

	 *	@return A reference to the instance of GLInterface in this program
	 */
	static GLInterface& Instance();

	/**
	 *	Use the default texture (checkerboard pattern) for rendering
	 */
	void UseDefaultTex();

	/**
	 *	Prepares application settings and create default resources

	 *	@return true if initialised successful
	 */
	bool Initialize(int argc, char** argv);

	/**
	 *	Prepare to render the scene.
	 */
	void beginRender();

	/**
	 *	Swap GL buffers and end rendering
	 */
	void endRender();

	/**
	 *	Create and open a new App Window, and create a new OpenGL Core context

	 *	@param title : The Window's title

	 *	@return true if window is created successfully
	 */
	bool createWindow(string title);

	/**
	 *	Enable an OpenGL parameter

	 *	@param param : Parameter to enable
	 */
	void EnableParam(GLenum param);

	/**
	 *	Disable an OpenGL parameter

	 *	@param param : Parameter to disable
	 */
	void DisableParam(GLenum param);

	/**
	 *	Draw a 3D Model in the scene

	 *	@param model : A reference to the Model to draw

	 *	@param texture : A texture to apply to the Model when rendering,
		which overrides the	Model's existing Texture
	 */
	void DrawModel(shared_ptr<Model>& model, shared_ptr<Texture> texture = nullptr);

	/**
	 *	Set the Model Matrix to use for rendering

	 *	@param mMatrix : The Matrix to use as the model Matrix
	 */
	void setModelMatrix(Matrix4 mMatrix);

	/**
	 *	Set the View Matrix to use for rendering

	 *	@param vMatrix : The Matrix to use as the View Matrix
	 */
	void setViewMatrix(Matrix4 vMatrix);

	/**
	 *	Set the Projection Matrix to use for rendering

	 *	@param pMatrix : The Matrix to use as the Projection Matrix
	 */
	void setProjection(Matrix4 pMatrix);

	/**
	 *	Create a list of new Texture references

	 *	@param count : Number of new Texture references to create

	 *	@return an array of new Texture references
	 */
	template<int count>
	array<unsigned int, count> CreateTextureList()
	{
		assert(count > 0);

		array<unsigned int, count> ret;

		glGenTextures(count, ret.data());

		return ret;
	}

	/**
	 *	Import Texture data into OpenGL

	 *	@param texture : A pointer to the Texture to import
	 */
	void BuildTexture(shared_ptr<Texture>& texture);

	/**
	 *	Build a Model's data into OpenGL

	 *	@param model : A pointer to the Model to import
	 */
	void BuildModel(shared_ptr<Model>& model);

	/**
	 *	Create a single new Texture reference

	 *	@return A new Texture reference
	 */
	unsigned int CreateTexture();

	/**
	 *	Delete a list of Textures.  These Textures will be unusable after calling this function

	 *	@param count : Number of Textures to delete
	 *	@param textures : List of Textures to delete (by reference)
	 */
	template<unsigned int count>
	void DeleteTextureList(array<unsigned int, count>& textures)
	{
		glDeleteTextures(count, textures.size());

		textures.fill(0);
	}

	/**
	 *	Create a number of new data buffers

	 *	@param count : Number of buffers to create

	 *	@return An array containing the references of the new data buffers
	 */
	template<unsigned int count>
	array<unsigned int, count> CreateBuffers()
	{
		unsigned int buffers[count];

		glGenBuffers(count, buffers);

		array<unsigned int, count> ret;

		std::copy(buffers, buffers + count, ret.begin());

		return ret;
	}

	/**
	 *	Delete a Texture from graphics memory.  This Texture will then be unusable.

	 *	@param texture : The reference of the Texture to delete.
	 */
	void DeleteTexture(unsigned int& texture);

	/**
	 *	Deletes all data associated with this GLInterface. The interface will be unusable
		after calling Release(), so only call this when it is no longer needed.
	 */
	void Release();

	/**
	 *	Bind a Texture for rendering

	 *	@param textureid : Reference/ID of the texture to bind

	 *	@return true if successful
	 */
	bool SetTexture(unsigned int);

	/**
	 *	Set a function to call at some point in the future

	 *	@param millis : Time in milliseconds from now to call the function

	 *	@param func : A pointer to the function to call
	 */
	void TimerCall(int millis, void(*func)(int val), int);

	/**
	 *	Take in a HTML colour and convert it to OpenGL (0-1 for each component)

	 *	@param r : Red component of the colour to convert
	 *	@param g : Green component of the colour to convert
	 *	@param b : Blue component of the colour to convert

	 *	@param ret : Length-3 array to store the converted colour to
	 */
	void CalculateGLColour(int r, int g, int b, GLfloat* ret);

	/**
	 *	Render a Vertex Array Object to scene

	 *	@param vao : The reference of the VAO to render

	 *	@param count : Number of vertices to render

	 *	@param texture : The texture to apply to the VAO

	 *	@param position : Position in the scene to render the VAO
	 */
	void DrawVAO(unsigned int vao, unsigned int count, shared_ptr<Texture> texture, Vector3f position);

	/**
	 *	Create a new Vertex Array Object

	 *	@return The reference of the new VAO
	 */
	unsigned int createVAO();

protected:

	/**
	 *	Create the default Texture/Material.  This will be a black-and-white 
		checkerboard pattern
	 */
	bool CreateDefaultMaterial();

	/**
	 *	Load the shader program used for rendering
	 */
	void LoadProgram();

	/**
	 *	Load and import a Shader file
	 
	 *	@param filename : path and name of the shader file to load

	 *	@param type : int representation of the shader type
		(either GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
	 */
	int LoadShader(string filename, unsigned int type);

	vector<Model*> meshlist;

	int curModel;

	GLuint defaulttex;
	Material defaultmaterial;

	unsigned int shader;

	Matrix4 modelM;
	Matrix4 viewM;
	Matrix4 projectionM;

private:
	// Keeping constructor/assignment private so nothing else can call them
	GLInterface();
	GLInterface(const GLInterface&);
	GLInterface& operator=(const GLInterface);

	static shared_ptr<GLInterface> instance;
};

#define glInterface GLInterface::Instance()

// Predefined colours
namespace GLColour
{
	static const array<float, 3> BLACK = { 0, 0, 0 };
	static const array<float, 3> WHITE = { 1, 1, 1 };
	static const array<float, 3> RED = { 1, 0, 0 };
	static const array<float, 3> GREEN = { 0, 1, 0 };
	static const array<float, 3> BLUE = { 0, 0, 1 };
};


