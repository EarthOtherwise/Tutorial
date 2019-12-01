#include "GraphicsInterface.h"
#include "ErrHandler.h"
#include "PicoPNG.h"
#include "OAssImp.h"

namespace Otherwise
{
void initGraphicsInterface()
{
	glewInit();

	glClearDepth(1.0);

	glClearColor(0.5f, 0.0f, 0.5f, 0.5f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
}

void depthTest(bool on)
{
	if (on)
	{
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}
}

void skyboxMode(bool on)
{
	if (on)
	{
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);
	}
	else
	{
		glCullFace(GL_BACK);
		glDepthFunc(GL_LESS);
	}
}

void clearBackScreenBuffer()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Texture2D::load(const std::string& file_path)
{
	unsigned long width, height;

	std::ifstream file(file_path, std::ios::binary);
	if (file.fail())
	{
		throwError("PNG File", "PNG " + file_path + " file could not be opened.");
	}

	file.seekg(0, std::ios::end);

	unsigned int fileSize = file.tellg();

	file.seekg(0, std::ios::beg);

	fileSize -= file.tellg();

	std::vector<char> imageData;

	imageData.resize(fileSize);

	file.read(&(imageData[0]), fileSize);

	std::vector<unsigned char> temp_image_data;
	std::vector<unsigned char> pixel_data;

	temp_image_data.resize(fileSize);
	for (size_t i = 0; i < fileSize; i++)
	{
		temp_image_data[i] = imageData[i];
	}

	const int err_code = decodePNG(pixel_data, width, height, &(temp_image_data[0]), temp_image_data.size());

	if (err_code != 0)
	{
		throwError("PNg Decoder Error", "PNG decoder threw error: " + std::to_string(err_code));
	}

	glGenTextures(1, &mTexture);

	glBindTexture(GL_TEXTURE_2D, mTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(pixel_data[0]));

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture2D::use() const
{
	glBindTexture(GL_TEXTURE_2D, mTexture);
}

void TextureCubeMap::load(std::string m_file_names[6])
{
	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture);

	for (unsigned int i = 0; i < 6; i++)
	{
		unsigned long width = 0;
		unsigned long height = 0;

		std::ifstream file(m_file_names[i], std::ios::binary);
		if (file.fail())
		{
			throwError("PNG File", "PNG " + m_file_names[i] + " file could not be opened.");
		}

		file.seekg(0, std::ios::end);

		unsigned int fileSize = file.tellg();

		file.seekg(0, std::ios::beg);

		fileSize -= file.tellg();

		std::vector<char> imageData;

		imageData.resize(fileSize);

		file.read(&(imageData[0]), fileSize);

		std::vector<unsigned char> temp_image_data;
		std::vector<unsigned char> pixel_data;

		temp_image_data.resize(fileSize);
		for (size_t i = 0; i < fileSize; i++)
		{
			temp_image_data[i] = imageData[i];
		}

		const int err_code = decodePNG(pixel_data, width, height, &(temp_image_data[0]), temp_image_data.size());

		if (err_code != 0)
		{
			throwError("PNG Decoder Error", "PNG decoder threw error: " + std::to_string(err_code));
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(pixel_data[0]));

	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void TextureCubeMap::use()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture);
}

void IndexedMesh::load(char* file_path)
{
	loadAssimp(file_path, indices, vertices, uvs, normals);
}

void VertexArray::init()
{
	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);

	glGenBuffers(1, &mVertexBuffer);
	glGenBuffers(1, &mUvBuffer);
	glGenBuffers(1, &mNormalBuffer);
	glGenBuffers(1, &mIndexBuffer);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, mUvBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, mNormalBuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
}

