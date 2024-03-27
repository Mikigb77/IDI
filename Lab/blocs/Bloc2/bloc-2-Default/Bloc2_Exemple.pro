TEMPLATE    = app
QT         += opengl 

INCLUDEPATH +=  /usr/include/glm
INCLUDEPATH +=  ./Model

FORMS += MyForm.ui

HEADERS += MyForm.h BL2GLWidget.h MyGLWidget.h model.h

SOURCES += main.cpp MyForm.cpp \
        BL2GLWidget.cpp MyGLWidget.cpp \
        ./Model/model.cc
