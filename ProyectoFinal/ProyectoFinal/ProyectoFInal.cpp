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

// Libreria de GLEW
#include <GL/glew.h>

// Libreria de GLFW
#include <GLFW/glfw3.h>

// Libreria de std image.h
#include "stb_image.h"

// Libreria GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Libreria de Load Models
#include "SOIL2/SOIL2.h"


// Libreria creadas
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


	// Inicialización de las funciones VAO y VBO
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	//Establecemos el atributo de posición
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//Establecemos el atributo para la normal 
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Inicilización de unidad de textura
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"), 1);


	//Inicialización del SkyBox
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	//Carga de imagenes para texturizar el skybox
	vector<const GLchar*> faces;
	faces.push_back("SkyBox/right.tga");
	faces.push_back("SkyBox/left.tga");
	faces.push_back("SkyBox/top.tga");
	faces.push_back("SkyBox/bottom.tga");
	faces.push_back("SkyBox/back.tga");
	faces.push_back("SkyBox/front.tga");

	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);
	//Creación de proyección en perspectiva
	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	//Inicialización de funciones de sonido ambiente e intro
	Sonido();
	Sonido2();
	// Ciclo De ejecución continua
	while (!glfwWindowShouldClose(window))
	{

		// Calculamos el ultimo frame y su diferentecía entre el actual y el ultimo
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Revisamos la inicialización de eventos para animaciones constantes y revisamos banderas
		glfwPollEvents();
		DoMovement();
		animacion();
		animacion2();
		animacion3();
		// Limpiamos el buffer de ejecución y color
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Declaramos el shader de iluminación a inicar uso
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		//Iniciamos su posicionamiento
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Establecemos la iluminación direccional, dedsde dirección, escular, ambiental y difusa.
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.1f, 0.1f, 0.1f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.1f, 0.1f, 0.1f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.4f, 0.4f, 0.4f);


		// Vector de Iluminación de punto
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);

		//Inicialización de pposición, e iluminación desde el arreglo de posiciones
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 1.0f, 1.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 1.0f, 1.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 1.8f);

		// Iniciaización de posición de spotlight, usado como animación de encedido y apagado de TV
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

		// Iniciamos la propiedad de brillo sobre material
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);

		// Creamos la creación de camara
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Iniciamos la prespectica de model, view y proyección 
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Realizamos la matreiz para la vista y proyección
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);
		glm::mat4 tmp = glm::mat4(1.0f); //Temp

		//Carga de modelo de personaje de dinosaurio
		view = camera.GetViewMatrix();//establecemos la vista 
		glm::mat4 model(1);//Declaración de model
		model = glm::translate(model, glm::vec3(posX, posY, posZ));//Transformación de traslación a posición inicial
		model = glm::rotate(model, glm::radians(-rotCuerpo), glm::vec3(0.0f, 0.0f, 1.0f));//Transformación de rotación por variable
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); //Envíamos el modelo a la matriz 
		DinosaurioCuerpo.Draw(lightingShader);//Dibujamos el modelo con parametro recibido de shader de iluminación

		view = camera.GetViewMatrix();//Establecemos vista
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));//Enviamos matriz de modelo
		DinosaurioPies.Draw(lightingShader);//Dibujamos el modelo  con parametro recibido de shader de iluminación

		glBindVertexArray(0);//Finalizamos vertex array


		//Caragamos modelo del dragon volador
		view = camera.GetViewMatrix();//Iniciamos vista
		model = glm::mat4(1);//Iniciamos modelo
		model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));//asignamos transformación de traslación al modelo, usado para animación
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));//asignamos transofmración de rotación al modelo, usado para cambio de dirección 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));//Enviamos a la matriz el modelo
		Dino.Draw(lightingShader);//Dibujamos el modelo del dinosaurio
		glBindVertexArray(0);//Finalizamos vertex array

		//Carga de modelo del piso
		view = camera.GetViewMatrix();//Iniciamos vista
		model = glm::mat4(1);//Iniciamos modelo
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));//Enviamos a la matriz el modelo
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);//Ingresamos transparencia a objetos
		Piso.Draw(lightingShader);//Dibujamos 
	
		//Envíamos Modelo de Fachada a matriz
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Dibujamos el modelo de fachada
		Fachada.Draw(lightingShader);
		//Envíamos Modelo de Planta1 a matriz
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Dibujamos el modelo de Planta1
		Planta1.Draw(lightingShader);
		//Envíamos Modelo de Planta2 a matriz
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Dibujamos el modelo de Planta1
		Planta2.Draw(lightingShader);
		//Envíamos Modelo de Planta3 a matriz
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Dibujamos el modelo de Planta3
		Planta3.Draw(lightingShader);
		//Envíamos Modelo de Planta4 a matriz
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Dibujamos el modelo de Planta4
		Planta4.Draw(lightingShader);
		//Envíamos Modelo de Planta5 a matriz
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Dibujamos el modelo de Planta5
		Planta5.Draw(lightingShader);
		//Envíamos Modelo de Planta6 a matriz
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Dibujamos el modelo de Planta6
		Planta6.Draw(lightingShader);
		//Envíamos Modelo de Sillon1 a matriz
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Dibujamos el modelo de Sillon1
		Sillon1.Draw(lightingShader);
		//Envíamos Modelo de Sillon2 a matriz
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Dibujamos el modelo de Sillon2
		Sillon2.Draw(lightingShader);
		//Envíamos Modelo de Cojines a matriz
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Dibujamos el modelo de Cojines
		Cojines.Draw(lightingShader);
		//Envíamos Modelo de Mesa a matriz
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Dibujamos el modelo de Mesa
		Mesa.Draw(lightingShader);
		//Envíamos Modelo de Mesa2 a matriz
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Dibujamos el modelo de Mesa2
		Mesa2.Draw(lightingShader);
		//Envíamos Modelo de TV a matriz
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Dibujamos el modelo de TV
		TV.Draw(lightingShader);
		//Envíamos Modelo de Tapete a matriz
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Dibujamos el modelo de Tapete
		Tapete.Draw(lightingShader);
		//Envíamos Modelo de Libros a matriz
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Dibujamos el modelo de Libros
		Libros.Draw(lightingShader);

		//Enviamos el color alpha
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);
		//Finalizamos vertex array
		glBindVertexArray(0);

		//Carga de Modelo Pedro Picapiedra
		//Iniciamos Camara de vista
		view = camera.GetViewMatrix();
		//Enviamos modelo Cabeza de Pedro a la matriz
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Dibujamos el modelo de Cabeza
		CabezaP.Draw(lightingShader);

		//Iniciamos Camara de vista
		view = camera.GetViewMatrix();
		//Enviamos modelo Cuerpo de Pedro a la matriz
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Dibujamos el modelo de Cuerpo
		CuerpoP.Draw(lightingShader);

		//Iniciamos Camara de vista
		view = camera.GetViewMatrix();
		//Enviamos modelo Pies de Pedro a la matriz
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Dibujamos el modelo de Pies
		PiesP.Draw(lightingShader);

		//Iniciamos Camara de vista
		view = camera.GetViewMatrix();
		//Transformación de traslación al modelo
		model = glm::translate(model, glm::vec3(posX2, posY2, posZ2));
		//Transformación de rotación al modelo
		model = glm::rotate(model, glm::radians(-rotBrazoDer), glm::vec3(0.0f, 0.0f, 1.0f));
		//Enviamos modelo brazo de derecho de Pedro a la matriz
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Dibujamos el modelo de Brazo Derecho
		BrazoDer.Draw(lightingShader);

		//Iniciamos Camara de vista
		view = camera.GetViewMatrix();
		model = glm::translate(model, glm::vec3(posX2, posY2, posZ2));
		//Enviamos modelo brazo de izquierdo de Pedro a la matriz
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Dibujamos el modelo de Brazo Izquierdo
		BrazoIzq.Draw(lightingShader);

		//Finalizamos vertex array
		glBindVertexArray(0);

		//Iniciamos el shader animación para uso
		Anim.Use();
		//Utilizamos el tiempo generado por procesador para animación
		tiempo = glfwGetTime();
		//Inicamos modelo, vista y proyección del modelo
		modelLoc = glGetUniformLocation(Anim.Program, "model");
		viewLoc = glGetUniformLocation(Anim.Program, "view");
		projLoc = glGetUniformLocation(Anim.Program, "projection");
		//Enviamos vista a la matriz de modelo
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		//Enviamos proyección a la matriz de modelo
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		//Enviamos modelo a la matriz de modelo
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Enviamos el tiempo del procesador a la matriz de modelo
		glUniform1f(glGetUniformLocation(Anim.Program, "time"), tiempo);
		//Inicializamos modelo de objeto agua
		model = glm::mat4(1);
		//Enviamos modelo de objeto
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Dibujamos objeto con parametro de animación recibido
		Agua.Draw(Anim);
		//Finalizamos vertex array
		glBindVertexArray(0);

		// Inicializamos uso de shader de iluminación
		lampShader.Use();
		
		//Inicializamos objeto de modelo, vista y proyección
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		//Enviamos vista y proyección a la matriz
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		//Inicialización del modelo de iluminación
		model = glm::mat4(1);
		//Transformación de traslación en iluminación
		model = glm::translate(model, lightPos);
		//Transformación de escalado en iluminación
		model = glm::scale(model, glm::vec3(0.2f));
		//Enviar modelo a matriz 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		
		
		//Creación  de modelo de iluminación punto
		model = glm::mat4(1);
		//Transformación de traslación en iluminación punto
		model = glm::translate(model, pointLightPositions[0]);
		//Transformación de escalado en iluminación punto
		model = glm::scale(model, glm::vec3(0.2f)); 
		//Enviar modelo a matriz
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		//Dibujado de cubo de point light
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Creación de modelo de iluminación punto
		model = glm::mat4(1);
		//Transformación de traslación en iluminación punto
		model = glm::translate(model, pointLightPositions[1]);
		//Transformación de escalado en iluminación punto
		model = glm::scale(model, glm::vec3(0.2f,0.5f,0.7f)); 
		//Enviar modelo a matriz
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		//Dibujado de cubo de point light
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// Dibujamos al ultimo el skybox
		glDepthFunc(GL_LEQUAL);
		//Iniciamos shader de sky box para su uso
		SkyBoxshader.Use();
		//Movemos el componente de vista de la matriz
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		//Enviamos la vista y proyección a la matriz 
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// Dibujamos el cubo para la asignación de skybox
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default




		// intercambiamos los buffer de ventana
		glfwSwapBuffers(window);
	}

	//Eliminamos memoria de skybox y objetos
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);
	// Terminamos GLFW para limpiar cualquier recurso almacenado
	glfwTerminate();



	return 0;
}

