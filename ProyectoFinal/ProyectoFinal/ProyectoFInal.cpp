/*
* Proyecto Final.cpp
* 
* Nombre del Proyecto: Entorno de los picapiedra
* 
* Autor: Hernández Hernández Alonso de Jesús
* 
* Materia: Computación Grafica Interacción Humano Computadora
*/

/*Sección de declaración de bibliotecas y dependecias a usar para su procesamiento*/
#include <iostream>
#include <cmath>
#include <Windows.h>
#include <mmsystem.h>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"

/*Declaración de las funciones a utilizar*/
// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void animacion();
void animacion2();
void animacion3();
void Sonido();
void Sonido2();
void Sonido3();


// Dimensiones de la ventana al crear
const GLuint WIDTH = 1920, HEIGHT = 1080;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Declaración de modelo de camara y su posición
Camera  camera(glm::vec3(4.0f, 1.0f, 0.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;

//Arreglo para teclas
bool keys[1024];

//Variable de uso de movimiento de ratón (periferico)
bool firstMouse = true;


// Atributos del uso de ilumincación, posición inicial
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);

//Declaración de posición inical de objeto a mover con Key Frames
glm::vec3 PosIni(0.0f, 0.0f, 0.0f);
glm::vec3 PosIni2(0.0f, 0.0f, 0.0f);


/*Declaración de variables, para activar animaciones */
bool active;
float anim1=0.0f;
float tiempo;
int bandera;

// Posición inicial del pointlight y cubo
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,3.0f, 0.0f),
	glm::vec3(1.5f, 0.82f, 1.15f)

};

//Variables de movimiento y rotación para animación 3
float movKitX = 0.0;
float movKitZ = 0.0;
float rotKit = 0.0;

//Bandera a usar la activación de recorrido de movimiento 
bool circuito = false;

/*Banderas para activar movimiento y recorrido por secciones*/
bool recorrido1 = true;
bool recorrido2 = false;
bool recorrido3 = false;
bool recorrido4 = false;
bool recorrido5 = false;
bool recorrido6 = false;
bool recorrido7 = false;
bool recorrido8 = false;
bool recorrido9 = false;

// Declaración de medida de tiempo entre frames usado y el ultimo frame
GLfloat deltaTime = 0.0f;	// Diferencia de tiempo entre frames
GLfloat lastFrame = 0.0f;  	// Tiempo del Ultimo Frame

// Declaración de los key frames, primer key frame
float posX = PosIni.x, posY = PosIni.y, posZ = PosIni.z, rotCuerpo = 0;

// Declaración de los key frames, segundo key frame
float posX2 = PosIni.x, posY2 = PosIni.y, posZ2 = PosIni2.z, rotBrazoDer = 0;

//Definición de memoria de los key frames, valor 4
#define MAX_FRAMES 4

//Variables a usar de numero maximo de pasos y paso actual, uso para key frame
int i_max_steps = 190;
int i_curr_steps = 0;

//Variables a usar de numero maximo de pasos y paso actual, uso para key frame 2
int i_max_steps2 = 190;
int i_curr_steps2 = 0;

//Definición Estructural del Frame, para acceso a sus atributos
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float incX;		//Variable para IncrementoX
	float incY;		//Variable para IncrementoY
	float incZ;		//Variable para IncrementoZ
	float rotCuerpo; //Variable para Rotación de Cuerpo
	float rotInc; //Variable para guardar rotación 

}FRAME;

//Definición Estructural del Frame 2, para acceso a sus atributos
typedef struct _frame2
{
	//Variables para GUARDAR Key Frames
	float posX2;		//Variable para PosicionX
	float posY2;		//Variable para PosicionY
	float posZ2;		//Variable para PosicionZ
	float incX2;		//Variable para IncrementoX
	float incY2;		//Variable para IncrementoY
	float incZ2;		//Variable para IncrementoZ
	float rotBrazoDer; //Variable para guardar rotación de brazos
	float rotInc2;	//Variable para guardar incremento de brazos

}FRAME2;

/*
* Declaración de arreglo Key Frame con tamaño 4
* Variable para introducir los datos de posición e interpolación al frame
* Bandera de activación de animación
* Indice de arreglo para recorrido
*/
FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

