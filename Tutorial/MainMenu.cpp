#include "MainMenu.h"
#include "SpriteFont.h"
#include"imageLoad.h"
#include "simpleShader.h"

MainMenu::MainMenu()
{
}


MainMenu::~MainMenu()
{
}

void MainMenu::init(Otherwise::GraphicsResourceManager * GRM, std::string vertShader, std::string fragShader,
	int screenWidth, int screenHeight, glm::vec2 cameraPosition, float cameraZoom,
	Otherwise::Window* windowptr, Otherwise::GUI* gui, Otherwise::InputHandler* input)
{
	mGRM = GRM;
	mInput = input;
	mProgramID = Otherwise::compileLinkSimpleShaders(vertShader, fragShader);
	mPerspectiveUniformID = glGetUniformLocation(mProgramID, "Perspective");
	mCamera.init(screenWidth, screenHeight, cameraPosition, cameraZoom);
	mOrthographicMatrix = mCamera.getMatrix();
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;
	mWindow = windowptr;
	mGUI = gui;
}

void MainMenu::mainMenuLoop()
{
	mGRM->mMultiSprite2D.removeBackSprites(3);

	GLuint starscapeTextureID = Otherwise::loadPng("Textures/rock.png");

	float fifthScreenWidth = mScreenWidth / 5.0f;
	float fifthScreenHeight = mScreenHeight / 5.0f;
	float twoFifthsScreenWidth = fifthScreenWidth * 2.0f;
	float twoFifthsScreenHeight = fifthScreenHeight * 2.0f;

	mGRM->mMultiSprite2D.addSprite(starscapeTextureID, 0.0f,
		Otherwise::Vertex(fifthScreenWidth, fifthScreenHeight, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
		Otherwise::Vertex(fifthScreenWidth, twoFifthsScreenHeight, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
		Otherwise::Vertex(twoFifthsScreenWidth, fifthScreenHeight, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
		Otherwise::Vertex(twoFifthsScreenWidth, twoFifthsScreenHeight, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)));

	mGRM->mMultiSprite2D.addSprite(starscapeTextureID, 0.0f,
		Otherwise::Vertex(mScreenWidth - twoFifthsScreenWidth, fifthScreenHeight, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
		Otherwise::Vertex(mScreenWidth - twoFifthsScreenWidth, twoFifthsScreenHeight, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
		Otherwise::Vertex(mScreenWidth - fifthScreenWidth, fifthScreenHeight, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)),
		Otherwise::Vertex(mScreenWidth - fifthScreenWidth, twoFifthsScreenHeight, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, Otherwise::ColourRGBA8(255, 255, 255, 255)));

	Otherwise::SpriteFont spriteFont("Fonts/chintzy.ttf", 64);
	char buffer[256];
	sprintf_s(buffer, "Bad Asteroids Clone");
	spriteFont.draw(mGRM->mMultiSprite2D, buffer, glm::vec2(mScreenWidth / 2.0f, mScreenHeight - fifthScreenHeight), glm::vec2(0.6f), 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255), Otherwise::Justification::MIDDLE);

	mGUI->loadScheme("TaharezLook.scheme");
	mGUI->setFont("DejaVuSans-10");

	CEGUI::PushButton* startButton = (CEGUI::PushButton*)mGUI->createWidget("TaharezLook/Button", glm::vec4(0.4f, 0.3f, 0.2f, 0.1f), glm::vec4(0.0f), "StartButton");
	startButton->setText("Start");
	startButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenu::beginGame, this));

	CEGUI::PushButton* exitButton = (CEGUI::PushButton*)mGUI->createWidget("TaharezLook/Button", glm::vec4(0.4f, 0.5f, 0.2f, 0.1f), glm::vec4(0.0f), "ExitButton");
	exitButton->setText("Exit");
	exitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenu::exitGUI, this));

	CEGUI::Combobox* textBox = static_cast<CEGUI::Combobox*>(mGUI->createWidget("TaharezLook/Combobox", glm::vec4(0.4f, 0.7f, 0.2f, 0.1f), glm::vec4(0.0f), "TypeBox"));

	mGUI->setMouseCursor("TaharezLook/MouseArrow");
	mGUI->showMouseCursor();
	SDL_ShowCursor(false);

	glDisable(GL_DEPTH_TEST);

	mGRM->mMultiSprite2D.prepareBatches();

	while (!mStartGame)
	{
		mInput->inputQueue();
		mGUI->update();

		render();
	}

	glEnable(GL_DEPTH_TEST);

}

void MainMenu::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(mProgramID);

	glUniformMatrix4fv(mPerspectiveUniformID, 1, GL_FALSE, &mOrthographicMatrix[0][0]);

	mGRM->mMultiSprite2D.renderBatches();

	mGUI->render();

	glUseProgram(0);

	mWindow->swapBuffer();
}

bool MainMenu::exitGUI(const CEGUI::EventArgs & e)
{
	SDL_Quit();
	exit(0);
	return false;
}

bool MainMenu::beginGame(const CEGUI::EventArgs &e)
{
	mStartGame = true;
	return true;
}