//Funcion de Insertado de sonido de intro
void Sonido()
{
	//PlaySound(TEXT("picapiedra.wav"), NULL, SND_SYNC );
}

//Funcion de Insertado de sonido de ambientación
void Sonido2()
{

	PlaySound(TEXT("ambiente.wav"), NULL, SND_LOOP | SND_ASYNC);

}

//Función de insertado de sonido de animación gol
void Sonido3()
{

	PlaySound(TEXT("gol.wav"), NULL, SND_ASYNC);

}

// Funciónde movimiento de teclas
void DoMovement()

{

	// Control de camara para enfrente
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	//Control de camara para atras
	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	//Control de camara para izquierda
	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	//Control de camara para derecha
	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}

	//Control de iniciado de animación de movimiento de dragon
	if (keys[GLFW_KEY_I])
	{
		circuito = true;
	}

	//Control de pausado de movimiento de dragon
	if (keys[GLFW_KEY_O])
	{
		circuito = false;
	}


	//Movimiento de cuerpo de dinosaurio positivo
	if (keys[GLFW_KEY_1])
	{
		if (rotCuerpo < 45.0)
			rotCuerpo += 1.0f;

	}

	//Movimiento de cuerpo de dinosaurio negativo
	if (keys[GLFW_KEY_2])
	{
		if (rotCuerpo > -1)
			rotCuerpo -= 1.0f;

	}

	//Movimiento de brazos de Pedro positivo
	if (keys[GLFW_KEY_3])
	{
		if (rotBrazoDer< 20.0)
			rotBrazoDer += 1.0f;
	}

	//Movimiento de brazos de Pedro negativo
	if (keys[GLFW_KEY_4])
	{
		if (rotBrazoDer > 0)
			rotBrazoDer -= 1.0f;

	}

}


