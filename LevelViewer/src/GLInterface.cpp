/*
 *	GLInterface.cpp by Chris Allen

 *	This file is provided "as-is", for the sole purpose of a demonstration of my
	work.  It is not intended to be copied or used in an external or third-party
	project, and no support will be given for that use.

 *	You may not use or copy this file, in whole or in part, to use for your own
	projects.  All rights reserved over this file.
 */

#include "GLInterface.h"

shared_ptr<GLInterface> GLInterface::instance(nullptr);


GLInterface::GLInterface()
{
	curModel = 0;

	meshlist.clear();
}


void GLInterface::UseDefaultTex()
{
	SetTexture(defaulttex);
}


bool GLInterface::CreateDefaultMaterial()
{
	int i, j, c;
	GLubyte checkImage[64][64][4];

	for (i = 0; i < 64; i++) {
		for (j = 0; j < 64; j++) {
			c = 255 * (((i & 0x8) == 0) ^ ((j & 0x8) == 0));
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	glBindTexture(GL_TEXTURE_2D, defaulttex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);

	defaultmaterial.Create(0, GLColour::WHITE, defaulttex);

	return true;
}


void GLInterface::LoadProgram()
{
	int vert = LoadShader("texture.vert", GL_VERTEX_SHADER);
	int frag = LoadShader("texture.frag", GL_FRAGMENT_SHADER);

	shader = glCreateProgram();

	glAttachShader(shader, vert);
	glAttachShader(shader, frag);

	glBindAttribLocation(shader, 0, "vertexposition");
	glBindAttribLocation(shader, 1, "vertextexcoord");

	glLinkProgram(shader);

	glDetachShader(shader, vert);
	glDetachShader(shader, frag);

	glDeleteShader(vert);
	glDeleteShader(frag);

	glUseProgram(shader);
}


int GLInterface::LoadShader(string file, unsigned int type)
{
	int ret = -1;

	char* shader = nullptr;

	ifstream stream;

	if (file.length() <= 0)
	{
		return -1;
	}

	stream.open(file);

	if (!stream.is_open())
	{
		return -1;
	}

	stream.seekg(0, stream.end);

	long filesize = stream.tellg();

	stream.seekg(0, stream.beg);

	if (filesize <= 0)
	{
		stream.close();

		return -1;
	}

	shader = new char[filesize + 1];
	assert(shader);

	stream.read(shader, filesize);

	shader[filesize] = '\0';

	stream.close();

	ret = glCreateShader(type);

	const char* arr[] = { shader };

	glShaderSource(ret, 1, arr, nullptr);

	glCompileShader(ret);

	delete[] shader;
	shader = nullptr;

	int result;

	glGetShaderiv(ret, GL_COMPILE_STATUS, &result);
	if (result == false)
	{
		int loglength;

		glGetShaderiv(ret, GL_INFO_LOG_LENGTH, &loglength);

		if (loglength > 0)
		{
			char* log = new char[loglength];

			int written;

			glGetShaderInfoLog(ret, loglength, &written, log);

			delete[] log;
			log = nullptr;
		}
	}

	return ret;
}


GLInterface& GLInterface::Instance()
{
	if (instance == nullptr)	// Check if an instance exists
		instance.reset(new GLInterface);	//Create a new instance if not

	assert(instance);	// Make sure the reference is OK

	return *instance;	// return the actual instance, but as a reference
}


bool GLInterface::Initialize(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(800, 600);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextVersion(4, 0);

	glGenTextures(1, &defaulttex);

	CreateDefaultMaterial();

	return true;
}


unsigned int GLInterface::createVAO()
{
	unsigned int vao;

	glGenVertexArrays(1, &vao);

	return vao;
}


bool GLInterface::createWindow(string title)
{
	glutCreateWindow(title.c_str());

	/* GLEW initialization goes here so that a GL Context is open first */
	int err = glewInit();

	if (err != GLEW_OK)
		return false;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glEnable(GL_NORMALIZE);

	glDepthFunc(GL_LESS);

	glShadeModel(GL_SMOOTH);

	LoadProgram();

	return true;
}


void GLInterface::EnableParam(GLenum param)
{
	glEnable(param);
}


unsigned int GLInterface::CreateTexture()
{
	unsigned int ret;

	glGenTextures(1, &ret);

	return ret;
}


void GLInterface::DeleteTexture(unsigned int& texture)
{
	glDeleteTextures(1, &texture);

	texture = 0;
}


void GLInterface::BuildTexture(shared_ptr<Texture>& texture)
{
	glGenTextures(1, &texture->imgId);

	glBindTexture(GL_TEXTURE_2D, texture->imgId);

	glTexImage2D(GL_TEXTURE, 0, GL_RGB, texture->imgWidth, texture->imgHeight,
		0, GL_BGR_EXT, GL_UNSIGNED_BYTE, texture->data);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, texture->imgWidth, texture->imgHeight,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, texture->data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	texture->Release();
}


void GLInterface::Release()
{
	meshlist.clear();

	curModel = 0;
}


bool GLInterface::SetTexture(unsigned int textureid)
{
	if (textureid > 0)
		glBindTexture(GL_TEXTURE_2D, textureid);
	else
		glBindTexture(GL_TEXTURE_2D, defaulttex);

	return true;
}


void GLInterface::DrawVAO(unsigned int vao, unsigned int count, shared_ptr<Texture> texture, Vector3f position)
{
	glBindVertexArray(vao);

	glBindTexture(GL_TEXTURE_2D, texture->imgId);

	Matrix4 matrix = Matrix4::translate(position);

	unsigned int modelLoc = glGetUniformLocation(shader, "model");

	glUniformMatrix4fv(modelLoc, 1, false, matrix.data());

	unsigned int sampleLoc = glGetUniformLocation(shader, "tex1");

	glUniform1i(sampleLoc, 0);

	glDrawArrays(GL_TRIANGLES, 0, count);

	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(0);
}


void GLInterface::DrawModel(shared_ptr<Model>& model, shared_ptr<Texture> texture)
{
	Mesh* curmodel;
	shared_ptr<Texture> useTex = nullptr;

	for (int i = 0; i < model->modelcount; i++)
	{
		curmodel = &model->model[i];

		if (texture != nullptr)
			useTex = texture;
		else
		{
			if (curmodel->mMaterial != nullptr && curmodel->mMaterial->mTexture != nullptr)
				useTex = curmodel->mMaterial->mTexture;
			else
				useTex = defaultmaterial.mTexture;
		}

		glBindVertexArray(curmodel->vao);

		unsigned int modelLoc = glGetUniformLocation(shader, "model");

		glUniformMatrix4fv(modelLoc, 1, false, modelM.data());

		unsigned int sampleLoc = glGetUniformLocation(shader, "tex1");

		glUniform1i(sampleLoc, 0);

		glBindTexture(GL_TEXTURE_2D, useTex->imgId);

		glDrawArrays(GL_TRIANGLES, 0, curmodel->trianglecount * 3);
	}
}


void GLInterface::beginRender()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shader);

	unsigned int viewLoc = glGetUniformLocation(shader, "view");
	unsigned int projLoc = glGetUniformLocation(shader, "projection");

	glUniformMatrix4fv(viewLoc, 1, false, viewM.data());
	glUniformMatrix4fv(projLoc, 1, false, projectionM.data());
}


