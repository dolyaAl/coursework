#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

//global variables
unsigned int gm_tiles_count = 50;
const unsigned int scr_WIDTH = 800;
const unsigned int scr_HEIGHT = 650;
float gm_tiles_step_x = static_cast<float>(scr_WIDTH)/ static_cast<float>(gm_tiles_count);
float gm_tiles_step_y = static_cast<float>(scr_HEIGHT)/ static_cast<float>(gm_tiles_count);
double ms_xpos = 0;
double ms_ypos = 0;
bool ScreenResize = false;
bool GameStarted = false;
//

class tiles
{
private:
    int count = 0;
    int live_count = 0;
public:
    unsigned int** data;
    tiles()
    {
        count = gm_tiles_count;
        data = new unsigned int* [count];
        for (int i = 0; i < count; ++i)
        {
            data[i] = new unsigned int[count] {0};
        }
    }
    tiles(const tiles& til)
    {
        count = til.count;
        live_count = til.live_count;
        data = new unsigned int* [count];
        for (int i = 0; i < count; ++i)
        {
            data[i] = new unsigned int[count] {0};
        }
        for (int i = 0; i < count; ++i)
        {
            for (int j = 0; j < count; ++j)
            {
                data[i][j] = til.data[i][j];
            }
        }
    }
    ~tiles()
    {
        for (int i = 0; i < count; ++i)
        {
            delete[] data[i];
        }
        int count = 0;
        int live_count = 0;
    }
    void SetLive(unsigned int live, unsigned int x, unsigned int y)
    {
        data[x][y] = live;
        ++live_count;
    }
    int GetLive(unsigned int x, unsigned int y)
    {
        return data[x][y];
    }
    int GetLiveCount()
    {
        return live_count;
    }
    int getCount() const
    {
        return count;
    }

    void DrawLiveTiles(GLFWwindow* window)
    {
        float vertices[12]{ 0.f };
        unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3
        };
        unsigned int VBO1, VAO1, EBO1;
        glGenVertexArrays(1, &VAO1);
        glGenBuffers(1, &VBO1);
        glGenBuffers(1, &EBO1);

        glBindVertexArray(VAO1);

        glBindBuffer(GL_ARRAY_BUFFER, VBO1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);

        glBindVertexArray(VAO1);

        for (int i = 0; i < count; ++i)
        {
            for (int j = 0; j < count; ++j)
            {
                if (data[i][j] == 1)
                {
                    vertices[9] = -1.f + (float)(i * 2) / (float)(count);
                    vertices[10] = 1.f - (float)(j * 2) / (float)(count);
                    vertices[0] = vertices[9] + (float)(2) / (float)(count);
                    vertices[1] = vertices[10];
                    vertices[6] = vertices[9];
                    vertices[7] = vertices[10] - (float)(2) / (float)(count);
                    vertices[3] = vertices[9] + (float)(2) / (float)(count);
                    vertices[4] = vertices[10] - (float)(2) / (float)(count);

                    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                }
            }
        }
    }

    tiles& operator= (const tiles& new_tiles)
    {
        if (&new_tiles != this)
        {
            for (int i = 0; i < count; ++i)
            {
                delete[] data[i];
            }
            count = new_tiles.count;
            live_count = new_tiles.live_count;
            data = new unsigned int* [count] {0};
            for (int i = 0; i < count; ++i)
            {
                data[i] = new unsigned int[count] {0};
            }
            for (int i = 0; i < count; ++i)
            {
                for (int j = 0; j < count; ++j)
                {
                    data[i][j] = new_tiles.data[i][j];
                }
            }
        }
        return *this;
    }

    void FreeMemory()
    {
        for (int i = 0; i < count; ++i)
        {
            delete[] data[i];
        }
        delete[] data;
    }

    int getLiveNeighborsCount(int x, int y) const
    {
        int counter = 0;
        if (x == 0)
        {
            if (y == 0)
            {
                counter = data[x + 1][y] + data[x][y + 1] + data[x + 1][y + 1];
            }
            else if (y == count - 1)
            {
                counter = data[x + 1][y] + data[x][y - 1] + data[x + 1][y - 1];
            }
            else
            {
                counter = data[x + 1][y] + data[x][y - 1] + data[x][y + 1] + data[x + 1][y - 1] + data[x + 1][y + 1];
            }
        }
        else if (x == count - 1)
        {
            if (y == 0)
            {
                counter = data[x - 1][y] + data[x][y + 1] + data[x - 1][y + 1];
            }
            else if (y == count - 1)
            {
                counter = data[x - 1][y] + data[x][y - 1] + data[x - 1][y - 1];
            }
            else
            {
                counter = data[x - 1][y] + data[x][y - 1] + data[x][y + 1] + data[x - 1][y - 1] + data[x - 1][y + 1];
            }
        }
        else if (y == 0)
        {
            
            counter = data[x + 1][y] + data[x - 1][y] + data[x][y + 1] + data[x - 1][y + 1] + data[x + 1][y + 1];
            
        }
        else if (y == count - 1)
        {
            counter = data[x + 1][y] + data[x - 1][y] + data[x][y - 1] + data[x - 1][y - 1] + data[x + 1][y - 1];
        }
        else
        {
            counter = data[x + 1][y] + data[x - 1][y] + data[x][y - 1] + data[x - 1][y - 1] + data[x + 1][y - 1] + data[x][y + 1] + data[x + 1][y + 1] + data[x - 1][y + 1];
        }
        return counter;
    }
};