/// Animación Dinosaurio
void animacion()
{
	//Bandera de activación
	if (circuito)
	{
		//Bandera de activación recorrido 1
		if (recorrido1)
		{
			//Desplazamiento en X
			movKitX += 0.1f;
			//Bandera de limite de 20 unidades
			if (movKitX > 20)
			{
				//Cambio de recorrido
				recorrido1 = false;
				recorrido2 = true;
			}
		}
		//Bandera de activación recorrido 2
		if (recorrido2)
		{
			//Cambio de dirección
			rotKit += 1;
			//Bandera de limite de 90 grados
			if (rotKit > 90)
			{
				//Cambio de recorrido
				recorrido2 = false;
				recorrido3 = true;
			}
		}
		//Bandera de activación recorrido 3
		if (recorrido3)
		{
			//Desplazamiento en Z negativo
			movKitZ -= 0.1f;
			//Bandera de limite de 20 unidades menos
			if (movKitZ < -20)
			{
				//Cambio de recorrido
				recorrido3 = false;
				recorrido4 = true;
			}
		}
		//Bandera de activación recorrido 4
		if (recorrido4)
		{
			//Cambio de dirección
			rotKit += 1;
			//Bandera de limite en 180 grados
			if (rotKit > 180)
			{
				//Cambio de recorrido
				recorrido4 = false;
				recorrido5 = true;
			}
		}
		//Bandera de activación recorrido 5
		if (recorrido5)
		{
			//Desplazamiento en X negativo
			movKitX -= 0.1f;
			//Bandera de limite de 20 unidades menos
			if (movKitX < -20)
			{
				//Cambio de recorrido
				recorrido5 = false;
				recorrido6 = true;
			}
		}
		//Bandera de activación recorrido 6
		if (recorrido6)
		{
			//Cambio de dirección
			rotKit += 1;
			//Bandera de limite en 270 grados
			if (rotKit > 270)
			{
				//Cambio de recorrido
				recorrido6 = false;
				recorrido7 = true;
			}
		}
		//Bandera de activación recorrido 7
		if (recorrido7)
		{
			//Desplazamiento en Z positivo
			movKitZ += 0.1f;
			//Bandera de limite de 0 unidades
			if (movKitZ > 0)
			{
				//Cambio de recorrido
				recorrido7 = false;
				recorrido8 = true;
			}
		}
		//Bandera de activación recorrido 8
		if (recorrido8)
		{
			//Cambio de dirección
			rotKit += 1;
			//Bandera de limite en 360 grados
			if (rotKit > 360)
			{
				//Cambio de recorrido
				recorrido8 = false;
				recorrido9 = true;
			}
		}
		//Bandera de activación recorrido 9
		if (recorrido9)
		{
			//Desplazamiento en X positivo
			movKitX += 0.1f;
			//Bandera de limite de 0 unidades
			if (movKitX > 0)
			{
				//Cambio de recorrido
				recorrido9 = false;
				rotKit = 0;
				recorrido1 = true;
			}
		}

	}
}
	
