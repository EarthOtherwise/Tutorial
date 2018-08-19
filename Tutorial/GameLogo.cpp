#include "GameLogo.h"
#include "SDL/sdl.h"


GameLogo::GameLogo()
{
}


GameLogo::~GameLogo()
{
}

void GameLogo::init(std::string vertShader, std::string fragShader, int screenWidth, int screenHeight, glm::vec2 cameraPosition, float cameraZoom, Otherwise::Window* windowptr)
{
	mProgramID = Otherwise::compileLinkSimpleShaders(vertShader, fragShader);
	mPerspectiveUniformID = glGetUniformLocation(mProgramID, "Perspective");
	mCamera.init(screenWidth, screenHeight, cameraPosition, cameraZoom);
	mOrthographicMatrix = mCamera.getMatrix();
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;
	mWindow = windowptr;
	mMultiSprite.init();
}

void GameLogo::logoUpdateRenderLoop()
{
	glDisable(GL_DEPTH_TEST);
	
	GLuint starscapeTextureID = Otherwise::loadPng("Textures/starscape.png");

	float quarterScreenHeight = mScreenHeight / 4.0f;
	float threeQuartersScreenHeight = mScreenHeight - quarterScreenHeight;

	mMultiSprite.addSprite(starscapeTextureID, 0.0f,
		Otherwise::Vertex(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
		Otherwise::Vertex(0.0f, mScreenHeight, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
		Otherwise::Vertex(mScreenWidth, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
		Otherwise::Vertex(mScreenWidth, mScreenHeight, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)));

	mMultiSprite.addSprite(starscapeTextureID, 0.0f,
		Otherwise::Vertex(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(0, 0, 0, 255)),
		Otherwise::Vertex(0.0f, quarterScreenHeight, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(0, 0, 0, 255)),
		Otherwise::Vertex(mScreenWidth, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(0, 0, 0, 255)),
		Otherwise::Vertex(mScreenWidth, quarterScreenHeight, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(0, 0, 0, 255)));

	mMultiSprite.addSprite(starscapeTextureID, 0.0f,
		Otherwise::Vertex(0.0f, threeQuartersScreenHeight, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(0, 0, 0, 255)),
		Otherwise::Vertex(0.0f, mScreenHeight, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(0, 0, 0, 255)),
		Otherwise::Vertex(mScreenWidth, threeQuartersScreenHeight, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(0, 0, 0, 255)),
		Otherwise::Vertex(mScreenWidth, mScreenHeight, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(0, 0, 0, 255)));

	mMultiSprite.addSprite(Otherwise::loadPng("Textures/logo.png"), 0.0f,
		Otherwise::Vertex(quarterScreenHeight, quarterScreenHeight, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
		Otherwise::Vertex(quarterScreenHeight, threeQuartersScreenHeight, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
		Otherwise::Vertex(threeQuartersScreenHeight, quarterScreenHeight, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
		Otherwise::Vertex(threeQuartersScreenHeight, threeQuartersScreenHeight, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)));

	mMultiSprite.prepareBatches();

	int startTime = SDL_GetTicks();
	int currentTime = startTime;
	int logoLength = 5000;
	int logoTime = currentTime - startTime;

	while (logoTime < logoLength)
	{
		if (logoTime < 4000)
		{
			mMultiSprite.setSpriteAlpha(3, pow((logoTime/251), 2.0f));
			mMultiSprite.prepareBatches();
			render();
		}
		else
		{
			mMultiSprite.setSpriteAlpha(3, 255);
			mMultiSprite.prepareBatches();
			render();
		}
		currentTime = SDL_GetTicks();
		logoTime = currentTime - startTime;
	}

	glEnable(GL_DEPTH_TEST);
}

void GameLogo::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(mProgramID);

	glUniformMatrix4fv(mPerspectiveUniformID, 1, GL_FALSE, &mOrthographicMatrix[0][0]);

	mMultiSprite.renderBatches();
	
	glUseProgram(0);

	mWindow->swapBuffer();
}
