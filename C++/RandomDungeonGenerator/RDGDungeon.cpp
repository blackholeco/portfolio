//
//	RDGDungeon.cpp by Chris Allen
//
//	This file is provided "as-is", for the sole purpose of a demonstration of my
//	work.  It is not intended to be copied or used in an external or third-party
//	project, and no support will be given for that use.
//
//	You may not use or copy this file, in whole or in part, to use for your own
//	projects.  All rights reserved over this file.
//

#include "RDGDungeon.h"


static const unsigned int NODE_RIGHT = 0;
static const unsigned int NODE_DOWN = 1;
static const unsigned int NODE_LEFT = 2;
static const unsigned int NODE_UP = 3;


RDGDungeon::RDGDungeon(unsigned int width, unsigned int height)
{
	mDimensions = NovaVectorUtil::newInstance(width, height);
}


void RDGDungeon::addToScene()
{
	nContext->getMainScene()->add({ &mFloor, &mGeometry });
}


void RDGDungeon::update(long millis)
{

}


void RDGDungeon::setupStage()
{
	// Load the package for the App
	mPackage.load("PYE_PACKAGE.nova");

	// Generate the layout
	generate();
}


void RDGDungeon::generate()
{
	_node* pNode;

	// Adjust dimensions to be odd-numbered (allows for surrounding walls)
	if (static_cast<int>(mDimensions.x) % 2 == 0)
		mDimensions.x += 1;

	if (static_cast<int>(mDimensions.y) % 2 == 0)
		mDimensions.y += 1;

	const unsigned int halfWidth = static_cast<const int>(mDimensions.x / 2);
	const unsigned int halfHeight = static_cast<const int>(mDimensions.y / 2);

	mNodeList = DBG_NEW _node*[halfHeight];

	for (int i = 0; i < halfHeight; i++)
	{
		mNodeList[i] = DBG_NEW _node[halfWidth];

		for (int j = 0; j < halfWidth; j++)
		{
			pNode = &mNodeList[i][j];

			pNode->position = NovaVectorUtil::newInstance(j, i);
			pNode->visited = false;
			pNode->walls.set();
			pNode->previous = nullptr;

			if (i == 0)
				pNode->neighbours[NODE_UP] = nullptr;
			else
			{
				if (i == halfHeight - 1)
					pNode->neighbours[NODE_DOWN] = nullptr;

				pNode->neighbours[NODE_UP] = &mNodeList[i - 1][j];
				pNode->neighbours[NODE_UP]->neighbours[NODE_DOWN] = pNode;
			}

			if (j == 0)
				pNode->neighbours[NODE_LEFT] = nullptr;
			else
			{
				if (j == halfWidth - 1)
					pNode->neighbours[NODE_RIGHT] = nullptr;

				pNode->neighbours[NODE_LEFT] = &mNodeList[i][j - 1];
				pNode->neighbours[NODE_LEFT]->neighbours[NODE_RIGHT] = pNode;
			}
		}
	}

	// Node list allocated
	makePath();
	makeGeometry();

	for (int i = 0; i < halfHeight; i++)
		delete[] mNodeList[i];

	delete[] mNodeList;
	mNodeList = nullptr;
}


void RDGDungeon::makePath()
{
	_node* pNode = &mNodeList[0][0];
	bool complete = false;
	bitset<4> tested("0000");
	bool found = false;
	unsigned int direction;

	// Always want this to run at least once, hence the do-while
	do
	{
		pNode->visited = true;

		// Check that this node has an unvisited neighbour - if not, go backwards
		while (pNode != nullptr && !hasPath(pNode))
			pNode = pNode->previous;

		if (pNode == nullptr)
			return;

		tested.reset();

		found = false;

		do
		{
			direction = NovaRandom::randomInt(0, 3);

			if (!tested.test(direction))
				found = hasPath(pNode, direction);

			tested.set(direction);

		} while (!found && !tested.all());

		if (found)
			pNode = nextNode(pNode, direction);

		if (pNode == nullptr)
			complete = true;

	} while (!complete);

	// Layout generation complete
}