void GLInterface::endRender()
{
	glBindTexture(GL_TEXTURE_2D, defaulttex);
	glBindVertexArray(0);

	glutSwapBuffers();
}


void GLInterface::setModelMatrix(Matrix4 mMatrix)
{
	modelM = mMatrix;
}


void GLInterface::setViewMatrix(Matrix4 vMatrix)
{
	viewM = vMatrix;
}


void GLInterface::setProjection(Matrix4 pMatrix)
{
	projectionM = pMatrix;
}


void GLInterface::DisableParam(GLenum param)
{
	glDisable(param);
}


void GLInterface::TimerCall(int millis, void(*func)(int val), int value)
{
	assert(func);

	glutTimerFunc(millis, func, value);
}


void GLInterface::CalculateGLColour(int r, int g, int b, GLfloat* ret)
{
	assert(ret);

	ret[0] = (float)r / 255.0f;
	ret[1] = (float)g / 255.0f;
	ret[2] = (float)b / 255.0f;
}


void GLInterface::BuildModel(shared_ptr<Model>& model)
{
	Mesh* mesh = nullptr;

	float* vertices = nullptr;
	float* uvs = nullptr;

	for (int i = 0; i < model->materialcount; i++)
		BuildTexture(model->material[i].mTexture);

	for (int i = 0; i < model->modelcount; i++)
	{
		mesh = &model->model[i];

		if (mesh->materialref > -1)
			mesh->mMaterial.reset(new Material(model->material[mesh->materialref]));

		mesh->buffers = CreateBuffers<2>();

		glGenVertexArrays(1, &mesh->vao);

		glBindVertexArray(mesh->vao);

		glBindBuffer(GL_ARRAY_BUFFER, mesh->buffers[0]);

		vertices = new float[mesh->trianglecount * 9];

		if (mesh->uvcount > 0)
			uvs = new float[mesh->trianglecount * 6];

		for (int j = 0; j < mesh->trianglecount; j++)
		{
			MODELTRIANGLE* triangle = &mesh->triangle[j];

			for (int k = 0; k < 3; k++)
			{
				vertices[(j * 9) + (k * 3)] = mesh->vertex[triangle->point[k]].xyz[0];
				vertices[(j * 9) + (k * 3) + 1] = mesh->vertex[triangle->point[k]].xyz[1];
				vertices[(j * 9) + (k * 3) + 2] = mesh->vertex[triangle->point[k]].xyz[2];

				if (uvs != nullptr)
				{
					uvs[(j * 6) + (k * 2)] = mesh->uv[triangle->uvpoint[0]].uv[0];
					uvs[(j * 6) + (k * 2) + 1] = mesh->uv[triangle->uvpoint[0]].uv[1];
				}
			}
		}

		glBufferData(GL_ARRAY_BUFFER, mesh->trianglecount * 9 * sizeof(float), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);

		glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, nullptr);

		delete[] vertices;

		vertices = nullptr;

		if (uvs != nullptr)
		{
			glBindBuffer(GL_ARRAY_BUFFER, mesh->buffers[1]);
			glBufferData(GL_ARRAY_BUFFER, mesh->trianglecount * 6 * sizeof(float), uvs, GL_STATIC_DRAW);

			glEnableVertexAttribArray(1);

			glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, nullptr);

			delete[] uvs;

			uvs = nullptr;
		}
	}
}

