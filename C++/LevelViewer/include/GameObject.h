/*
 *	GameObject.h by Chris Allen

 *	This file is provided "as-is", for the sole purpose of a demonstration of my
	work.  It is not intended to be copied or used in an external or third-party
	project, and no support will be given for that use.

 *	You may not use or copy this file, in whole or in part, to use for your own
	projects.  All rights reserved over this file.
 */

#pragma once

#include <deque>

#include "GLInterface.h"

using std::deque;

/**
 *	Abstract base class for all Objects in the world
 */
class GameObject
{
public:
	virtual ~GameObject();

	/**
	 *	Render this object to the scene
	 */
	virtual void Draw() = 0;

	/**
	 *	Get this Object's position

	 *	@return This object's position as a 2D vector
	 */
	Vector2f GetPosition() const;

	/**
	 *	Check if this object is solid (prevents movement past/through)

	 *	@return true if the object is solid
	 */
	bool IsSolid() const;

protected:
	bool solid;
	Vector2f position;
	shared_ptr<Model> mModel;
};

/**
 *	Class to define scenery objects, such as map walls
 */
class Scenery : public GameObject
{
public:
	Scenery(shared_ptr<Model> model, const Vector2f position);

	/**
	 *	Render this object to scene
	 */
	void Draw();
};

/**
 *	Class to define the Floor of the level.

 *	Renders as a simple textured plane
 */
class Floor : public GameObject
{
public:
	Floor();

	/**
	 *	Set up this Floor

	 *	@param width : Floor's width
	 *	@param height : Floor's height
	 *	@param texture : Floor's texture
	 */
	void Initialize(unsigned int width, unsigned int height, shared_ptr<Texture> texture);

	/**
	 *	Import this Floor's model data into graphics
	 */
	void Import();

	/**
	 *	Render the floor in the scene
	 */
	void Draw();

protected:
	array<unsigned int, 2> buffer;

	unsigned int vao;

	shared_ptr<Texture> mTexture;

	unsigned int mWidth;
	unsigned int mHeight;
};

/*
 *	Abstract base class for any object in the game world that changes and requires updating
 */
class DynamicObject : public GameObject
{
public:

	/**
	 *	Check if this object is expired (no longer needed)

	 *	@return true if the object is expired
	 */
	bool HasExpired() const;

	/**
	 *	Update this object

	 *	@param millis : Time in milliseconds since the last update
	 */
	virtual void Update(const int millis) = 0;

protected:
	bool expired;
};

/*
 *	Structure defining a Bomb, allowing for easier passing and writing
 */
typedef struct _bomb
{
	shared_ptr<Texture> texture;
	Vector2i position;
	int radius;
}BOMB;

/*
 *	Class to handle a Bomb in the game world

 *	Bombs last for around five seconds, spawning an explosion afterwards
 */
class Bomb : public DynamicObject
{
public:
	Bomb();
	Bomb(shared_ptr<Model> model, BOMB original);

	/**
	 *	Render this Bomb to scene
	 */
	void Draw();

	/**
	 *	CURRENTLY UNIMPLEMENTED

	 *	Update the Bomb

	 *	@param millis : Length of time in milliseconds since the last update
	 */
	void Update(const int millis);

private:
	shared_ptr<Texture> mTexture;
};

/**
 *	Class to define an Explosion in the game world.

 *	Explosions spread out in a + shape from their origin (where the Bomb was placed)
 */
class Explosion : public DynamicObject
{
public:
	Explosion();
	Explosion(GLint texture, Bomb parent, char** minimap);

	/**
	 *	Render this Explosion to screen

	 *	TODO Currently draws using depreciated glVertex* commands.  Update to use shaders
	 */
	void Draw();

	/**
	 *	Check if a point on the map is covered by this Explosion

	 *	@param point : The point on the map to check

	 *	@return true if the point is covered by the Explosion
	 */
	bool isWithin(const Vector2i point);

	/**
	 *	Define this Explosion's graphic

	 *	@param radius : The maximum radius of the Explosion in all directions from the origin

	 *	@param minimap : A 2D array representing the area of the map around the Explosion.
						 Used for checking if there are objects in range that block the Explosion.
	 */
	void SetupExplosion(const int radius, char** minimap);

	/**
	 *	Update the Explosion

	 *	@param millis : Amount of time in milliseconds since the last update
	 */
	void Update(const int millis);

protected:
	float timer;
	int radius;

	Vector2f uvlist[16];
	Vector3f vertexlist[16];
};

/**
 *	Class to manage the Player's character object
 */
class PlayerCharacter : public DynamicObject
{
public:
	PlayerCharacter();

	/**
	 *	Draw the player's character at their current position
	 */
	void Draw();

	/**
	 *	Set a character's position in the map

	 * @param location The Player's new position
	 *
	 * TODO attempt to implement a method to check the position validity
	 */
	void SetPosition(const Vector2f location);

	/**
	 *	Initializes the player for a new game

	 *	@param model : The 3D Model graphic to use for this Player
	 *	@param texture : The Texture to apply to the 3D model
	 *	@param bombtex : The Texture to use for the character's Bomb object
	 */
	void SetUp(shared_ptr<Model> model, shared_ptr<Texture> texture, shared_ptr<Texture> bombtex);

	/**
	 *	CURRENTLY UNIMPLEMENTED

	 *	Update the Player object

	 *	@param millis : Time in milliseconds since the last update
	 */
	void Update(const int millis);

private:
	shared_ptr<Texture> mTexture;
	shared_ptr<Texture> mBombTex;
};
