// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__, __FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

int MyGLWidget::printOglError(const char file[], int line, const char func[])
{
    GLenum glErr;
    int retCode = 0;

    glErr = glGetError();
    const char *error = 0;
    switch (glErr)
    {
    case 0x0500:
        error = "GL_INVALID_ENUM";
        break;
    case 0x501:
        error = "GL_INVALID_VALUE";
        break;
    case 0x502:
        error = "GL_INVALID_OPERATION";
        break;
    case 0x503:
        error = "GL_STACK_OVERFLOW";
        break;
    case 0x504:
        error = "GL_STACK_UNDERFLOW";
        break;
    case 0x505:
        error = "GL_OUT_OF_MEMORY";
        break;
    default:
        error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
               file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget()
{
}

void MyGLWidget::initializeGL()
{
    BL2GLWidget::initializeGL();

    iniCamera();
}

void MyGLWidget::iniCamera()
{
    // variables
    FOV = M_PI / 2, ra = 1, znear = 0.4, zfar = 3;
    OBS = glm::vec3(0, 0, 1), VRP = glm::vec3(0, 0, 0), UP = glm::vec3(0, 1, 0);
    rotation = 0;

    // functions
    projectTransform();
    viewTransform();
    glEnable(GL_DEPTH_TEST);
}

void MyGLWidget::projectTransform()
{
    glm::mat4 Proj = glm::perspective(FOV, ra, znear, zfar);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform()
{
    glm::mat4 View = glm::lookAt(OBS, VRP, UP);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::paintGL()
{
    // BL2GLWidget::paintGL();

    // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
    // useu els paràmetres que considereu (els que hi ha són els de per defecte)
    //  glViewport (0, 0, ample, alt);

    // clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // transformacions
    viewTransform();
    modelTransform();

    glBindVertexArray(VAO_Homer);

    glDrawArrays(GL_TRIANGLES, 0, model.faces().size() * 3);

    glBindVertexArray(0);

    // the transform for the floor
    modelTransformFloor();

    // bind the VAO_Floor
    glBindVertexArray(VAO_Floor);

    glDrawArrays(GL_TRIANGLES, 0, 3 * 2);

    glBindVertexArray(0);
}

void MyGLWidget::resizeGL(int width, int height)
{
    BL2GLWidget::resizeGL(width, height);
}

void MyGLWidget::keyPressEvent(QKeyEvent *event)
{
    makeCurrent();
    switch (event->key())
    {
    case Qt::Key_S:
    { // escalar a més gran
        escala += 0.05;
        break;
    }
    case Qt::Key_D:
    { // escalar a més petit
        escala -= 0.05;
        break;
    }
    case Qt::Key_R:
        rotation += M_PI / 4;
        break;

    default:
        event->ignore();
        break;
    }
    update();
}

void MyGLWidget::creaBuffers()
{
    BL2GLWidget::creaBuffers();

    // creem el homer
    model.load("../../../models/HomerProves.obj");

    glGenVertexArrays(1, &VAO_Homer);
    glBindVertexArray(VAO_Homer);

    GLuint VBO_Homer[2];
    glGenBuffers(2, VBO_Homer);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Homer[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * model.faces().size() * 3 * 3, model.VBO_vertices(), GL_STATIC_DRAW);
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Homer[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * model.faces().size() * 3 * 3, model.VBO_matdiff(), GL_STATIC_DRAW);
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

    glBindVertexArray(0);

    glm::vec3 vertex[6] = {
        glm::vec3(-2.0, 0.0, -2),
        glm::vec3(2, 0.0, -2),
        glm::vec3(-2, 0.0, 2),
        glm::vec3(-2, 0.0, 2),
        glm::vec3(2, 0.0, -2),
        glm::vec3(2, 0.0, 2)

    };
    glm::vec3 colors[6] = {
        glm::vec3(1, 0, 0),
        glm::vec3(0, 1, 0),
        glm::vec3(0, 0, 1),
        glm::vec3(0, 0, 1),
        glm::vec3(0, 1, 0),
        glm::vec3(1, 0, 0)

    };

    // Creació del Vertex Array Object per pintar
    glGenVertexArrays(1, &VAO_Floor);
    glBindVertexArray(VAO_Floor);

    GLuint VBO_Floor[2];
    glGenBuffers(2, VBO_Floor);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Floor[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

    // Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Floor[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    // Activem l'atribut colorLoc
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

    glBindVertexArray(0);
}

void MyGLWidget::modelTransform()
{
    // Matriu de transformació de model
    glm::mat4 transform(1.0f);
    transform = glm::scale(transform, glm::vec3(escala));
    transform = glm::rotate(transform, rotation, glm::vec3(0, 1, 0));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformFloor()
{
    // Matriu de transformació de model
    glm::mat4 transform(1.0f);
    transform = glm::scale(transform, glm::vec3(escala));
    transform = glm::translate(transform, glm::vec3(0, -1, 0));
    // transform = glm::rotate(transform, rotation, glm::vec3(0, 1, 0));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::carregaShaders()
{
    BL2GLWidget::carregaShaders();
    projLoc = glGetUniformLocation(program->programId(), "proj");
    viewLoc = glGetUniformLocation(program->programId(), "view");
}
