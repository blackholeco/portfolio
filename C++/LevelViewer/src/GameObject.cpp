/*
 *	GameObject.cpp by Chris Allen

 *	This file is provided "as-is", for the sole purpose of a demonstration of my
	work.  It is not intended to be copied or used in an external or third-party
	project, and no support will be given for that use.

 *	You may not use or copy this file, in whole or in part, to use for your own
	projects.  All rights reserved over this file.
 */

#include "GameObject.h"

using std::atof;

/* GameObject Class Functions */

GameObject::~GameObject()
{

}


Floor::Floor()
{
	vao = 0;
	mWidth = 0;
	mHeight = 0;
}


void Floor::Initialize(unsigned int width, unsigned int height, shared_ptr<Texture> texture)
{
	mWidth = width;
	mHeight = height;

	mTexture = texture;
}


void Floor::Draw()
{
	glInterface.DrawVAO(vao, 6, mTexture, Vector3f(-0.5f, -0.5f, -0.5f));
}


void Floor::Import()
{
	vao = glInterface.createVAO();

	float vertices[] = {
		0, 0, 0,
		0, 0, mHeight,
		mWidth, 0, mHeight,

		0, 0, 0,
		mWidth, 0, mHeight,
		mWidth, 0, 0
	};


	float uvs[] = {
		0, 1,
		0, 0,
		1, 0,

		0, 1,
		1, 0,
		1, 1
	};

	buffer = glInterface.CreateBuffers<2>();

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, nullptr);


	glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), uvs, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, nullptr);


	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Scenery::Draw()
{
	glInterface.setModelMatrix(Matrix4::translate(position.x, 0, position.y));
	glInterface.DrawModel(mModel);
}


Vector2f GameObject::GetPosition() const
{
	return position;
}


bool GameObject::IsSolid() const
{
	return solid;
}

/* Scenery Class Functions */

Scenery::Scenery(shared_ptr<Model> model, const Vector2f pos)
{
	position = pos;
	mModel = model;
}

/* DynamicObject Class Functions */


bool DynamicObject::HasExpired() const
{
	return expired;
}

/* Bomb Class Functions */

Bomb::Bomb()
{
	expired = true;
}


Bomb::Bomb(shared_ptr<Model> model, BOMB orig)
{
	expired = false;
	position = orig.position;
	mModel = model;
	mTexture = orig.texture;
}


void Bomb::Draw()
{
	if (!expired)
	{
		glInterface.setModelMatrix(Matrix4::translate(position.x, 0, position.y));
		glInterface.DrawModel(mModel, mTexture);
	}
}


void Bomb::Update(const int millis)
{
	
}

/* Explosion Class Functions */

Explosion::Explosion()
{
	timer = 0;
	expired = true;
	solid = false;
}


Explosion::Explosion(GLint texture, Bomb parent, char** minimap)
{
	position.x = floorf(parent.GetPosition().x);
	position.y = floorf(parent.GetPosition().y);

	timer = 1000;

	SetupExplosion(radius, minimap);

	expired = false;

	for (int i = 0; i < radius; i++)
		delete[] minimap[i];

	delete[] minimap;
	minimap = nullptr;
}


void Explosion::Draw()
{
	glPushMatrix();

	glTranslatef(-0.5f, 0, 0);
	glTranslatef(position.x, 0, position.y);

	glBegin(GL_QUADS);

	glNormal3f(0, 0, 1);

	for (int i = 0; i < 16; i++)
	{
		glTexCoord2f(uvlist[i].x, uvlist[i].y);
		glVertex3f(vertexlist[i].x, vertexlist[i].y, vertexlist[i].z);
	}

	glEnd();

	glPopMatrix();
}


bool Explosion::isWithin(const Vector2i point)
{
	// Has to account for the actual radius of each arm of the explosion, not the highest radius
	if (point.x == position.x)
	{
		if (point.y < position.y)
			return ((position.y - point.y) < vertexlist[14].z);
		else if (point.y > position.y)
			return ((point.y - position.y) < vertexlist[5].z);
		else if (point.y == position.y)
			return true;

	}
	else if (point.y == position.y)
	{
		if (point.x < position.x)
			return ((position.x - point.x) < vertexlist[1].x);
		else if (point.x > position.x)
			return ((point.x - position.x) < vertexlist[10].x);
		else if (point.x == position.x)
			return true;
	}

	return false;
}


