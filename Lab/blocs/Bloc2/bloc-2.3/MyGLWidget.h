// MyGLWidget.h
#include "BL2GLWidget.h"
#include "model.h"

class MyGLWidget : public BL2GLWidget
{
  Q_OBJECT

public:
  MyGLWidget(QWidget *parent = 0) : BL2GLWidget(parent) {}
  ~MyGLWidget();

private:
  int printOglError(const char file[], int line, const char func[]);

protected:
  // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
  virtual void initializeGL();
  // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
  // Tot el que es dibuixa es dibuixa aqui.
  virtual void paintGL();
  // resizeGL - És cridat quan canvia la mida del widget
  virtual void resizeGL(int width, int height);
  // keyPressEvent - Es cridat quan es prem una tecla
  virtual void keyPressEvent(QKeyEvent *event);

  virtual void creaBuffers();
  virtual void carregaShaders();
  virtual void modelTransform();
  virtual void modelTransformFloor();
  virtual void projectTransform();
  virtual void viewTransform();
  virtual void iniCamera();

  // function to get the minmaxCoordenates from a custom figure from it's triangles
  virtual void minMaxCoordenates(glm::vec3 v[], unsigned int size);

  // function to get the minmaxCoordenates from a custom Model
  virtual void minMaxCoordenates(Model &m);

  // updates the camera from the sphere containg the scene
  virtual void updateCamera();

  Model model;

  GLfloat FOV, ra, znear, zfar;
  glm::vec3 OBS, VRP, UP;
  GLfloat rotation;

  glm::vec3 minCoordenates, maxCoordenates;
  glm::vec3 sphereCenter;
  GLfloat sphereRadius;

  glm::vec3 modelCenter, modelBase;
  GLfloat modelHeigth;
  GLfloat targetHeigth;

  GLboolean orthogonal;
  GLfloat orthoLeft, orthoRigth, orthoBottom, orthoTop;
  GLuint VAO_Homer;
  GLuint VAO_Floor;

  GLfloat pitch, yaw, roll;

  GLuint projLoc;
  GLuint viewLoc;
};
