/*
 *	Model.cpp by Chris Allen

 *	This file is provided "as-is", for the sole purpose of a demonstration of my
	work.  It is not intended to be copied or used in an external or third-party
	project, and no support will be given for that use.

 *	You may not use or copy this file, in whole or in part, to use for your own
	projects.  All rights reserved over this file.
 */

#include "Model.h"


Mesh::Mesh()
{
	vertex = nullptr;
	triangle = nullptr;
	uv = nullptr;

	materialref = -1;
	trianglecount = vertexcount = uvcount = 0;

	mMaterial.reset();

	Release();
}


Mesh::~Mesh()
{
	Release();
}


void Mesh::operator=(const Mesh& orig)
{
	materialref = orig.materialref;

	vao = orig.vao;
	buffers = orig.buffers;

	if (orig.mMaterial != nullptr)
		mMaterial = orig.mMaterial;

	trianglecount = orig.trianglecount;
	vertexcount = orig.vertexcount;
	uvcount = orig.uvcount;

	triangle = new MODELTRIANGLE[trianglecount];

	assert(triangle);
	assert(orig.triangle);

	memcpy_s(triangle, sizeof(MODELTRIANGLE) * trianglecount, orig.triangle, sizeof(MODELTRIANGLE) * trianglecount);

	vertex = new MODELVERTEX[vertexcount];

	assert(vertex);
	assert(orig.vertex);

	memcpy_s(vertex, sizeof(MODELVERTEX) * vertexcount, orig.vertex, sizeof(MODELVERTEX) * vertexcount);

	uv = new MODELUVCOORD[uvcount];
	assert(uv);
	assert(orig.uv);

	memcpy_s(uv, sizeof(MODELUVCOORD) * uvcount, orig.uv, sizeof(MODELUVCOORD) * uvcount);
}


void Mesh::ApplyMaterial(Material mat)
{
	mMaterial.reset(new Material(mat));
}


int Mesh::GetMaterialReference() const
{
	return materialref;
}


void Mesh::Release()
{
	if (mMaterial.get() != nullptr)
		mMaterial.reset();

	if (triangle)
	{
		assert(triangle);
		delete[] triangle;
		triangle = nullptr;
	}

	if (uv)
	{
		assert(uv);
		delete[] uv;
		uv = nullptr;
	}

	if (vertex)
	{
		assert(vertex);
		delete[] vertex;
		vertex = nullptr;
	}
};


bool Mesh::Unpack(ifstream& stream)
{
	int i = 0;
	MODELOBJECT object;

	assert(stream.is_open());

	memset(&object, 0, sizeof(MODELOBJECT));

	stream.read((char*)&object, sizeof(MODELOBJECT));

	vertexcount = object.vertexcount;
	uvcount = object.uvcount;
	trianglecount = object.trianglecount;
	materialref = object.materialref;

	vertex = new MODELVERTEX[vertexcount];
	assert(vertex);

	triangle = new MODELTRIANGLE[trianglecount];
	assert(triangle);

	uv = new MODELUVCOORD[uvcount];
	assert(uv);

	for (i = 0; i < vertexcount; i++)
		stream.read((char*)&vertex[i], sizeof(MODELVERTEX));

	for (i = 0; i < trianglecount; i++)
		stream.read((char*)&triangle[i], sizeof(MODELTRIANGLE));

	for (i = 0; i < uvcount; i++)
		stream.read((char*)&uv[i], sizeof(MODELUVCOORD));

	return true;
}


Model::Model()
{
	model = nullptr;
	material = nullptr;

	modelcount = materialcount = 0;

}


Model::Model(const Model& orig)
{
	modelcount = orig.modelcount;
	materialcount = orig.materialcount;

	model = new Mesh[modelcount];
	assert(model);

	for (int i = 0; i < modelcount; i++)
		model[i] = orig.model[i];

	if (materialcount > 0)
	{
		material = new Material[materialcount];

		for (int i = 0; i < materialcount; i++)
			material[i] = orig.material[i];
	}
}


void Model::operator =(const Model& orig)
{
	model = orig.model;
	material = orig.material;
	modelcount = orig.modelcount;
	materialcount = orig.materialcount;
}


void Model::Release()
{
	if (model)
	{
		assert(model);
		delete[] model;
		model = nullptr;
	}

	if (material)
	{
		assert(material);
		delete[] material;
		material = nullptr;
	}

	modelcount = materialcount = 0;
}


bool Model::Unpack(ifstream& stream, shared_ptr<Model>& model)
{
	model.reset(new Model());

	assert(model != nullptr);

	return model->Unpack(stream);
}


bool Model::Unpack(ifstream& stream)
{
	assert(stream.is_open());

	stream.read(reinterpret_cast<char*>(&modelcount), sizeof(int));
	stream.read(reinterpret_cast<char*>(&materialcount), sizeof(int));

	//unsigned char fourByte[4];

	// has to be this way because stream is binary data
	//stream.read((char*) fourByte, 4);
	//stream.read((char*)&materialcount, sizeof(int));

	assert(modelcount > 0);

	model = new Mesh[modelcount];

	if (materialcount > 0)		// might be 0
	{
		material = new Material[materialcount];

		for (int i = 0; i < materialcount; i++)
			material[i].Unpack(stream);
	}
	else
		material = nullptr;

	for (int i = 0; i < modelcount; i++)
		model[i].Unpack(stream);

	return true;
}

