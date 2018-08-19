#include "MultiSprite.h"
#include"ErrHandler.h"
#include<algorithm>

namespace Otherwise
{

	MultiSprite::MultiSprite() : mVbo(0), mVao(0)
	{
	}


	MultiSprite::~MultiSprite()
	{
	}

	void MultiSprite::init()
	{
		if (mVao != 0)
		{
			throwError("error", "You tried to call init for a sprite batch twice dumbass");
			return;
		}
		glGenVertexArrays(1, &mVao);

		glBindVertexArray(mVao);

		glGenBuffers(1, &mVbo);

		glBindBuffer(GL_ARRAY_BUFFER, mVbo);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mX));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mU));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, mColour));

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mNormalX));

		glBindVertexArray(0);
	}

	void MultiSprite::clearCache()
	{
		mSprites.clear();
		mSpriteBatches.clear();
	}

	void MultiSprite::addSprite(glm::vec4 destRect, glm::vec4 uvRect, unsigned int textureID, float depth, ColourRGBA8 colour)
	{
		Vertex bottomLeft(destRect.x, destRect.y, depth, uvRect.x, uvRect.y, 0.0f, 0.0f, 1.0f, colour);
		Vertex bottomRight(destRect.x + destRect.z, destRect.y, depth, uvRect.x + uvRect.z, uvRect.y, 0.0f, 0.0f, 1.0f, colour);
		Vertex topLeft(destRect.x, destRect.y + destRect.w, depth, uvRect.x, uvRect.y + uvRect.w, 0.0f, 0.0f, 1.0f, colour);
		Vertex topRight(destRect.x + destRect.z, destRect.y + destRect.w, depth, uvRect.x + uvRect.z, uvRect.y + uvRect.w, 0.0f, 0.0f, 1.0f, colour);

		mSprites.emplace_back(textureID, depth, bottomLeft, topLeft, bottomRight, topRight);
	}

	void MultiSprite::setSpriteAlpha(int sprite, GLubyte newAlpha)
	{
		mSprites[sprite].mBottomLeft.mColour.mA = newAlpha;
		mSprites[sprite].mTopLeft.mColour.mA = newAlpha;
		mSprites[sprite].mBottomRight.mColour.mA = newAlpha;
		mSprites[sprite].mTopRight.mColour.mA = newAlpha;
	}

	void MultiSprite::prepareBatches()
	{
		if (mSprites.empty())
		{
			throwError("error", "You tried to prepare sprite batches without any sprites");
			return;
		}

		mSortingSprites.resize(mSprites.size());
		for (size_t i = 0; i < mSprites.size(); i++)
		{
			mSortingSprites[i] = &mSprites[i];
		}

		std::sort(mSortingSprites.begin(), mSortingSprites.end(), compareTexture);

		std::vector<Vertex> verticies;
		verticies.resize(mSortingSprites.size() * 6);

		int currentOffset = 0;
		int currentVertex = 0;

		mSpriteBatches.emplace_back(currentOffset, 6, mSortingSprites[0]->mTextureID);
		verticies[currentVertex++] = mSortingSprites[0]->mTopLeft;
		verticies[currentVertex++] = mSortingSprites[0]->mBottomLeft;
		verticies[currentVertex++] = mSortingSprites[0]->mBottomRight;
		verticies[currentVertex++] = mSortingSprites[0]->mBottomRight;
		verticies[currentVertex++] = mSortingSprites[0]->mTopRight;
		verticies[currentVertex++] = mSortingSprites[0]->mTopLeft;

		currentOffset += 6;

		for (size_t currentSprite = 1; currentSprite < mSortingSprites.size(); currentSprite++)
		{
			if (mSortingSprites[currentSprite]->mTextureID != mSortingSprites[currentSprite - 1]->mTextureID)
			{
				mSpriteBatches.emplace_back(currentOffset, 6, mSortingSprites[currentSprite]->mTextureID);
			}
			else
			{
				mSpriteBatches.back().mNumVerticies += 6;
			}

			verticies[currentVertex++] = mSortingSprites[currentSprite]->mTopLeft;
			verticies[currentVertex++] = mSortingSprites[currentSprite]->mBottomLeft;
			verticies[currentVertex++] = mSortingSprites[currentSprite]->mBottomRight;
			verticies[currentVertex++] = mSortingSprites[currentSprite]->mBottomRight;
			verticies[currentVertex++] = mSortingSprites[currentSprite]->mTopRight;
			verticies[currentVertex++] = mSortingSprites[currentSprite]->mTopLeft;

			currentOffset += 6;
		}

		glBindBuffer(GL_ARRAY_BUFFER, mVbo);

		glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

		glBufferSubData(GL_ARRAY_BUFFER, 0, verticies.size() * sizeof(Vertex), verticies.data());

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void MultiSprite::renderBatches()
	{
		glBindVertexArray(mVao);

		/*GLuint queryID;
		glGenQueries(1, &queryID);*/

		for (unsigned int i = 0; i < mSpriteBatches.size(); i++)
		{
			glBindTexture(GL_TEXTURE_2D, mSpriteBatches[i].mTextureID);

			//glBeginQuery(GL_SAMPLES_PASSED, queryID);

			glDrawArrays(GL_TRIANGLES, mSpriteBatches[i].mOffset, mSpriteBatches[i].mNumVerticies);

			/*glEndQuery(GL_SAMPLES_PASSED);
			GLuint query = GL_FALSE;
			while (query == GL_FALSE)
			{
				glGetQueryObjectuiv(queryID, GL_QUERY_RESULT_AVAILABLE, &query);
			}
			glGetQueryObjectuiv(queryID, GL_QUERY_RESULT, &query);
			std::cout << "Sprite " << i << '\n' << query << '\n';*/
		}

		glBindVertexArray(0);
	}
}