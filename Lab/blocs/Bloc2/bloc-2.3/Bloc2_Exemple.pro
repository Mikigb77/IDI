TEMPLATE    = app
QT         += opengl 

INCLUDEPATH +=  /home/kali/Desktop/upc/idi/IDI/Dependencies/ \
                ./Model/

FORMS += MyForm.ui

HEADERS += MyForm.h BL2GLWidget.h MyGLWidget.h Model/model.h

SOURCES += main.cpp MyForm.cpp \
        BL2GLWidget.cpp MyGLWidget.cpp \
        Model/model.cpp