void VertexArray::update(const IndexedMesh& object)
{
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, object.vertices.size() * sizeof(ovec3),
		&object.vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mUvBuffer);
	glBufferData(GL_ARRAY_BUFFER, object.uvs.size() * sizeof(ovec2),
		&object.uvs[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mNormalBuffer);
	glBufferData(GL_ARRAY_BUFFER, object.normals.size() * sizeof(ovec3),
		&object.normals[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		object.indices.size() * sizeof(unsigned short), &object.indices[0],
		GL_STATIC_DRAW); 

	index_count = object.indices.size();
}

void VertexArray::render()
{
	glBindVertexArray(mVao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

	glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_SHORT,
		(void *)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Shader::load(const std::string& vertShaderFilePath, const std::string& fragShaderFilePath)
	{
	GLuint vertShaderID = glCreateShader(GL_VERTEX_SHADER);

	if (vertShaderID == 0)
	{
		throwConsoleError("vertex"/*, "OpenGL couldn't create the vertex shader."*/);
	}

	std::ifstream vertFile(vertShaderFilePath);

	if (!vertFile.is_open())
	{
		throwConsoleError("vertexFile"/*, "Couldn't open the vertex file, check the file name."*/);
	}

	std::string fileContents = "";
	std::string tempLine = "";

	while (std::getline(vertFile, tempLine))
	{
		fileContents += tempLine + "\n";
	}

	vertFile.close();

	const char* contentsPtr = fileContents.c_str();

	glShaderSource(vertShaderID, 1, &contentsPtr, nullptr);

	glCompileShader(vertShaderID);

	GLint completed = 0;
	glGetShaderiv(vertShaderID, GL_COMPILE_STATUS, &completed);

	if (completed == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(vertShaderID, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(vertShaderID, maxLength, &maxLength, &errorLog[0]);

		std::printf("%s \n", &errorLog[0]);

		glDeleteShader(vertShaderID);

		throwConsoleError("VertCompile"/*, "Vertex Shader failed to compile."*/);
	}

	GLuint fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	if (fragShaderID == 0)
	{
		throwConsoleError("fragment"/*, "OpenGL couldn't create the fragment shader."*/);
	}

	std::ifstream fragFile(fragShaderFilePath);

	if (!fragFile.is_open())
	{
		throwConsoleError("fragmentFile"/*, "Couldn't open the fragment file, check the file name."*/);
	}

	fileContents = "";
	tempLine = "";

	while (std::getline(fragFile, tempLine))
	{
		fileContents += tempLine + "\n";
	}

	fragFile.close();

	contentsPtr = fileContents.c_str();

	glShaderSource(fragShaderID, 1, &contentsPtr, nullptr);

	glCompileShader(fragShaderID);

	completed = 0;
	glGetShaderiv(fragShaderID, GL_COMPILE_STATUS, &completed);

	if (completed == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(fragShaderID, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(fragShaderID, maxLength, &maxLength, &errorLog[0]);

		std::printf("%s \n", &errorLog[0]);

		glDeleteShader(fragShaderID);

		throwConsoleError("FragCompile"/*, "Fragment Shader failed to compile."*/);
	}

	mShaderID = glCreateProgram();

	glAttachShader(mShaderID, vertShaderID);
	glAttachShader(mShaderID, fragShaderID);

	glLinkProgram(mShaderID);

	completed = 0;
	glGetProgramiv(mShaderID, GL_LINK_STATUS, (int *)&completed);

	if (completed == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(mShaderID, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<char> errorLog(maxLength);
		glGetProgramInfoLog(mShaderID, maxLength, &maxLength, &errorLog[0]);

		std::printf("%s \n", &errorLog[0]);

		glDeleteShader(vertShaderID);
		glDeleteShader(fragShaderID);

		throwConsoleError("ShaderLinker"/*, "OpenGL could not link our shaders."*/);
	}

	glDetachShader(mShaderID, vertShaderID);
	glDetachShader(mShaderID, fragShaderID);
	glDeleteShader(vertShaderID);
	glDeleteShader(fragShaderID);
}

void Shader::use() const
{
	glUseProgram(mShaderID);
}

void ShaderUniform::get(Shader& shader, std::string & name)
{
	mShaderUniformID = glGetUniformLocation(shader.mShaderID, name.c_str());
}

void ShaderUniform::fill(omat4 & fill)
{
	glUniformMatrix4fv(mShaderUniformID, 1, GL_FALSE, &fill.matrix[0][0]);
}

void ShaderUniform::fill(ovec3 & fill)
{
	glUniform3f(mShaderUniformID, fill.vect.x, fill.vect.y, fill.vect.z);
}

void ShaderUniform::fill(float & fill)
{
	glUniform1f(mShaderUniformID, fill);
}

}