void Explosion::SetupExplosion(const int radius, char** minimap)
{
	int vertex = 0;
	int posvertex;

	bool obstaclefound = false;
	// analyse the left direction

	for (posvertex = 0; posvertex >= -radius && !obstaclefound; --posvertex)
		obstaclefound = !(minimap[radius][radius + posvertex] == 'F' || minimap[radius][radius + posvertex] == '@');

	posvertex++;	// add 1 because the value is decremented before the for loop checks

	if (obstaclefound)
		posvertex++;

	uvlist[0] = Vector2f(0.355f, 0.355f);
	vertexlist[0] = Vector3f(0, 0, 0);

	uvlist[1] = Vector2f(0, 0.355f);
	vertexlist[1] = Vector3f(static_cast<float>(posvertex), 0, 0);

	uvlist[2] = Vector2f(0, 0.665f);
	vertexlist[2] = Vector3f(static_cast<float>(posvertex), 0, 1);

	uvlist[3] = Vector2f(0.355f, 0.665f);
	vertexlist[3] = Vector3f(0, 0, 1);

	//analyze down
	obstaclefound = false;

	for (posvertex = 1; posvertex < radius + 1 && !obstaclefound; posvertex++)
		obstaclefound = !(minimap[radius + posvertex][radius] == 'F' || minimap[radius + posvertex][radius] == '@');

	if (obstaclefound)
		posvertex--;

	uvlist[4] = Vector2f(0.355f, 0.665f);
	vertexlist[4] = Vector3f(0, 0, 1);

	uvlist[5] = Vector2f(0.355f, 1);
	vertexlist[5] = Vector3f(0, 0, static_cast<float>(posvertex));

	uvlist[6] = Vector2f(0.665f, 1);
	vertexlist[6] = Vector3f(1, 0, static_cast<float>(posvertex));

	uvlist[7] = Vector2f(0.655f, 0.665f);
	vertexlist[7] = Vector3f(1, 0, 1);

	obstaclefound = false;

	// analyze right
	for (posvertex = 1; posvertex < radius + 1 && !obstaclefound; posvertex++)
		obstaclefound = !(minimap[radius][radius + posvertex] == 'F' || minimap[radius][radius + posvertex] == '@');

	if (obstaclefound)
		posvertex--;

	uvlist[8] = Vector2f(0.665f, 0.355f);
	vertexlist[8] = Vector3f(1, 0, 0);

	uvlist[9] = Vector2f(0.665f, 0.665f);
	vertexlist[9] = Vector3f(1, 0, 1);

	uvlist[10] = Vector2f(1, 0.665f);
	vertexlist[10] = Vector3f(static_cast<float>(posvertex), 0, 1);

	uvlist[11] = Vector2f(1, 0.355f);
	vertexlist[11] = Vector3f(static_cast<float>(posvertex), 0, 0);

	obstaclefound = false;

	//analyze up
	for (posvertex = 0; posvertex >= -radius && !obstaclefound; --posvertex)
		obstaclefound = !(minimap[radius + posvertex][radius] == 'F' || minimap[radius + posvertex][radius] == '@');

	posvertex++;

	if (obstaclefound)
		posvertex++;

	uvlist[12] = Vector2f(0.355f, 0.355f);
	vertexlist[12] = Vector3f(0, 0, 0);

	uvlist[13] = Vector2f(0.665f, 0.355f);
	vertexlist[13] = Vector3f(1, 0, 0);

	uvlist[14] = Vector2f(0.665f, 0);
	vertexlist[14] = Vector3f(1, 0, static_cast<float>(posvertex));

	uvlist[15] = Vector2f(0.355f, 0);
	vertexlist[15] = Vector3f(0, 0, static_cast<float>(posvertex));
}

void Explosion::Update(const int millis)
{
	if (!expired)
	{
		timer -= millis;

		if (timer <= 0)
			expired = true;
	}
}

/* PlayerCharacter class */
PlayerCharacter::PlayerCharacter()
{
	position = Vector2f(0, 0);
}


void PlayerCharacter::Draw()
{
	glInterface.setModelMatrix(Matrix4::translate(position.x, 0, position.y));
	glInterface.DrawModel(mModel, mTexture);
}


void PlayerCharacter::SetPosition(const Vector2f location)
{
	position = location;
}


void PlayerCharacter::SetUp(shared_ptr<Model> model, shared_ptr<Texture> texture, shared_ptr<Texture> bombtex)
{
	mModel = model;
	mTexture = texture;

	mBombTex = bombtex;
}


void PlayerCharacter::Update(const int millis)
{

}