/*
* Declaración de arreglo Key Frame 2 con tamaño 4
* Variable para introducir los datos de posición e interpolación al frame
* Bandera de activación de animación
* Indice de arreglo para recorrido
*/
FRAME2 KeyFrame2[MAX_FRAMES];
int FrameIndex2 = 0;			//introducir datos
bool play2 = false;
int playIndex2 = 0;

//Areglo de los vertices para dibujo de cubo de iluminación
float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};


//Declaración de model de luz
glm::vec3 Light1 = glm::vec3(0);

//Función para salvar los frames
void saveFrame(void)
{
	//Imprime el numero de frame almacenado
	printf("frameindex %d\n", FrameIndex);

	//Arreglo de estructura para almacenar la posición en el frame en el elemento de posición
	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].rotCuerpo= rotCuerpo;

	FrameIndex++;
}

//Función para salvar los frames
void saveFrame2(void)
{
	//Arreglo de estructura para almacenar la posición en el frame en el elemento de posición
	printf("frameindex %d\n", FrameIndex2);

	KeyFrame2[FrameIndex2].posX2 = posX2;
	KeyFrame2[FrameIndex2].posY2 = posY2;
	KeyFrame2[FrameIndex2].posZ2 = posZ2;

	KeyFrame2[FrameIndex2].rotBrazoDer = rotBrazoDer;


	FrameIndex2++;
}

//Función para reiniciar posición inicial de objeto de frame 1
void resetElements(void)
{
	//Variable regresando la posición inicial almacenada en el frame arreglo de posición 0
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotCuerpo = KeyFrame[0].rotCuerpo;
}

//Función para reiniciar posición inicial de objeto
void resetElements2(void)
{
	//Variable regresando la posición inicial almacenada en el frame arreglo de posición 0
	posX2 = KeyFrame2[0].posX2;
	posY2 = KeyFrame2[0].posY2;
	posZ2 = KeyFrame2[0].posZ2;

	rotBrazoDer = KeyFrame2[0].rotBrazoDer;

}

//Función de calculo de interpolación para realizar la animación y su velocidad de incremento, siendo lineal.
void interpolation(void)
{
	
	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps; //Incremento en X
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps; //Incremento en Y
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps; //Incremento en Z

	KeyFrame[playIndex].rotInc = (KeyFrame[playIndex + 1].rotCuerpo - KeyFrame[playIndex].rotCuerpo) / i_max_steps; //Incremeneto en rotación de objeto

}

//Función de calculo de interpolación para realizar la animación y su velocidad de incremento, siendo lineal.
void interpolation2(void)
{

	KeyFrame2[playIndex2].incX2 = (KeyFrame2[playIndex2 + 1].posX2 - KeyFrame2[playIndex2].posX2) / i_max_steps2; //Incremento en X
	KeyFrame2[playIndex2].incY2 = (KeyFrame2[playIndex2 + 1].posY2 - KeyFrame2[playIndex2].posY2) / i_max_steps2; //Incremento en Y
	KeyFrame2[playIndex2].incZ2 = (KeyFrame2[playIndex2 + 1].posZ2 - KeyFrame2[playIndex2].posZ2) / i_max_steps2; //Incremento en Z

	KeyFrame2[playIndex2].rotInc2 = (KeyFrame2[playIndex2 + 1].rotBrazoDer - KeyFrame2[playIndex2].rotBrazoDer) / i_max_steps2; // Incremeneto en rotación de objeto
}

