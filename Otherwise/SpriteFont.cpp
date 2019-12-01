#include "SpriteFont.h"

#include <SDL/SDL.h>
#include"ErrHandler.h"
#include<algorithm>

int closestPow2(int i) {
	i--;
	int pi = 1;
	while (i > 0) {
		i >>= 1;
		pi <<= 1;
	}
	return pi;
}

#define MAX_TEXTURE_RES 4096

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

	SpriteFont::SpriteFont(const char* font, int size, char cs, char ce) {
		init(font, size, cs, ce);
	}

	void SpriteFont::init(const char* font, int size) {
		init(font, size, FIRST_PRINTABLE_CHAR, LAST_PRINTABLE_CHAR);
	}

	void SpriteFont::init(const char* font, int size, char cs, char ce) {
		// Initialize SDL_ttf
		if (!TTF_WasInit()) {
			TTF_Init();
		}
		TTF_Font* f = TTF_OpenFont(font, size);
		if (f == nullptr) {
			fprintf(stderr, "Failed to open TTF font %s\n", font);
			fflush(stderr);
			throw 281;
		}
		_fontHeight = TTF_FontHeight(f);
		_regStart = cs;
		_regLength = ce - cs + 1;
		int padding = size / 8;

		// First neasure all the regions
		glm::ivec4* glyphRects = new glm::ivec4[_regLength];
		int i = 0, advance;
		for (char c = cs; c <= ce; c++) {
			TTF_GlyphMetrics(f, c, &glyphRects[i].x, &glyphRects[i].z, &glyphRects[i].y, &glyphRects[i].w, &advance);
			glyphRects[i].z -= glyphRects[i].x;
			glyphRects[i].x = 0;
			glyphRects[i].w -= glyphRects[i].y;
			glyphRects[i].y = 0;
			i++;
		}

		// Find best partitioning of glyphs
		int rows = 1, w, h, bestWidth = 0, bestHeight = 0, area = MAX_TEXTURE_RES * MAX_TEXTURE_RES, bestRows = 0;
		std::vector<int>* bestPartition = nullptr;
		while (rows <= _regLength) {
			h = rows * (padding + _fontHeight) + padding;
			auto gr = createRows(glyphRects, _regLength, rows, padding, w);

			// Desire a power of 2 texture
			w = closestPow2(w);
			h = closestPow2(h);

			// A texture must be feasible
			if (w > MAX_TEXTURE_RES || h > MAX_TEXTURE_RES) {
				rows++;
				delete[] gr;
				continue;
			}

			// Check for minimal area
			if (area >= w * h) {
				if (bestPartition) delete[] bestPartition;
				bestPartition = gr;
				bestWidth = w;
				bestHeight = h;
				bestRows = rows;
				area = bestWidth * bestHeight;
				rows++;
			}
			else {
				delete[] gr;
				break;
			}
		}

		// Can a bitmap font be made?
		if (!bestPartition) {
			fprintf(stderr, "Failed to Map TTF font %s to texture. Try lowering resolution.\n", font);
			fflush(stderr);
			throw 282;
		}
		// Create the texture
		glGenTextures(1, &_texID);
		glBindTexture(GL_TEXTURE_2D, _texID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bestWidth, bestHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		// Now draw all the glyphs
		SDL_Color fg = { 255, 255, 255, 255 };
		int ly = padding;
		for (int ri = 0; ri < bestRows; ri++) {
			int lx = padding;
			for (size_t ci = 0; ci < bestPartition[ri].size(); ci++) {
				int gi = bestPartition[ri][ci];

				SDL_Surface* glyphSurface = TTF_RenderGlyph_Blended(f, (char)(cs + gi), fg);

				// Pre-multiplication occurs here
				unsigned char* sp = (unsigned char*)glyphSurface->pixels;
				int cp = glyphSurface->w * glyphSurface->h * 4;
				for (int i = 0; i < cp; i += 4) {
					float a = sp[i + 3] / 255.0f;
					sp[i] = (unsigned char)((float)sp[i] * a);
					sp[i + 1] = sp[i];
					sp[i + 2] = sp[i];
				}

				// Save glyph image and update coordinates
				glTexSubImage2D(GL_TEXTURE_2D, 0, lx, bestHeight - ly - 1 - glyphSurface->h, glyphSurface->w, glyphSurface->h, GL_BGRA, GL_UNSIGNED_BYTE, glyphSurface->pixels);
				glyphRects[gi].x = lx;
				glyphRects[gi].y = ly;
				glyphRects[gi].z = glyphSurface->w;
				glyphRects[gi].w = glyphSurface->h;

				SDL_FreeSurface(glyphSurface);
				glyphSurface = nullptr;

				lx += glyphRects[gi].z + padding;
			}
			ly += _fontHeight + padding;
		}

		// Draw the unsupported glyph
		int rs = padding - 1;
		int* pureWhiteSquare = new int[rs * rs];
		memset(pureWhiteSquare, 0xffffffff, rs * rs * sizeof(int));
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, rs, rs, GL_RGBA, GL_UNSIGNED_BYTE, pureWhiteSquare);
		delete[] pureWhiteSquare;
		pureWhiteSquare = nullptr;

		// Set some texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// Create spriteBatch glyphs
		_glyphs = new CharGlyph[_regLength + 1];
		for (i = 0; i < _regLength; i++) {
			_glyphs[i].character = (char)(cs + i);
			_glyphs[i].size = glm::vec2(glyphRects[i].z, glyphRects[i].w);
			_glyphs[i].uvRect = glm::vec4(
				(float)glyphRects[i].x / (float)bestWidth,
				(float)glyphRects[i].y / (float)bestHeight,
				(float)glyphRects[i].z / (float)bestWidth,
				(float)glyphRects[i].w / (float)bestHeight
				);
		}
		_glyphs[_regLength].character = ' ';
		_glyphs[_regLength].size = _glyphs[0].size;
		_glyphs[_regLength].uvRect = glm::vec4(0, 0, (float)rs / (float)bestWidth, (float)rs / (float)bestHeight);

		glBindTexture(GL_TEXTURE_2D, 0);
		delete[] glyphRects;
		delete[] bestPartition;
		TTF_CloseFont(f);
	}

	void SpriteFont::dispose() {
		if (_texID != 0) {
			glDeleteTextures(1, &_texID);
			_texID = 0;
		}
		if (_glyphs) {
			delete[] _glyphs;
			_glyphs = nullptr;
		}
	}

	std::vector<int>* SpriteFont::createRows(glm::ivec4* rects, int rectsLength, int r, int padding, int& w) {
		// Blank initialize
		std::vector<int>* l = new std::vector<int>[r]();
		int* cw = new int[r]();
		for (int i = 0; i < r; i++) {
			cw[i] = padding;
		}

		// Loop through all glyphs
		for (int i = 0; i < rectsLength; i++) {
			// Find row for placement
			int ri = 0;
			for (int rii = 1; rii < r; rii++)
				if (cw[rii] < cw[ri]) ri = rii;

			// Add width to that row
			cw[ri] += rects[i].z + padding;

			// Add glyph to the row list
			l[ri].push_back(i);
		}

		// Find the max width
		w = 0;
		for (int i = 0; i < r; i++) {
			if (cw[i] > w) w = cw[i];
		}

		return l;
	}

	glm::vec2 SpriteFont::measure(const char* s) {
		glm::vec2 size(0, _fontHeight);
		float cw = 0;
		for (int si = 0; s[si] != 0; si++) {
			char c = s[si];
			if (s[si] == '\n') {
				size.y += _fontHeight;
				if (size.x < cw)
					size.x = cw;
				cw = 0;
			}
			else {
				// Check for correct glyph
				int gi = c - _regStart;
				if (gi < 0 || gi >= _regLength)
					gi = _regLength;
				cw += _glyphs[gi].size.x;
			}
		}
		if (size.x < cw)
			size.x = cw;
		return size;
	}

	void SpriteFont::draw(MultiSprite& batch, const char* s, glm::vec2 position, glm::vec2 scaling,
		float depth, ColourRGBA8 tint, Justification just /* = Justification::LEFT */) {
		glm::vec2 tp = position;
		// Apply justification
		if (just == Justification::MIDDLE) {
			tp.x -= measure(s).x * scaling.x / 2;
		}
		else if (just == Justification::RIGHT) {
			tp.x -= measure(s).x * scaling.x;
		}
		for (int si = 0; s[si] != 0; si++) {
			char c = s[si];
			if (s[si] == '\n') {
				tp.y += _fontHeight * scaling.y;
				tp.x = position.x;
			}
			else {
				// Check for correct glyph
				int gi = c - _regStart;
				if (gi < 0 || gi >= _regLength)
					gi = _regLength;
				glm::vec4 destRect(tp, _glyphs[gi].size * scaling);
				batch.addSprite(destRect, _glyphs[gi].uvRect, _texID, depth, tint);
				tp.x += _glyphs[gi].size.x * scaling.x;
			}
		}
	}

}