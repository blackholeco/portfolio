/*
 *	Model.cpp by Chris Allen

 *	This file is provided "as-is", for the sole purpose of a demonstration of my
	work.  It is not intended to be copied or used in an external or third-party
	project, and no support will be given for that use.

 *	You may not use or copy this file, in whole or in part, to use for your own
	projects.  All rights reserved over this file.
 */

#pragma once

#include <fstream>
#include <string>
#include <memory>
#include <cassert>
#include <vector>
#include <iostream>

using std::string;
using std::ifstream;
using std::shared_ptr;
using std::ios;
using std::cout;
using std::endl;
using std::vector;


/**
 *	MODEL
 
 *	Loads a Model file (currently .obj only) into memory
 
 *	Does support multiple objects in a single file, but compiles them all into a single object.
 
 *	Does not currently support Textures or UVS, but could be configured to do so.
 
 *	Faces in the OBJ file must be triangular (only have three points).
 */

/**
 *	Structure defining a 3D point in space
 */
struct Vector3f
{
	float x;
	float y;
	float z;
};

/**
 *	Data structure defining a 3D Model
 */
struct _model
{
	float* vertices;		// Model vertices
	unsigned int* indices;	// Model indices
	
	unsigned int vCount;	// Vertex Count
	unsigned int iCount;	// Number of indices
};

/**
 *	Structure describing a single face or triangle
 */
typedef struct _triangle
{
	long point[3];	// Stores the vertex index for the three points of this face
} TRIANGLE;


/* Handy functions for all parsers */
class FileParser
{
public:

/**
 *	Find the file extension in the provided string
 
 *	@param filename : The name and path of the file
 
 *	@return : The filetype/extension of the file (without the dot (.))
 */
	static string findFiletype(string filename);
	
/**
 *	Check if a file is of a given type/extension
 
 *	@param filename : The filename to check
 *	@param filetype : The type/extension to check by (without the dot)
 
 *	@return true if the extensions match, false otherwise
 */
	static bool isFiletype(string filename, string filetype);
	
/**
 *	Split a single string into a number of strings, based on a character or characters
 
 *	@param str: The string to split (will be unaltered)
 *	@param tokens: The character or characters to use as separators
 *	@param split: Pointer to an array of strings, which will be populated by this function.  This array should not be allocated before calling this function.  You should call delete[] on this array when it is no longer required.
 
 *	@return The number of strings created by this function.
 */
	static int splitString(const string& str, const string tokens, string** split);

private:
	FileParser() {};
};


/**
 *	Parser for models stored in the .obj file format
 */
class ModelParser
{
public:

/**
 *	Parse a new 3D Model (currently supports .obj files only)
 
 *	@param filename: The path and name of the file to load
 *	@param model: Pointer to a _model struct to put the data into
 
 *	@return true if the file loads successfully, false if not
 */
	static bool parse(string filename, _model* model);

private:

	ModelParser() {};

/**
 *	Collect the model's raw data.  Simply collects all vertex and index data, without further processing.
 
 *	@param filename : The path and name of the file to read
 *	@param data : Pointer to a _model struct to store the data from file
 */
	static void firstPass(string filename, _model* data);
	
/**
 *	Convert the parsed data into a more graphics-friendly format,
	and populate the _model struct data.
	
 *	Currently ignores textures and normals, but in future this can be developed to support these.
 
 *	@param data : A pointer to the _model struct to populate with data
 */
	static void secondPass(_model* data);
	
	vector<Vector3f> vertices;
	vector<TRIANGLE> tris;
	
	unsigned int count;
};


/**
 *	Class to handle the loading and parsing of a 3D model
 
 *	Each Model can be made up of one or more Meshes
 
 *	Currently, this only recognises the .obj file format, and ignores textures, uvs and normals
 */
class Model
{
public:
	Model();
	
	/**
	 *	Load a model from file
	 
	 *	@param filename : The path and name of the file to load
	 
	 *	@return True if model loads successfully, false otherwise
	 */
	bool Load(string filename);

protected:

	/**
	 *	Parse this model into the graphics library
	 
	 *	This should follow a successful call to Load(), and requires a graphics context
	  to be open
	  
	 *	@param model : A _model struct containing the data for this Model
	 */
	void Parse(_model& model);
};
