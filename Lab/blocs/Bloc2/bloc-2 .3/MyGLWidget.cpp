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

    // default values for the variables
    FOV = M_PI / 2, ra = 1, znear = 0.4, zfar = 3;
    OBS = glm::vec3(0, 0, 1), VRP = glm::vec3(0, 0, 0), UP = glm::vec3(0, 1, 0);
    rotation = 0;
    orthogonal = false;
    orthoLeft = orthoRigth = orthoBottom = orthoTop = 1;
    minCoordenates = glm::vec3(INFINITY);
    maxCoordenates = glm::vec3(-INFINITY);
    targetHeigth = 4;

    // default initialization foor the openGL + call to creaBuffers
    BL2GLWidget::initializeGL();

    iniCamera();
}

void MyGLWidget::iniCamera()
{
    // update the camera with the sphere containg the sceen
    updateCamera();
    glEnable(GL_DEPTH_TEST);
}

// use only in debugging never in production code
void printMinMax(glm::vec3 min, glm::vec3 max)
{
    printf("\nmin: (%f, %f, %f)", min.x, min.y, min.z);
    printf("\nmax: (%f, %f, %f)", max.x, max.y, max.z);
}

void MyGLWidget::minMaxCoordenates(glm::vec3 v[], unsigned int size)
{
    for (unsigned int i = 0; i < size; ++i)
    {
        minCoordenates = glm::min(minCoordenates, v[i]);
        maxCoordenates = glm::max(maxCoordenates, v[i]);
    }
}

void MyGLWidget::minMaxCoordenates(Model &m)
{
    glm::vec3 auxMin = glm::vec3(INFINITY), auxMax = glm::vec3(-INFINITY);
    for (unsigned int i = 0; i < m.vertices().size(); i += 3)
    {
        glm::vec3 aux = glm::vec3(m.vertices()[i], m.vertices()[i + 1], m.vertices()[i + 2]);
        auxMin = glm::min(auxMin, aux);
        auxMax = glm::max(auxMax, aux);
    }

    // get the center base and heigth of the model
    modelCenter = (auxMax + auxMin) / 2.f;
    modelBase = glm::vec3(modelCenter.x, auxMin.y, modelCenter.z);
    modelHeigth = auxMax.y - auxMin.y;

    // make the auxMin and auxMax relative to the center or base of the model (depending on what you need)
    auxMax -= modelBase;
    auxMin -= modelBase;

    // scale them to get the actual final heigth of the model on when it's rendered (dependent on targetHeigth)
    // make sure this transformations are present on the model transform (otherwise skip them)
    auxMax *= targetHeigth / modelHeigth;
    auxMin *= targetHeigth / modelHeigth;

    // finaly use the "normalized" model coordenates to update the minMaxCoordenates
    minCoordenates = glm::min(minCoordenates, auxMin);
    maxCoordenates = glm::max(maxCoordenates, auxMax);
}

void MyGLWidget::updateCamera()
{
    // only for debuging purpouses:
    // printMinMax(minCoordenates, maxCoordenates);

    //  update the sphere radius and the sphere center
    sphereCenter = (minCoordenates + maxCoordenates) / 2.f;
    sphereRadius = glm::distance(sphereCenter, minCoordenates);

    // update the variabels that depend on the containg sphere
    OBS = glm::vec3(0, sphereRadius / 2, 2 * sphereRadius);
    znear = glm::distance(OBS, sphereCenter) - sphereRadius;
    zfar = glm::distance(OBS, sphereCenter) + sphereRadius;
    VRP = sphereCenter;
    FOV = glm::asin(sphereRadius / glm::distance(sphereCenter, OBS)) * 2.f;

    // since we wanna update the camera we also need to update all the parameters related to it

    // update the relation aspect
    ra = float(QOpenGLWidget::width()) / float(QOpenGLWidget::height());

    if (ra < 1)
        // update the FOV based on the ra
        FOV = glm::atan(glm::tan(FOV / 2.f) / ra) * 2.f;

    // update the parameters used for the orthogonal view
    orthoLeft = orthoBottom = -sphereRadius;
    orthoRigth = orthoTop = sphereRadius;

    // update them depending on the ra
    if (ra > 1)
    {
        orthoLeft *= ra;
        orthoRigth *= ra;
    }
    else if (ra < 1)
    {
        orthoBottom /= ra;
        orthoTop /= ra;
    }

    // update the projectionMatrix and the viewMatrix
    projectTransform();
    viewTransform();
}

void MyGLWidget::projectTransform()
{
    glm::mat4 Proj;
    if (orthogonal)
        Proj = glm::ortho(orthoLeft, orthoRigth, orthoBottom, orthoTop, znear, zfar);
    else
        Proj = glm::perspective(FOV, ra, znear, zfar);
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
    updateCamera();
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
    { // rotar el model 45 graus
        rotation += M_PI / 4;
        break;
    }
    case Qt::Key_O:
    { // activa/desactiva la camera ortogonal
        orthogonal = !orthogonal;
        projectTransform();
        break;
    }
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
    model.load("../../../models/Patricio.obj");

    // update the minMaxCoordenates
    minMaxCoordenates(model);

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
        glm::vec3(-2.5, 0.0, -2.5),
        glm::vec3(2.5, 0.0, -2.5),
        glm::vec3(-2.5, 0.0, 2.5),
        glm::vec3(-2.5, 0.0, 2.5),
        glm::vec3(2.5, 0.0, -2.5),
        glm::vec3(2.5, 0.0, 2.5)

    };
    glm::vec3 colors[6] = {
        glm::vec3(1, 0, 0),
        glm::vec3(0, 1, 0),
        glm::vec3(0, 0, 1),
        glm::vec3(0, 0, 1),
        glm::vec3(0, 1, 0),
        glm::vec3(1, 0, 0)

    };

    // check for the minMaxCoordenates
    minMaxCoordenates(vertex, 6);

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
    // aconseguim una escalació unitaria a partir de l'altura
    transform = glm::scale(transform, glm::vec3(1.f / modelHeigth));
    // escalem el model a la altrua desitjada
    transform = glm::scale(transform, glm::vec3(targetHeigth));
    // apliquem la escala final sobre el model escalat a targetHeigth
    transform = glm::scale(transform, glm::vec3(escala));
    // apliquem la rotació
    transform = glm::rotate(transform, rotation, glm::vec3(0, 1, 0));
    // portem el model al origen de coordenades
    // (en aquest cas portem la base del model al origen i la resta de vertex seran relatius a la base)
    transform = glm::translate(transform, -modelBase);
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformFloor()
{
    // Matriu de transformació de model
    glm::mat4 transform(1.0f);
    transform = glm::translate(transform, glm::vec3(0));
    transform = glm::scale(transform, glm::vec3(escala));
    transform = glm::translate(transform, glm::vec3(0, 0, 0));
    //  transform = glm::rotate(transform, rotation, glm::vec3(0, 1, 0));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::carregaShaders()
{
    BL2GLWidget::carregaShaders();
    projLoc = glGetUniformLocation(program->programId(), "proj");
    viewLoc = glGetUniformLocation(program->programId(), "view");
}
