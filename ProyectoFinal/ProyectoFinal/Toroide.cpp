#include "Toroide.h"
void toroide()
{
    unsigned int indices[48600];
    GLfloat verticesTS[6075], verticesT[97200];

    int i, j, cont, cont2, aux;
    cont = 0;
    cont2 = 0;
    aux = 0;


    for (i = 0; i < 360; i = i + 45)
    {
        for (j = 0; j < 360; j = j + 45)
        {
            verticesT[cont] = (1 + 0.5 * cos((j * 3.14159265) / 180)) * cos((i * 3.14159265) / 180);
            verticesT[cont + 1] = 0.5 * sin((j * 3.14159265) / 180);
            verticesT[cont + 2] = (1 + 0.5 * cos((j * 3.14159265) / 180)) * sin((i * 3.14159265) / 180);

            cont = cont + 3;
        }
    }


    for (j = 0; j < 8; j++)
    {
        if (j == 7)
        {
            for (i = j * 8; i < j * 8 + 8; i++)
            {
                if (i == j * 8 + 7)
                {
                    indices[cont2] = i;
                    indices[cont2 + 1] = i - 7;
                    indices[cont2 + 2] = aux;
                    cont2 = cont2 + 3;
                    aux++;
                }
                else
                {
                    indices[cont2] = i;
                    indices[cont2 + 1] = i + 1;
                    indices[cont2 + 2] = aux;
                    cont2 = cont2 + 3;
                    aux++;
                }
            }
        }
        else
        {
            for (i = j * 8; i < j * 8 + 8; i++)
            {
                if (i == j * 8 + 7)
                {
                    indices[cont2] = i;
                    indices[cont2 + 1] = i - 7;
                    indices[cont2 + 2] = i + 8;
                    cont2 = cont2 + 3;
                }
                else
                {
                    indices[cont2] = i;
                    indices[cont2 + 1] = i + 1;
                    indices[cont2 + 2] = i + 8;
                    cont2 = cont2 + 3;
                }
            }
        }

    }


    aux = 0;
    for (j = 0; j < 8; j++)
    {
        if (j == 0)
        {
            for (i = j * 8; i < j * 8 + 8; i++)
            {
                if (i == j * 8 + 7)
                {
                    indices[cont2] = i;
                    indices[cont2 + 1] = i - 7;
                    indices[cont2 + 2] = 56;
                    cont2 = cont2 + 3;
                    aux++;
                }
                else
                {
                    indices[cont2] = i;
                    indices[cont2 + 1] = i + 1;
                    indices[cont2 + 2] = i + 57;
                    cont2 = cont2 + 3;
                    aux++;
                }
            }
        }
        else
        {
            for (i = j * 8; i < j * 8 + 8; i++)
            {
                if (i == j * 8 + 7)
                {
                    indices[cont2] = i;
                    indices[cont2 + 1] = i - 7;
                    indices[cont2 + 2] = i - 15;
                    cont2 = cont2 + 3;
                }
                else
                {
                    indices[cont2] = i;
                    indices[cont2 + 1] = i + 1;
                    indices[cont2 + 2] = i - 7;
                    cont2 = cont2 + 3;
                }
            }
        }
    }

    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glGenBuffers(1, &EBO);

    // Enlazar  Vertex Array Object
    glBindVertexArray(VAO2);

    //2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesT), verticesT, GL_STATIC_DRAW);
    // 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 4. Despues colocamos las caracteristicas de los vertices

    //Posicion
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);


    glBindBuffer(GL_ARRAY_BUFFER, 0);


    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)




}