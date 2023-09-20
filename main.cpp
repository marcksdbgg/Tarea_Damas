#include <iostream>
#include "Graphics.h"
#include "table.h"

using namespace std;


// Inicializamos la ventana de color blanco
void init(void) {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-100.0, 100.0, -100.0, 100.0);
}

// Dibujamos los cuadrados como tabla
void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    drawTable();
    glFlush();
}

int main(int argc, char** argv){
    // inicializamos la ventana con fondo blanco
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(720, 720);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Circulos");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}