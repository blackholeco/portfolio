#include "Model.h"


string FileParser::findFiletype(string filename)
{
	// Make sure there are no return characters in the string
	if (filename.find_first_of('\r') != string::npos)
		filename = filename.substr(0, filename.find_first_of('\r'));

	// Return the text after the dot
	return filename.substr(filename.find_last_of(".") + 1);
}


bool FileParser::isFiletype(string filename, string filetype)
{
	string ft = findFiletype(filename);

	return (ft.compare(filetype) == 0);
}


int FileParser::splitString(const string& str, const string tokens, string** split)
{
	vector<string> found;
	int pos = 0;
	int count = 0;
	string temp = str;

	while ((pos = temp.find_first_of(tokens)) != string::npos)
	{
		found.push_back(temp.substr(0, pos));
		temp = temp.substr(pos + 1);
	}

	if (temp.length() > 0)
		found.push_back(temp);

	count = found.size();

	if (count <= 0)
		return 0;

	*split = new string[count];

	for (int i = 0; i < count; i++)
		(*split)[i].assign(found.at(i));

	found.clear();

	return count;
}


bool ModelParser::parse(string filename, _model* model)
{
	assert(filename.length() > 0);

	if (!(FileParser::isFiletype(filename, "obj") || FileParser::isFiletype(filename, "OBJ")))
	{
		cout << __FUNCTION__ << " " << filename << " : Filetype " << FileParser::findFiletype(filename) << " not supported" << endl;

		return false;
	}

	firstPass(filename, data);

	if (data.count > 0)
		secondPass(data, model);

	vertices.clear();
	tris.clear();

	return model->count > 0;
}


void ModelParser::firstPass(string filename, _model* data)
{
	int tricount = -1;
	int matcount = 0;

	string readLine;
	string* splitstr = nullptr;

	ifstream stream;

	char* end = nullptr;

	stream.open(filename);

	if (!stream.is_open())
		return;

	while (getline(stream, readLine) && stream.good())
	{
		if (readLine.length() <= 0)
			continue;

		//skip comments
		if (readLine.find('#') != string::npos)
			continue;

		// split line into multiple strings
		int strings = FileParser::splitString(readLine, " ", &splitstr);

		if (strings <= 0 || splitstr == nullptr)
			continue;

		if (strcmp(splitstr[0].c_str(), "v") == 0)
		{
			// read as vertex
			Vector3f newvert;

			newvert.x = strtof(splitstr[1].c_str(), &end);
			newvert.y = strtof(splitstr[2].c_str(), &end);
			newvert.z = strtof(splitstr[3].c_str(), &end);

			vertices.push_back(newvert);
		}
		else if (strcmp(splitstr[0].c_str(), "f") == 0)
		{
			// read as face
			TRIANGLE newtri;

			for (int i = 0; i < 3; i++)
			{
				string* tmp;
				int c = FileParser::splitString(splitstr[i + 1], "/", &tmp);

				// Decrement the index by 1 as our arrays start at index 0
				newtri.point[i] = atoi(tmp[0].c_str()) - 1;

				delete[] tmp;
				tmp = nullptr;
			}

			tris.push_back(newtri);
		}

		delete[] splitstr;
		splitstr = nullptr;
	}

	stream.close();

	// now convert this into data readable by Graphics Lib (in the second pass)
}


void ModelParser::secondPass(_model* model)
{
	unsigned int index = 0;

	model->vCount = vertices.size();
	model->iCount = tris.size * 3;
	model->vertices = new float[model->vCount * 3];
	model->indices = new unsigned int[model->iCount];

	// Convert the Vertex data into a more readable format
	for(Vector3f* vec : vertices)
	{
		model->vertices[index] = vec->x;
		model->vertices[index + 1] = vec->y;
		model->vertices[index + 2] = vec->z;

		index += 3;
	}

	index = 0;
	
	// Create the index list
	for(TRIANGLE* tri : tris)
	{
		memcpy_s(&model->indices[index], 3 * sizeof(float), tri->point, 3 * sizeof(float));

		tri = nullptr;
		
		index += 3;
	}
}


Model::Model()
{}


bool Model::Load(string filename)
{
	bool success = false;
	_model model;

	memset(&model, 0, sizeof(_model));

	success = ModelParser::parse(filename, &model);

	if(!success)
		return false;

	Parse(model)

	delete[] model->vertices;
	delete[] model->indices;

	memset(&model, 0, sizeof(_model));
	return success;
}


void Model::Parse(_model& model)
{
	//TODO Import the model data into graphics library
}