tiles GameTiles;
tiles NewGenGameTiles;

tiles newGen(const tiles n_gm_t)
{
    tiles n_gen;
    for (int i = 0; i < n_gm_t.getCount(); ++i)
    {
        for (int j = 0; j < n_gm_t.getCount(); ++j)
        {
            if (n_gm_t.data[i][j] == 1 && n_gm_t.getLiveNeighborsCount(i, j) >= 2 && n_gm_t.getLiveNeighborsCount(i, j) <= 3)
            {
                n_gen.SetLive(1, i, j);
            }
            else if (n_gm_t.getLiveNeighborsCount(i, j) == 3)
            {
                n_gen.SetLive(1, i, j);
            }
        }
    }
    return n_gen;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void DrawVertical(GLFWwindow* window, unsigned int VBO)
{
    float vertices[] = {
         -1.f, -1.f,
         -1.f,  1.f
    };
    for (int i = 0; i < gm_tiles_count; ++i)
    {
        
        glDrawArrays(GL_LINE_STRIP, 0, 2);
        vertices[0] += (float)2 / gm_tiles_count;
        vertices[2] += (float)2 / gm_tiles_count;
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    }
}

void DrawHorizontal(GLFWwindow* window, unsigned int VBO)
{
    float vertices[] = {
         -1.f, -1.f,
         1.f,  -1.f
    };

    for (int i = 0; i < gm_tiles_count; ++i)
    {
        glDrawArrays(GL_LINE_STRIP, 0, 2);
        vertices[1] += (float)2 / gm_tiles_count;
        vertices[3] += (float)2 / gm_tiles_count;
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (yoffset < 0)
    {
        gm_tiles_count++;
    }
    else if (yoffset > 0)
    {
        if (gm_tiles_count > 2)
        {
            gm_tiles_count--;
        }
    }
    gm_tiles_step_x = static_cast<float>(scr_WIDTH) / static_cast<float>(gm_tiles_count);
    gm_tiles_step_y = static_cast<float>(scr_HEIGHT) / static_cast<float>(gm_tiles_count);
    tiles n_tiles;
    n_tiles = GameTiles;
    GameTiles = n_tiles;
}

void processInput(GLFWwindow* window, tiles* gm_t)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        ScreenResize = !ScreenResize;
    }
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
    {
        GameStarted = !GameStarted;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        glfwGetCursorPos(window, &ms_xpos, &ms_ypos);
        gm_t->SetLive(1, (unsigned int)ms_xpos / gm_tiles_step_x, (unsigned int)ms_ypos / gm_tiles_step_y);
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        glfwGetCursorPos(window, &ms_xpos, &ms_ypos);
        gm_t->SetLive(0, (unsigned int)ms_xpos / gm_tiles_step_x, (unsigned int)ms_ypos / gm_tiles_step_y);
    }
    if (!ScreenResize)
    {
        glfwSetScrollCallback(window, nullptr);
    }
    else if(ScreenResize && gm_t->GetLiveCount() == 0 && !GameStarted)
    {
        glfwSetScrollCallback(window, scroll_callback);
    }
}

void GameRenderer(GLFWwindow* window, unsigned int VBO)
{
    DrawVertical(window, VBO);
    DrawHorizontal(window, VBO);

    GameTiles.DrawLiveTiles(window);
    if (GameStarted)
    {
        NewGenGameTiles = newGen(GameTiles);
        if (glfwGetTime() >= 0.3f)
        {
            GameTiles = NewGenGameTiles;
            glfwSetTime(0);
        }
    }
}

//������ �������
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.f, 0.f, 0.f, 1.0f);\n"
"}\n\0";
//

int main(int argc, char* argv[])
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
    {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(scr_WIDTH, scr_HEIGHT, "GameOfLife", nullptr, nullptr);
    if (!window)
    {
        std::cout << "Can't create Window" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if(!gladLoadGL())
	{
		std::cout << "Can't load GLAD!" << std::endl;
		return -1;
	}
	
	std::cout << "OpenGL " << GLVersion.major << "." << GLVersion.minor << std::endl;
	
    glClearColor(0.7f, 0.7f, 0.7f, 1);
	
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        processInput(window, &GameTiles);

        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); 

        GameRenderer(window, VBO);
       
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}