int main()
{
	// Inicialización de  GLFW
	glfwInit();
	// Seteamos los valores requeridos por GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Creamos el objeto de ventana por medio de la funcion de glfw, recibiendo resolución y nombre de ventana.
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Picapiedras", nullptr, nullptr);

	//Comprobación de creación de venta correctamente.
	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	//Cambio de contexto anterior a nuevo
	glfwMakeContextCurrent(window);

	//Recibimos el tamaño de buffer de ventana
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Inicializamos las funciones de callback de mouse y teclado 
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// Opciones de GLFW 
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Inicializamos para recibir funciones de punteros y extensiones
	glewExperimental = GL_TRUE;
	// Inicializamos GLEW para los punteros y verificamos que se haya creado correctamente
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Definimos las dimensiones del viewport
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glEnable(GL_DEPTH_TEST);

	/*
	* Inicializamos los shaders a usar
	* Shader para iluminación
	* Shader para iluminación
	* Shader de Skybox
	*/
	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
	Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");

	//Pedro Picapiedra
	//Modelo de la Cabeza
	Model CabezaP((char*)"Models/Obj/Models/Pedro/Cabeza/Cabeza.obj");
	//Modelo de el Cuerpo
	Model CuerpoP((char*)"Models/Obj/Models/Pedro/Cuerpo/CuerpoP.obj");
	//Modelo del Brazo Izquierdo
	Model BrazoIzq((char*)"Models/Obj/Models/Pedro/BrazoIzq/BrazoIzq.obj");
	//Modelo del Brazo Derecho
	Model BrazoDer((char*)"Models/Obj/Models/Pedro/BrazoDer/BrazoDer.obj");
	//Modelo de Pies
	Model PiesP((char*)"Models/Obj/Models/Pedro/Pies/Pies.obj");


	//Modelo de Dinosaurio
	Model Dino((char*)"Models/Obj/Models/Dragon/Dragon.obj");	

	//Shader creado para animacion del agua
	Shader Anim("Shaders/anim.vs", "Shaders/anim.frag");
	//Modelo de el agua
	Model Agua((char*)"Models/Obj/Models/Agua/Agua.obj");
	//Modelo del dinosaurio cuerpo
	Model DinosaurioCuerpo((char*)"Models/Obj/Models/DinoPerson/Cuerpo/Cuerpo.obj");
	//Modelo del dinosaurio pies
	Model DinosaurioPies((char*)"Models/Obj/Models/DinoPerson/Pies/Pies.obj");
	//Modelo del piso
	Model Piso((char*)"Models/Obj/Models/Piso/Piso.obj");
	//Modelo de la fachada
	Model Fachada((char*)"Models/Obj/Models/Fachada/Fachada.obj");
	//Modelo de Planta 1
	Model Planta1((char*)"Models/Obj/Models/Planta1/Planta1.obj");
	//Modelo de Planta 2
	Model Planta2((char*)"Models/Obj/Models/Planta2/Planta2.obj");
	//Modelo de Planta 3
	Model Planta3((char*)"Models/Obj/Models/Planta3/Planta3.obj");
	//Modelo de Planta 4
	Model Planta4((char*)"Models/Obj/Models/Planta4/Planta4.obj");
	//Modelo de Planta 5
	Model Planta5((char*)"Models/Obj/Models/Planta5/Planta5.obj");
	//Modelo de Planta 6
	Model Planta6((char*)"Models/Obj/Models/Planta6/Planta6.obj");
	//Modelo de Sillon1
	Model Sillon1((char*)"Models/Obj/Models/Sillon1/Sillon.obj");
	//Modelo de Sillon2
	Model Sillon2((char*)"Models/Obj/Models/Sillon2/Sillon2.obj");
	//Modelo de Cojines
	Model Cojines((char*)"Models/Obj/Models/Cojines/Cojines.obj");
	//Modelo de Mesa
	Model Mesa((char*)"Models/Obj/Models/Mesa/Mesa.obj");
	//Modelo de Mesa2
	Model Mesa2((char*)"Models/Obj/Models/Mesa2/Mesa2.obj");
	//Modelo de TV
	Model TV((char*)"Models/Obj/Models/TV/TV.obj");
	//Modelo de Tapete
	Model Tapete((char*)"Models/Obj/Models/Tapete/Tapete.obj");
	//Modelo de Libros
	Model Libros((char*)"Models/Obj/Models/Libros/Libros.obj");


	//Ciclo creado para inicializar valor seguro 0 todos los elementos de la estructura de frame
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].incX = 0;
		KeyFrame[i].incY = 0;
		KeyFrame[i].incZ = 0;
		KeyFrame[i].rotCuerpo= 0;
		KeyFrame[i].rotInc = 0;
	}

	//Ciclo creado para inicializar valor seguro 0 todos los elementos de la estructura de frame 2
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame2[i].posX2 = 0;
		KeyFrame2[i].incX2 = 0;
		KeyFrame2[i].incY2 = 0;
		KeyFrame2[i].incZ2 = 0;
		KeyFrame2[i].rotBrazoDer = 0;
		KeyFrame2[i].rotInc2 = 0;
	}


	//Arreglo de vertices para dibujo de cubo para skybox
	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	//Atributo de Posición
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Atributo para la normal 
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Inicilización de unidad de textura
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"), 1);


	//SkyBox
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	//load textures
	vector<const GLchar*> faces;
	faces.push_back("SkyBox/right.tga");
	faces.push_back("SkyBox/left.tga");
	faces.push_back("SkyBox/top.tga");
	faces.push_back("SkyBox/bottom.tga");
	faces.push_back("SkyBox/back.tga");
	faces.push_back("SkyBox/front.tga");

	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	Sonido();
	Sonido2();
	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		animacion();
		animacion2();
		animacion3();
		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//// OpenGL options
		//glEnable(GL_DEPTH_TEST);



		//Load Model


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.1f, 0.1f, 0.1f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.1f, 0.1f, 0.1f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.4f, 0.4f, 0.4f);


		// Point light 1
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);


		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 1.0f, 1.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 1.0f, 1.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 1.8f);

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), 1.5f, 0.82f, 1.18f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), -5.0f, 0.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), anim1, anim1, anim1);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), anim1, anim1, anim1);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), anim1, anim1, anim1);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.35f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.44f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);
		glBindVertexArray(VAO);
		glm::mat4 tmp = glm::mat4(1.0f); //Temp



		//Carga de modelo 
		//Personaje
		view = camera.GetViewMatrix();
		glm::mat4 model(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(-rotCuerpo), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DinosaurioCuerpo.Draw(lightingShader);

		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(posX, posY, posZ));
		//model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0));
		//model = glm::rotate(model, glm::radians(-rotPies), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DinosaurioPies.Draw(lightingShader);

		glBindVertexArray(0);


		//Dragon
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Dino.Draw(lightingShader);
		glBindVertexArray(0);

		//Carga de modelo 
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Piso.Draw(lightingShader);
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Cielo.Draw(lightingShader);
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Montañas.Draw(lightingShader);
	
		//Modelo
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Fachada.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Planta1.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Planta2.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Planta3.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Planta4.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Planta5.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Planta6.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Sillon1.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Sillon2.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Cojines.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mesa.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mesa2.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		TV.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Tapete.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Libros.Draw(lightingShader);

		//glDisable(GL_BLEND); //Desactiva el canal alfa 
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);
		glBindVertexArray(0);

		//Pedro
		view = camera.GetViewMatrix();
		//glm::mat4 model(1);
		//model = glm::translate(model, glm::vec3(posX, posY, posZ));
		//model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0));
		//model = glm::rotate(model, glm::radians(-rotBrazoDer), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CabezaP.Draw(lightingShader);

		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(posX, posY, posZ));
		//model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0));
		//model = glm::rotate(model, glm::radians(-rotBrazoIzq), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CuerpoP.Draw(lightingShader);

		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(posX2, posY2, posZ2));
		//model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0));
		//model = glm::rotate(model, glm::radians(-rotPies), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		PiesP.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::translate(model, glm::vec3(posX2, posY2, posZ2));
		model = glm::rotate(model, glm::radians(-rotBrazoDer), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BrazoDer.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::translate(model, glm::vec3(posX2, posY2, posZ2));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BrazoIzq.Draw(lightingShader);

		glBindVertexArray(0);


		Anim.Use();
		tiempo = glfwGetTime();
		modelLoc = glGetUniformLocation(Anim.Program, "model");
		viewLoc = glGetUniformLocation(Anim.Program, "view");
		projLoc = glGetUniformLocation(Anim.Program, "projection");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(Anim.Program, "time"), tiempo);
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Agua.Draw(Anim);
		
		glBindVertexArray(0);

		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		
		
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)

		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[0]);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[1]);
		model = glm::scale(model, glm::vec3(0.2f,0.5f,0.7f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);




		// Draw skybox as last
		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		SkyBoxshader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default




		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

void Sonido()
{
	//PlaySound(TEXT("picapiedra.wav"), NULL, SND_SYNC );
}

void Sonido2()
{

	PlaySound(TEXT("ambiente.wav"), NULL, SND_LOOP | SND_ASYNC);

}

void Sonido3()
{

	PlaySound(TEXT("gol.wav"), NULL, SND_ASYNC);

}

// Moves/alters the camera positions based on user input
void DoMovement()

{

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}

	if (keys[GLFW_KEY_I])
	{
		circuito = true;
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(1.0f, 1.0f, 0.0f);
		}
		else
		{
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
		}
	}

	if (keys[GLFW_KEY_O])
	{
		circuito = false;
	}



	if (keys[GLFW_KEY_1])
	{
		if (rotCuerpo < 45.0)
			rotCuerpo += 1.0f;

	}

	if (keys[GLFW_KEY_2])
	{
		if (rotCuerpo > -1)
			rotCuerpo -= 1.0f;

	}

	if (keys[GLFW_KEY_3])
	{
		if (rotBrazoDer< 20.0)
			rotBrazoDer += 1.0f;
	}

	if (keys[GLFW_KEY_4])
	{
		if (rotBrazoDer > 0)
			rotBrazoDer -= 1.0f;

	}

}


