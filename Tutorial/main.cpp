#include "InputHandler.h"
#include "InitFileReadWrite.h"
#include "OSInterface.h"
#include "Cameras.h"
#include "SpriteFont.h"
#include "SpatialSceneGraphOct.h"
#include "GameLogo.h"
#include "GUI.h"
#include "MainMenu.h"
#include "OAudio.h"
#include "MessagingSystem.h"
#include "OCollision.h"
#include "ErrHandler.h"
#include "Particles.h"

#include "GraphicsInterface.h"

/*temporarily include iostream*/
#include <iostream>

//#define GLM_ENABLE_EXPERIMENTAL
//#include "glm/gtc/quaternion.hpp"
//#include "glm/gtx/quaternion.hpp"
//#include "glm/gtx/euler_angles.hpp"

int main(int argc, char** argv)
{
	Otherwise::OSInterface osInterface;
	osInterface.init();
	osInterface.createWindow("Window", 512, 512, 0);

	Otherwise::initGraphicsInterface();

	Otherwise::CorrespondentManager manager;
	manager.init();

	Otherwise::Correspondent quitCorrespondent;
	quitCorrespondent.init(&manager, (std::string)"QuitGame");

	Otherwise::ONetwork network(&manager);
	std::string ip;
	ip = "0";
	if (ip == "0")
	{
		network.initHosting(1598);
	}
	else
	{
		network.connectToHost(ip.c_str(), 1598);
	}

	/*GameLogo gameLogo;
	gameLogo.init("Logo.vert", "Logo.frag", 512, 512, Otherwise::ovec2(0.0f, 0.0f), 1.0f, &osInterface, &graphics);
	gameLogo.logoUpdateRenderLoop();*/

	Otherwise::GUI gui;
	gui.init("GUI", &manager, &osInterface);

	Otherwise::InputHandler newInput;
	newInput.init(&manager, &osInterface);

	/*MainMenu mainMenu;
	mainMenu.init(&graphics, "Logo.vert", "Logo.frag", 512, 512, Otherwise::ovec2(0.0f, 0.0f), 1.0f, &osInterface, &gui, &newInput);
	mainMenu.mainMenuLoop();*/

	Otherwise::relativeMouseMotionToggle();

	Otherwise::OAudio audio;
	audio.init((std::string)"SoundMap1.txt", &manager);

	bool doQuit = false;
	
	Otherwise::Shader simple_shader;
	simple_shader.load("SimpleVert.vert", "SimpleFrag.frag");

	std::string perspective = "Perspective";
	std::string camera = "CameraMatrix";
	std::string model = "ModelMatrix";
	std::string model_camera = "ModelCameraMatrix";
	std::string light_pos = "LightPosition";
	std::string light_colour = "LightColour";
	std::string light_intensity = "LightIntensity";
	std::string scale = "Scale";

	Otherwise::Shader texture_shader;
	texture_shader.load("TextureVert.vert", "TextureFrag.frag");

	Otherwise::ShaderUniform texture_perspective;
	texture_perspective.get(texture_shader, perspective);
	Otherwise::ShaderUniform texture_camera;
	texture_camera.get(texture_shader, camera);
	Otherwise::ShaderUniform texture_model;
	texture_model.get(texture_shader, model);
	Otherwise::ShaderUniform texture_model_camera;
	texture_model_camera.get(texture_shader, model_camera);
	Otherwise::ShaderUniform texture_light_position;
	texture_light_position.get(texture_shader, light_pos);
	Otherwise::ShaderUniform texture_light_colour;
	texture_light_colour.get(texture_shader, light_colour);
	Otherwise::ShaderUniform texture_light_intensity;
	texture_light_intensity.get(texture_shader, light_intensity);

	Otherwise::Shader mesh_shader;
	mesh_shader.load("Mesh.vert", "Mesh.frag");

	Otherwise::ShaderUniform mesh_perspective;
	mesh_perspective.get(mesh_shader, perspective);
	Otherwise::ShaderUniform mesh_camera;
	mesh_camera.get(mesh_shader, camera);
	Otherwise::ShaderUniform mesh_model;
	mesh_model.get(mesh_shader, model);
	Otherwise::ShaderUniform mesh_model_camera;
	mesh_model_camera.get(mesh_shader, model_camera);
	Otherwise::ShaderUniform mesh_light_position;
	mesh_light_position.get(mesh_shader, light_pos);
	Otherwise::ShaderUniform mesh_light_colour;
	mesh_light_colour.get(mesh_shader, light_colour);
	Otherwise::ShaderUniform mesh_light_intensity;
	mesh_light_intensity.get(mesh_shader, light_intensity);


	Otherwise::Shader billboard_shader;
	billboard_shader.load("Billboard.vert", "Billboard.frag");

	Otherwise::ShaderUniform billboard_perspective;
	billboard_perspective.get(billboard_shader, perspective);
	Otherwise::ShaderUniform billboard_camera;
	billboard_camera.get(billboard_shader, camera);
	Otherwise::ShaderUniform billboard_model;
	billboard_model.get(billboard_shader, model);
	Otherwise::ShaderUniform billboard_scale;
	billboard_scale.get(billboard_shader, scale);

	Otherwise::Shader skybox_shader;
	skybox_shader.load("Skybox.vert", "Skybox.frag");

	Otherwise::ShaderUniform skybox_perspective;
	skybox_perspective.get(skybox_shader, perspective);
	Otherwise::ShaderUniform skybox_camera;
	skybox_camera.get(skybox_shader, camera);
	Otherwise::ShaderUniform skybox_model;
	skybox_model.get(skybox_shader, model);

	Otherwise::Texture2D texture_2d;
	texture_2d.load("Textures/rainbow.png");
	Otherwise::IndexedMesh plane;
	plane.load("Meshes/plane.obj");
	Otherwise::VertexArray mesh_rend;
	mesh_rend.init();
	mesh_rend.update(plane);

	Otherwise::MultiSprite multiSprite3;
	multiSprite3.init();

	Otherwise::SpriteFont spriteFont("Fonts/chintzy.ttf", 64);
	char buffer[256];
	sprintf_s(buffer, "Test String");
	spriteFont.draw(multiSprite3, buffer, glm::vec2(0.0f, 0.0f), glm::vec2(0.01f), 0.0f, Otherwise::ColourRGBA8(255, 255, 255, 255));

	Otherwise::Camera2D camera2D (512, 512, Otherwise::ovec2(0.0f, 0.0f), 1.0f);
	Otherwise::omat4 ortho = camera2D.getMatrix();

	Otherwise::Camera3D camera3D(512, 512, Otherwise::ovec3(0.0f, 0.0f, 5.0f),
		45.0f, 1.0f, 100.0f, Otherwise::ovec3(0.0f, 1.0f, 0.0f), &manager, 1.57f, 0.0f);

	Otherwise::Camera3D cameraCheck(512, 512, Otherwise::ovec3(0.0f, 0.0f, 0.0f),
		45.0f, 1.0f, 20.0f, Otherwise::ovec3(0.0f, 1.0f, 0.0f), &manager, -1.57f, 0.0f);

	gui.mChatBox.setVisible(true);

	std::vector<Otherwise::omat4> meshModelMatrices;

	for (float i = -10.0f; i < 10.0f; i += 5.0f)
	{
		for (float j = -10.0f; j < 10.0f; j += 5.0f)
		{
			for (float k = -10.0f; k < 10.0f; k += 5.0f)
			{
				Otherwise::omat4 tempMatrix(1.0f);// = Otherwise::oscale(Otherwise::omat4(1.0f), Otherwise::ovec3(k, k, k));
				//tempMatrix *= Otherwise::oeulerAngleXYZ(i, j, k);
				tempMatrix *= Otherwise::translate_matrix(Otherwise::omat4(1.0f), Otherwise::ovec3(i, j, k));
				meshModelMatrices.push_back(tempMatrix);
			}
		}
	}

	Otherwise::ParticleEmitterCone pEmit;
	pEmit.init(Otherwise::ovec3(1.0f), 0.001f, Otherwise::ovec3(0.0f, 0.0f, 0.001f), 0.0005f,
		1000, 100000);


	//"Meshes/skybox.obj"
	Otherwise::TextureCubeMap skybox;
	std::string cube_map_file_paths[6];
	cube_map_file_paths[0] = "Textures/xpos.png";
	cube_map_file_paths[1] = "Textures/xneg.png";
	cube_map_file_paths[2] = "Textures/ypos.png";
	cube_map_file_paths[3] = "Textures/yneg.png";
	cube_map_file_paths[4] = "Textures/zpos.png";
	cube_map_file_paths[5] = "Textures/zneg.png";
	skybox.load(cube_map_file_paths);
	Otherwise::IndexedMesh box;
	box.load("Meshes/skybox.obj");
	Otherwise::VertexArray meshRenderer;
	meshRenderer.init();
	meshRenderer.update(box);

	unsigned int lastTime = SDL_GetTicks();

	float light_intensity_float = 5.0f;
	float scale_of = 0.7f;

	while (!quitCorrespondent.getMessage())
	{
		unsigned int deltaTime = SDL_GetTicks() - lastTime;
		lastTime = SDL_GetTicks();

		network.update();
		gui.update();
		audio.update();
		camera3D.update();

		Otherwise::clearBackScreenBuffer();

		// triangle.textureDraw();
		//multiSprite3.renderBatches();
		//multiSprite.renderBatches();

		texture_shader.use();

		texture_perspective.fill(camera3D.getPerspectiveMatrix());
		texture_camera.fill(camera3D.getCameraMatrix());
		texture_model.fill(camera3D.getModelMatrix());
		texture_model_camera.fill(camera3D.getCameraMatrix());
		texture_light_position.fill(Otherwise::ovec3(2.0f, 0.5f, -1.5f));
		texture_light_colour.fill(Otherwise::ovec3(1.0f, 1.0f, 1.0f));
		texture_light_intensity.fill(light_intensity_float);

		mesh_shader.use();

		mesh_perspective.fill(camera3D.getPerspectiveMatrix());
		mesh_camera.fill(camera3D.getCameraMatrix());
		mesh_model_camera.fill(camera3D.getCameraMatrix());
		mesh_light_position.fill(Otherwise::ovec3(2.0f, 0.5f, -1.5f));
		mesh_light_colour.fill(Otherwise::ovec3(1.0f, 1.0f, 1.0f));
		mesh_light_intensity.fill(light_intensity_float);


		billboard_shader.use();

		billboard_perspective.fill(camera3D.getProjectionMatrix());
		billboard_camera.fill(camera3D.getCameraMatrix());
		billboard_scale.fill(scale_of);

		pEmit.loopParticles(deltaTime);
		std::vector<Otherwise::ovec3> vector = pEmit.getParticlePositions();
		std::vector<Otherwise::omat4> particleMatrices;

		for (unsigned int i = 0; i < vector.size(); i++)
		{
			particleMatrices.push_back(Otherwise::translate_matrix(Otherwise::omat4(1.0f), vector[i]));
		}

		texture_2d.use();
		for (unsigned int i = 0; i < particleMatrices.size(); i++)
		{
			billboard_model.fill(particleMatrices[i]);
			mesh_rend.render();
		}

		skybox_shader.use();

		skybox_perspective.fill(camera3D.getProjectionMatrix());
		Otherwise::omat4 modifiedCameraMatrix = camera3D.getCameraMatrix();
		modifiedCameraMatrix.matrix[3][0] = 0.0f;
		modifiedCameraMatrix.matrix[3][1] = 0.0f;
		modifiedCameraMatrix.matrix[3][2] = 0.0f;
		skybox_camera.fill(modifiedCameraMatrix);
		skybox_model.fill(Otherwise::omat4(1.0f));

		Otherwise::skyboxMode(true);
		skybox.use();
		meshRenderer.render();
		Otherwise::skyboxMode(false);

		newInput.inputQueue();

		gui.render();

		osInterface.swapBuffer();
	}

	return 0;
}