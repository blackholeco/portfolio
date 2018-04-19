/*
 *	Model.h by Chris Allen

 *	This file is provided "as-is", for the sole purpose of a demonstration of my
	work.  It is not intended to be copied or used in an external or third-party
	project, and no support will be given for that use.

 *	You may not use or copy this file, in whole or in part, to use for your own
	projects.  All rights reserved over this file.
 */

#pragma once

#include "Material.h"


/*
 *	Struct to hold data for a single vertex (e.g. position, normal)
 */
typedef struct _modelvertex
{
	array<float, 3> xyz;
	array<float, 3> normal;
} MODELVERTEX;

/*
 *	Struct to hold a UV co-ordinate
 */
typedef struct _modeluvcoord
{
	array<float, 2> uv;
} MODELUVCOORD;

/*
 *	Struct to hold index data on a single face of a Mesh
 */
typedef struct _modeltriangle
{
	array<long, 3> point;
	array<long, 3> uvpoint;
	array<float, 3> normal;
} MODELTRIANGLE;

/*
 *	Struct to hold data on a single model
 */
typedef struct _modelobject
{
	int materialref;
	int vertexcount;
	int trianglecount;
	int uvcount;
} MODELOBJECT;


/*
 *	Class to handle a Mesh.

 *	A Mesh is a single object within a 3D model file.
 */
class Mesh
{
public:
	Mesh();
	~Mesh();

	void operator=(const Mesh& orig);

	/**
	 * Set this Model's Material

	 *	@param material : The Material to apply to this Mesh
	 */
	void ApplyMaterial(Material material);

	/**
	 *	Get the reference for this Mesh's Material

	 *	@return this Mesh's Material reference
	 */
	int GetMaterialReference() const;

	/**
	 *	Delete any dynamic data stored by this Mesh.  This should only be called when the Mesh
		is no longer requred.  Calling Release will leave the Mesh in an unusable state
	 */
	void Release();

	/**
	 *	Read this Mesh from an open package.

	 *	@param stream: A file stream which must already be open to the package file to read

	 *	@return true if the package loads successfully

	 *	@throws Runtime Error : No objects or materials found in the data
	 */
	bool Unpack(ifstream& stream);

protected:
	friend class GLInterface;

	array<unsigned int, 2> buffers;

	unsigned int vao;

	int materialref;

	shared_ptr<Material> mMaterial;		//points to an existing Material or uses a specific one

	int trianglecount;
	int vertexcount;
	int uvcount;

	MODELVERTEX* vertex;
	MODELTRIANGLE* triangle;
	MODELUVCOORD* uv;
};

/*
 *	Model Class

 *	Handles a 3D model loaded from file.  Can contain one or many Meshes,
	as well as textures
 */
class Model
{
public:
	Model();
	Model(const Model& orig);

	void operator =(const Model&);

	/**
	 *	Completely free up any data stored by this Model.  The Model will be
		unusable after this function is called, so only call Release() if this
		Model is no longer needed
	 */
	void Release();

	/**
	 *	Read this Model's data from a package file.  The Model should be the next thing that the input filestream will read

	 *	@param stream : A filestream which is already open to the package file containing this Model's data
	 */
	bool Unpack(ifstream& stream);

	/**
	 *	Static version of the Unpack method

	 *	@param stream : A filestream which is already open to the package file containing this Model's data
	 *	@param model : The Model to unpack the data to
	 */
	static bool Unpack(ifstream& stream, shared_ptr<Model>& model);

protected:
	friend class GLInterface;

	Mesh* model;

	Material* material;

	int modelcount;
	int materialcount;
};