/// Animación Dinosaurio
void animacion()
{
	if (circuito)
	{
		if (recorrido1)
		{
			movKitX += 0.1f;
			if (movKitX > 20)
			{
				recorrido1 = false;
				recorrido2 = true;
			}
		}
		if (recorrido2)
		{
			rotKit += 1;
			if (rotKit > 90)
			{
				recorrido2 = false;
				recorrido3 = true;
			}
		}
		if (recorrido3)
		{
			movKitZ -= 0.1f;
			if (movKitZ < -20)
			{
				recorrido3 = false;
				recorrido4 = true;
			}
		}
		if (recorrido4)
		{
			rotKit += 1;
			if (rotKit > 180)
			{
				recorrido4 = false;
				recorrido5 = true;
			}
		}
		if (recorrido5)
		{
			movKitX -= 0.1f;
			if (movKitX < -20)
			{
				recorrido5 = false;
				recorrido6 = true;
			}
		}
		if (recorrido6)
		{
			rotKit += 1;
			if (rotKit > 270)
			{
				recorrido6 = false;
				recorrido7 = true;
			}
		}
		if (recorrido7)
		{
			movKitZ += 0.1f;
			if (movKitZ > 0)
			{
				recorrido7 = false;
				recorrido8 = true;
			}
		}
		if (recorrido8)
		{
			rotKit += 1;
			if (rotKit > 360)
			{
				recorrido8 = false;
				recorrido9 = true;
			}
		}
		if (recorrido9)
		{
			movKitX += 0.1f;
			if (movKitX > 0)
			{
				recorrido9 = false;
				rotKit = 0;
				recorrido1 = true;
			}
		}

	}
}
	

