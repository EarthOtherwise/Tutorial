#pragma once
#include "glm/glm.hpp"
#include "SDL/SDL_events.h"
#include "CEGUI/CEGUI.h"
#include "CEGUI/RendererModules/OpenGL/GL3Renderer.h"
#include "MessagingSystem.h"

namespace Otherwise
{
	class GUI
	{
	public:
		GUI();
		~GUI();
		void init(const std::string &resourceDirectory, CorrespondentManager *corrManager);
		void update();
		void destory();
		void render();

		void setMouseCursor(const std::string &imageFile);
		void showMouseCursor();
		void hideMouseCursor();
		
		void keyUpFunc(SDL_Event &evnt);
		void keyDownFunc(SDL_Event &evnt);
		void mouseMotionFunc(SDL_Event &evnt);
		void decodeInputText(SDL_Event &textEvent);
		void mouseButtonDownFunc(SDL_Event &evnt);
		void mouseButtonUPFunc(SDL_Event &evnt);
		

		static CEGUI::OpenGL3Renderer* getRenderer() {
			return mGUIRenderer;
		}
		const CEGUI::GUIContext* getContext() {
			return mContext;
		}

		void loadScheme(const std::string &schemeFile);
		void setFont(const std::string &fontFile);

		CEGUI::Window* createWidget(const std::string &scheme, const glm::vec4 &percRect, const glm::vec4 &pixRect, const std::string & name);
		static void setWidgetRect(CEGUI::Window* widget, const glm::vec4 &percRect, const glm::vec4 &pixRect);

	private:
		static CEGUI::OpenGL3Renderer* mGUIRenderer;
		CEGUI::GUIContext* mContext = nullptr;
		CEGUI::Window* mRoot = nullptr;
		unsigned int mPreviousTime = 0;
		Correspondent mFromInput;
	};

}