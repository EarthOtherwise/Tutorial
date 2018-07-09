#pragma once
#include"GL/glew.h"
#include<vector>

namespace Otherwise
{
	struct ColourRGBA8
	{
		ColourRGBA8() : mR(255), mG(255), mB(255), mA(255) {}
		ColourRGBA8(GLubyte r, GLubyte g, GLubyte b, GLubyte a) : mR(r), mG(g), mB(b), mA(a) {}
		GLubyte mR;
		GLubyte mG;
		GLubyte mB;
		GLubyte mA;
	};
	struct Vertex
	{
		Vertex() {}
		Vertex(float x, float y, float u, float v, ColourRGBA8 colour) 
		: mX(x), mY(y), mU(u), mV(v), mColour(colour) {}

		float mX = 0.0f;
		float mY = 0.0f;
		float mU = 0.0f;
		float mV = 0.0f;

		ColourRGBA8 mColour;
	};

	struct Sprite
	{
		Sprite(GLuint textureID, float depth, Vertex &bottomLeft, Vertex &topLeft, Vertex &bottomRight,
			Vertex &topRight) : mBottomLeft(bottomLeft), mTopLeft(topLeft), mBottomRight(bottomRight),
			mTopRight(topRight), mTextureID(textureID), mDepth(depth) {}

		GLuint mTextureID = 0;
		float mDepth = 0.0f;

		Vertex mBottomLeft;
		Vertex mBottomRight;
		Vertex mTopLeft;
		Vertex mTopRight;
	};

	struct SpriteBatch
	{
		SpriteBatch(GLuint offset, GLuint numVertices, GLuint textureID)
			: mOffset(offset), mNumVerticies(numVertices), mTextureID(textureID) {}

		GLuint mTextureID = 0;
		GLuint mNumVerticies = 0;
		GLuint mOffset = 0;
	};

	class MultiSprite
	{
	public:
		MultiSprite();
		~MultiSprite();

		void init();

		void clearCache();

		void addSprite(GLuint textureID, float depth, Vertex &bottomLeft, Vertex &topLeft, Vertex &bottomRight,
			Vertex &topRight)
		{
			mSprites.emplace_back(textureID, depth, bottomLeft, topLeft, bottomRight, topRight);
		}

		void prepareBatches();

		void renderBatches();
	private:
		static bool compareTexture(Sprite* a, Sprite* b)
		{
			return (a->mTextureID < b->mTextureID);
		}

		GLuint mVbo = 0;
		GLuint mVao = 0;

		std::vector<Sprite*> mSortingSprites;
		std::vector<Sprite> mSprites;
		std::vector<SpriteBatch> mSpriteBatches;
	};

}