//Funcion de Animacion 2 por key frames
void animacion2()
{

	//Bandera de play
	if (play)
	{
		//Bandera de pasos actaules mayor a pasos maximos
		if (i_curr_steps >= i_max_steps) 
		{
			//Incremento de indice de actual
			playIndex++;
			//Bandera indice actual al indice almacenado no sea mayor, termina animación
			if (playIndex > FrameIndex - 2)	
			{
				printf("termina anim\n");
				//Inicializamos a valores iniciales 
				playIndex = 0;
				play = false;
			}
			else 
			{
				//Reiniciamos contador
				i_curr_steps = 0; 
				//interpolamos para realizar 
				interpolation();
			}
		}
		else
		{
			//Recorrido de posición 
			posX += KeyFrame[playIndex].incX;
			posY += KeyFrame[playIndex].incY;
			posZ += KeyFrame[playIndex].incZ;

			rotCuerpo += KeyFrame[playIndex].rotInc;

			i_curr_steps++;
		}

	}
}

//Funcion de Animacion 3 por key frames
void animacion3()
{
	//Bandera de play
	if (play2)
	{
		//Bandera de pasos actaules mayor a pasos maximos
		if (i_curr_steps2 >= i_max_steps2) 
		{
			//Incremento de indice de actual
			playIndex2++;
			//Bandera de posición 2
			if (playIndex2 == 1)
			{
				//Función de sonido 3, sonido de gol
				Sonido3();
			}
			//Bandera indice actual al indice almacenado no sea mayor, termina animación
			if (playIndex2 > FrameIndex2 - 2)	
			{
				printf("termina anim\n");
				//Función de sonido 2, sonido de ambiente
				Sonido2();
				//Inicializamos a valores iniciales 
				playIndex2 = 0;
				play2 = false;
			}
			else 
			{
				//Reiniciamos contador
				i_curr_steps2 = 0; 
				//interpolamos para realizar 
				interpolation2();
			}
		}
		else
		{
			//Recorrido de posición 
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

	//Tecla M activación de animación de luz de TV
	if (keys[GLFW_KEY_M])
	{
		//Contador
		bandera = bandera + 1;
		//Bandera para apagado
		if (bandera%2==0)
		{
			anim1 = 0.0f;
		}
		//Caso contrario Enciende la luz
		else
		{
			anim1 = 1.0f;
		}

	}

	//Tecla L de activación para reproducción de key frames
	if (keys[GLFW_KEY_L])
	{
		//Bandera de estado de animación, y indice de frame sea mayor a 0
		if (play == false && (FrameIndex > 1))
		{
			//Reiniciamos Elementos
			resetElements();
			//Realizamos interpolación			
			interpolation();
			//Iniciamos valores seguros
			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			//Caso contrario apaga animación
			play = false;
		}

	}

	//Tecla K para guarda estado de posición de objeto y key frame
	if (keys[GLFW_KEY_K])
	{
		//Bandera de el indice de frames es menor al maximo de frames 
		if (FrameIndex < MAX_FRAMES)
		{
			//Función para salvar el estado, posicion del objeto y frame
			saveFrame();
		}

	}

	//Tecla P de activación para reproducción de key frames
	if (keys[GLFW_KEY_P])
	{
		//Bandera de estado de animación, y indice de frame sea mayor a 0
		if (play2 == false && (FrameIndex2 > 1))
		{
			//Reiniciamos Elementos
			resetElements2();
			//Realizamos interpolación				
			interpolation2();
			//Iniciamos valores seguros
			play2 = true;
			playIndex2 = 0;
			i_curr_steps2 = 0;
		}
		else
		{
			//Caso contrario apaga animación
			play2 = false;
		}

	}

	//Tecla O para guarda estado de posición de objeto y key frame
	if (keys[GLFW_KEY_O])
	{
		//Bandera de el indice de frames es menor al maximo de frames 
		if (FrameIndex2 < MAX_FRAMES)
		{
			//Función para salvar el estado, posicion del objeto y frame
			saveFrame2();
		}

	}

}

//Función de llamada de mouse
void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	//Bandera para lectura de posición de mouse
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	//Almacenado de posición anterior de ambos ejes
	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	//Almacen de posición actual
	lastX = xPos;
	lastY = yPos;

	//enviado de posición a funcion y procesamiento de camara
	camera.ProcessMouseMovement(xOffset, yOffset);
}