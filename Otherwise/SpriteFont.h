/*
This is a modified version of the SpriteFont class from the
Seed Of Andromeda source code.
Use it for any of your projects, commercial or otherwise,
free of charge, but do not remove this disclaimer.
THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS
ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO
EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
OF THIS SOFTWARE.
November 28 2014
Original Author: Cristian Zaloj
Modified By: Benjamin Arnold
*/

#pragma once

#ifndef SpriteFont_h__
#define SpriteFont_h__

#include <TTF/SDL_ttf.h>
#include "glm/glm.hpp"
#include <map>
#include <vector>

#include"GL/glew.h"

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
	Vertex(float x, float y, float z, float u, float v, float normalX, float normalY, float normalZ, ColourRGBA8 colour)
		: mX(x), mY(y), mZ(z), mU(u), mV(v), mNormalX(normalX), mNormalY(normalY), mNormalZ(normalZ), mColour(colour) {}

	float mX = 0.0f;
	float mY = 0.0f;
	float mZ = 0.0f;
	float mU = 0.0f;
	float mV = 0.0f;
	float mNormalX = 0.0f;
	float mNormalY = 0.0f;
	float mNormalZ = 0.0f;

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
	void addSprite(glm::vec4 destRect, glm::vec4 uvRect, unsigned int textureID, float depth, ColourRGBA8 colour);
	void setSpriteAlpha(int sprite, GLubyte newAlpha);
	void removeBackSprites(int numberOfSprites)
	{
		for (int i = 0; i < numberOfSprites; i++)
		{
			mSprites.pop_back();
		}
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

	struct CharGlyph {
	public:
		char character;
		glm::vec4 uvRect;
		glm::vec2 size;
	};

#define FIRST_PRINTABLE_CHAR ((char)32)
#define LAST_PRINTABLE_CHAR ((char)126)

	/// For text justification
	enum class Justification {
		LEFT, MIDDLE, RIGHT
	};

	class SpriteFont {
	public:
		SpriteFont() {};
		SpriteFont(const char* font, int size, char cs, char ce);
		SpriteFont(const char* font, int size) :
			SpriteFont(font, size, FIRST_PRINTABLE_CHAR, LAST_PRINTABLE_CHAR) {
		}

		void init(const char* font, int size);
		void init(const char* font, int size, char cs, char ce);

		/// Destroys the font resources
		void dispose();

		int getFontHeight() const {
			return _fontHeight;
		}

		/// Measures the dimensions of the text
		glm::vec2 measure(const char* s);

		/// Draws using a spritebatch
		void draw(MultiSprite& batch, const char* s, glm::vec2 position, glm::vec2 scaling,
			float depth, ColourRGBA8 tint, Justification just = Justification::LEFT);
	private:
		static std::vector<int>* createRows(glm::ivec4* rects, int rectsLength, int r, int padding, int& w);

		int _regStart, _regLength;
		CharGlyph* _glyphs;
		int _fontHeight;

		unsigned int _texID;
	};

}

#endif // SpriteFont_h__