void RDGDungeon::makeGeometry()
{
	// Start with a simplified layout
	bool** layout = nullptr;
	vector<NovaVector3> vertex;
	vector<NovaVector2> uv;
	vector<unsigned int> index;
	nModelData data;
	nImage mGTexture;
	nImage mFTexture;

	const int width = static_cast<const int>(mDimensions.x);
	const int height = static_cast<const int>(mDimensions.y);

	layout = DBG_NEW bool*[height];

	for (int i = 0; i < height; i++)
	{
		layout[i] = DBG_NEW bool[width];

		for (int j = 0; j < width; j++)
		{
			if (i == 0 || j == 0)
				layout[i][j] = true;
			else if ((i - 1) % 2 == 1)
			{
				if ((j - 1) % 2 == 1)
					layout[i][j] = true;
				else
					layout[i][j] = mNodeList[(i - 1) / 2][(j - 1) / 2].walls.test(NODE_DOWN);
			}
			else
			{
				if ((j - 1) % 2 == 1)
					layout[i][j] = mNodeList[(i - 1) / 2][(j - 1) / 2].walls.test(NODE_RIGHT);
				else
					layout[i][j] = false;
			}
		}
	}

	// Simple layout complete - convert to geometry

	unsigned int element = 0;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (layout[i][j])
			{
				// apply the top plane
				vertex.push_back(NovaVectorUtil::newInstance(
					j - 0.5f, 0.5f, i + 0.5f));

				vertex.push_back(NovaVectorUtil::newInstance(
					j + 0.5f, 0.5f, i + 0.5f));

				vertex.push_back(NovaVectorUtil::newInstance(
					j - 0.5f, 0.5f, i - 0.5f));

				vertex.push_back(NovaVectorUtil::newInstance(
					j + 0.5f, 0.5f, i - 0.5f));

				uv.push_back(NovaVectorUtil::newInstance(0.0f, 0.0f));
				uv.push_back(NovaVectorUtil::newInstance(1.0f, 0.0f));
				uv.push_back(NovaVectorUtil::newInstance(0.0f, 1.0f));
				uv.push_back(NovaVectorUtil::newInstance(1.0f, 1.0f));

				index.push_back(element);
				index.push_back(element + 3);
				index.push_back(element + 2);
				index.push_back(element);
				index.push_back(element + 1);
				index.push_back(element + 3);

				element += 4;

				// check the left hand side
				if (j == 0 || !layout[i][j - 1])
				{
					vertex.push_back(NovaVectorUtil::newInstance(
						j - 0.5f, -0.5f, i - 0.5f));

					vertex.push_back(NovaVectorUtil::newInstance(
						j - 0.5f, -0.5f, i + 0.5f));

					vertex.push_back(NovaVectorUtil::newInstance(
						j - 0.5f, 0.5f, i - 0.5f));

					vertex.push_back(NovaVectorUtil::newInstance(
						j - 0.5f, 0.5f, i + 0.5f));

					uv.push_back(NovaVectorUtil::newInstance(0.0f, 0.0f));
					uv.push_back(NovaVectorUtil::newInstance(1.0f, 0.0f));
					uv.push_back(NovaVectorUtil::newInstance(0.0f, 1.0f));
					uv.push_back(NovaVectorUtil::newInstance(1.0f, 1.0f));

					index.push_back(element);
					index.push_back(element + 3);
					index.push_back(element + 2);
					index.push_back(element);
					index.push_back(element + 1);
					index.push_back(element + 3);

					element += 4;
				}

				// check the right hand side
				if (j == width - 1 || !layout[i][j + 1])
				{
					vertex.push_back(NovaVectorUtil::newInstance(j + 0.5f, -0.5f, i + 0.5f));
					vertex.push_back(NovaVectorUtil::newInstance(j + 0.5f, -0.5f, i - 0.5f));
					vertex.push_back(NovaVectorUtil::newInstance(j + 0.5f, 0.5f, i + 0.5f));
					vertex.push_back(NovaVectorUtil::newInstance(j + 0.5f, 0.5f, i - 0.5f));

					uv.push_back(NovaVectorUtil::newInstance(0.0f, 0.0f));
					uv.push_back(NovaVectorUtil::newInstance(1.0f, 0.0f));
					uv.push_back(NovaVectorUtil::newInstance(0.0f, 1.0f));
					uv.push_back(NovaVectorUtil::newInstance(1.0f, 1.0f));

					index.push_back(element);
					index.push_back(element + 3);
					index.push_back(element + 2);
					index.push_back(element);
					index.push_back(element + 1);
					index.push_back(element + 3);

					element += 4;
				}

				// check the front
				if (i == height - 1 || !layout[i + 1][j])
				{
					vertex.push_back(NovaVectorUtil::newInstance(j - 0.5f, -0.5f, i + 0.5f));
					vertex.push_back(NovaVectorUtil::newInstance(j + 0.5f, -0.5f, i + 0.5f));
					vertex.push_back(NovaVectorUtil::newInstance(j - 0.5f, 0.5f, i + 0.5f));
					vertex.push_back(NovaVectorUtil::newInstance(j + 0.5f, 0.5f, i + 0.5f));

					uv.push_back(NovaVectorUtil::newInstance(0.0f, 0.0f));
					uv.push_back(NovaVectorUtil::newInstance(1.0f, 0.0f));
					uv.push_back(NovaVectorUtil::newInstance(0.0f, 1.0f));
					uv.push_back(NovaVectorUtil::newInstance(1.0f, 1.0f));

					index.push_back(element);
					index.push_back(element + 3);
					index.push_back(element + 2);
					index.push_back(element);
					index.push_back(element + 1);
					index.push_back(element + 3);

					element += 4;
				}
			}
		}
	}

	data.vertices = DBG_NEW float[vertex.size() * 3];
	data.uvs = DBG_NEW float[uv.size() * 2];
	data.indices = DBG_NEW unsigned int[index.size()];

	data.elementCount = static_cast<unsigned int>(vertex.size());
	data.indexCount = static_cast<unsigned int>(index.size());

	for (unsigned int i = 0; i < data.elementCount; i++)
	{
		memcpy_s(&data.vertices[i * 3], 3 * sizeof(float), vertex.at(i).xyz.data(),
			3 * sizeof(float));

		memcpy_s(&data.uvs[i * 2], 2 * sizeof(float), uv.at(i).xy.data(), 2 * sizeof(float));
	}

	memcpy_s(data.indices, data.indexCount * sizeof(unsigned int), index.data(),
		data.indexCount * sizeof(unsigned int));

	vertex.clear();
	uv.clear();
	index.clear();

	mGTexture = mPackage.findImage("WALL");
	mFTexture = mPackage.findImage("FLOOR");

	mGeometry.model = nContext->getModelFactory()->newModel();
	mGeometry.model->createFromData(data, mGTexture);
	mGeometry.model->build();
	mGeometry.model->cleanUp();
	mGeometry.colour = NovaColour::WHITE;
	mGeometry.transMask = NovaColour::NONE;
	mGeometry.transform.setPosition(static_cast<float>(-width / 2), 0, static_cast<float>(-height / 2));

	mFloor.model = nContext->getModelFactory()->newModel();
	mFloor.model->createFromData(nContext->getRenderer()->getPlane(), mFTexture);
	mFloor.model->build();
	mFloor.colour = NovaColour::WHITE;
	mFloor.transMask = NovaColour::NONE;
	mFloor.transform.setPosition(0, -0.5f, 0)
		.setRotation(90, 1, 0, 0)
		.setScale(mDimensions.x, mDimensions.y, 1);


	// delete the layout
	for (int i = 0; i < height; i++)
		delete[] layout[i];

	delete[] layout;
	layout = nullptr;
}


bool RDGDungeon::hasPath(_node* pNode)
{
	if (pNode == nullptr)
		return false;

	for (int i = 0; i < 4; i++)
	{
		if (pNode->neighbours[i] != nullptr && !pNode->neighbours[i]->visited)
			return true;
	}

	return false;
}


bool RDGDungeon::hasPath(_node* pNode, unsigned int direction)
{
	if (pNode == nullptr)
		return false;

	if (direction > 3)
		return false;

	return pNode->neighbours[direction] != nullptr && !pNode->neighbours[direction]->visited;
}


RDGDungeon::_node* RDGDungeon::nextNode(_node* pNode, unsigned int direction)
{
	_node* ret = nullptr;

	if (pNode == nullptr || direction > 3)
		return nullptr;

	ret = pNode->neighbours[direction];

	if (ret != nullptr)
	{
		pNode->walls.set(direction, false);
		ret->previous = pNode;

		if (direction < 2)
			ret->walls.set(direction + 2, false);
		else
			ret->walls.set(direction - 2, false);
	}

	return ret;
}

