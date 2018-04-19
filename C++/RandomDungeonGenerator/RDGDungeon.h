#pragma once

//
//	RDGDungeon.h by Chris Allen
//
//	This file is provided "as-is", for the sole purpose of a demonstration of my
//	work.  It is not intended to be copied or used in an external or third-party
//	project, and no support will be given for that use.
//
//	You may not use or copy this file, in whole or in part, to use for your own
//	projects.  All rights reserved over this file.
//

#include <NovaStage.h>
#include <NovaPackage.h>

#include <bitset>

using std::bitset;

/**
 *	Class to handle the RandomDungeonGenerator's Stage, creating and holding
	the dungeon geometry, and displaying it
 */
class RDGDungeon : public NovaStage
{
public:
	RDGDungeon(unsigned int width, unsigned int height);

	void update(long millis);
	void addToScene();
	void setupStage();
	void release() {};

protected:
	struct _node
	{
		NovaVector2 position; // Position in array for debug purposes
		_node* neighbours[4];
		_node* previous;
		bitset<4> walls;
		bool visited;
	};

	NovaPackage mPackage;
	NovaVector2 mDimensions;
	Model mGeometry;
	Model mFloor;

	_node** mNodeList;

	void generate();
	void makePath();
	void makeGeometry();
	bool hasPath(_node* pNode);
	bool hasPath(_node* pNode, unsigned int direction);
	_node* nextNode(_node* pNode, unsigned int direction);
};