void animacion2()
{

	//Movimiento del personaje

	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += KeyFrame[playIndex].incX;
			posY += KeyFrame[playIndex].incY;
			posZ += KeyFrame[playIndex].incZ;

			rotCuerpo += KeyFrame[playIndex].rotInc;

			i_curr_steps++;
		}

	}
}

void animacion3()
{

	//Movimiento del personaje

	if (play2)
	{
		if (i_curr_steps2 >= i_max_steps2) //end of animation between frames?
		{
			playIndex2++;
			if (playIndex2 == 1)
			{
				Sonido3();
			}
			if (playIndex2 > FrameIndex2 - 2)	//end of total animation?
			{
				printf("termina anim\n");
				Sonido2();
				playIndex2 = 0;
				play2 = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps2 = 0; //Reset counter
								  //Interpolation
				interpolation2();
			}
		}
		else
		{
			//Draw animation
			posX2 += KeyFrame2[playIndex2].incX2;
			posY2 += KeyFrame2[playIndex2].incY2;
			posZ2 += KeyFrame2[playIndex2].incZ2;

			rotBrazoDer += KeyFrame2[playIndex2].rotInc2;

			i_curr_steps2++;
		}

	}
}

	// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_M])
	{
		bandera = bandera + 1;
		if (bandera%2==0)
		{
			anim1 = 0.0f;
		}
		else
		{
			anim1 = 1.0f;
		}

	}

	if (keys[GLFW_KEY_L])
	{
		if (play == false && (FrameIndex > 1))
		{

			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
		}

	}

	if (keys[GLFW_KEY_K])
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}

	}
	if (keys[GLFW_KEY_P])
	{
		if (play2 == false && (FrameIndex2 > 1))
		{

			resetElements2();
			//First Interpolation				
			interpolation2();

			play2 = true;
			playIndex2 = 0;
			i_curr_steps2 = 0;
		}
		else
		{
			play2 = false;
		}

	}

	if (keys[GLFW_KEY_O])
	{
		if (FrameIndex2 < MAX_FRAMES)
		{
			saveFrame2();
		}

	}

}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}