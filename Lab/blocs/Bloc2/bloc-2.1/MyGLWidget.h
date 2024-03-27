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

  Model model;

  GLfloat FOV, ra, znear, zfar;
  glm::vec3 OBS, VRP, UP;
  GLfloat rotation;

  GLuint VAO_Homer;
  GLuint VAO_Floor;

  GLuint projLoc;
  GLuint viewLoc;
};
