#include "GameLogo.h"
#include "SDL/sdl.h"


GameLogo::GameLogo()
{
}


GameLogo::~GameLogo()
{
}

void GameLogo::init(std::string vertShader, std::string fragShader, int screenWidth, int screenHeight, glm::vec2 cameraPosition, float cameraZoom, Otherwise::OSInterface* windowptr, Otherwise::GraphicsResourceManager* graphics)
{
	mProgramID = Otherwise::compileLinkSimpleShaders(vertShader, fragShader);
	mPerspectiveUniformID = glGetUniformLocation(mProgramID, "Perspective");
	mCamera.init(screenWidth, screenHeight, cameraPosition, cameraZoom);
	mOrthographicMatrix = mCamera.getMatrix();
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;
	mWindow = windowptr;
	mGraphics = graphics;
	mGraphics->mMultiSprite2D.init();
}

void GameLogo::logoUpdateRenderLoop()
{
	glDisable(GL_DEPTH_TEST);
	
	GLuint starscapeTextureID = Otherwise::loadPng("Textures/starscape.png");

	float quarterScreenHeight = mScreenHeight / 4.0f;
	float threeQuartersScreenHeight = mScreenHeight - quarterScreenHeight;

	mGraphics->mMultiSprite2D.addSprite(starscapeTextureID, 0.0f,
		Otherwise::Vertex(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
		Otherwise::Vertex(0.0f, mScreenHeight, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
		Otherwise::Vertex(mScreenWidth, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
		Otherwise::Vertex(mScreenWidth, mScreenHeight, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)));

	mGraphics->mMultiSprite2D.addSprite(starscapeTextureID, 0.0f,
		Otherwise::Vertex(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(0, 0, 0, 255)),
		Otherwise::Vertex(0.0f, quarterScreenHeight, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(0, 0, 0, 255)),
		Otherwise::Vertex(mScreenWidth, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(0, 0, 0, 255)),
		Otherwise::Vertex(mScreenWidth, quarterScreenHeight, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(0, 0, 0, 255)));

	mGraphics->mMultiSprite2D.addSprite(starscapeTextureID, 0.0f,
		Otherwise::Vertex(0.0f, threeQuartersScreenHeight, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(0, 0, 0, 255)),
		Otherwise::Vertex(0.0f, mScreenHeight, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(0, 0, 0, 255)),
		Otherwise::Vertex(mScreenWidth, threeQuartersScreenHeight, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(0, 0, 0, 255)),
		Otherwise::Vertex(mScreenWidth, mScreenHeight, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(0, 0, 0, 255)));

	mGraphics->mMultiSprite2D.addSprite(Otherwise::loadPng("Textures/logo.png"), 0.0f,
		Otherwise::Vertex(quarterScreenHeight, quarterScreenHeight, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
		Otherwise::Vertex(quarterScreenHeight, threeQuartersScreenHeight, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
		Otherwise::Vertex(threeQuartersScreenHeight, quarterScreenHeight, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
		Otherwise::Vertex(threeQuartersScreenHeight, threeQuartersScreenHeight, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)));

	mGraphics->mMultiSprite2D.prepareBatches();

	int startTime = SDL_GetTicks();
	int currentTime = startTime;
	int logoLength = 5;
	int logoTime = currentTime - startTime;

	while (logoTime < logoLength)
	{
		if (logoTime < 4000)
		{
			mGraphics->mMultiSprite2D.setSpriteAlpha(3, pow((logoTime/251), 2.0f));
			mGraphics->mMultiSprite2D.prepareBatches();
			render();
		}
		else
		{
			mGraphics->mMultiSprite2D.setSpriteAlpha(3, 255);
			mGraphics->mMultiSprite2D.prepareBatches();
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

	mGraphics->mMultiSprite2D.renderBatches();
	
	glUseProgram(0);

	mWindow->swapBuffer();
}
