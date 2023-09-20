//
// Created by marck on 19/09/2023.
//

#ifndef TAREA_DAMAS_GRAPHICS_H
#define TAREA_DAMAS_GRAPHICS_H

#include "table.h"
#include "Gl/glut.h"
#include <cmath>

struct Circle{
    float x;
    float y;
    float radius;
    float color[3];
};

// Estructura de una linea con color
struct Line{
    float x1;
    float y1;
    float x2;
    float y2;
    float color[3];
    float width;
};

// Estructura cuadrado
struct square{
    int x;
    int y;
    int width;
    int height;
    float color[3];
};

// Dibuja un circulo
void drawCircle(Circle circle){
    glColor3f(circle.color[0], circle.color[1], circle.color[2]);
    glBegin(GL_POLYGON);
    for(int i = 0; i < 360; ++i){
        float degInRad = i * M_PI / 180;
        glVertex2f(circle.x + cos(degInRad) * circle.radius, circle.y + sin(degInRad) * circle.radius);
    }
    glEnd();
}

// Dibuja una linea
void drawLine(Line line){
    glColor3f(line.color[0], line.color[1], line.color[2]);
    glLineWidth(line.width);
    glBegin(GL_LINES);
    glVertex2f(line.x1, line.y1);
    glVertex2f(line.x2, line.y2);
    glEnd();
}

// Dibuja un cuadrado
void drawSquare(square square){
    glColor3f(square.color[0], square.color[1], square.color[2]);
    glBegin(GL_POLYGON);
    glVertex2f(square.x, square.y);
    glVertex2f(square.x + square.width, square.y);
    glVertex2f(square.x + square.width, square.y + square.height);
    glVertex2f(square.x, square.y + square.height);
    glEnd();
}

// Creamos 64 cuadrados para el tablero
square squares[64];

// Usamos los cuadrados para dibujar el tablero 32 de color piel y 32 de color cafe
void drawTable(){
    int x = -100;
    int y = -100;
    int width = 25;
    int height = 25;
    int color = 0;
    for(int i = 0; i < 64; i++){
        if(color == 0){
            squares[i].x = x;
            squares[i].y = y;
            squares[i].width = width;
            squares[i].height = height;
            squares[i].color[0] = 1;
            squares[i].color[1] = 0.8;
            squares[i].color[2] = 0.6;
            color = 1;
        }else{
            squares[i].x = x;
            squares[i].y = y;
            squares[i].width = width;
            squares[i].height = height;
            squares[i].color[0] = 0.5;
            squares[i].color[1] = 0.2;
            squares[i].color[2] = 0.00;
            color = 0;
        }
        x += 25;
        if(x == 100){
            x = -100;
            y += 25;
            if(color == 0){
                color = 1;
            }else{
                color = 0;
            }
        }
    }
    for(int i = 0; i < 64; i++){
        drawSquare(squares[i]);
    }
}

// Creamos 12 fichas de un color rojo pastel
Circle redCircles[12];
// Creamos 12 fichas de un color negro elegante
Circle blackCircles[12];


//8x8

/* //Funciones para lo del mouse
 float distance(float x1, float y1, float x2, float y2){
    return sqrt(pow(x1-x2,2) + pow(y1-y2,2));
}


void mouseCallback(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        for(auto& circle: circles){
            if(distance(circle.x, circle.y, (float) x, (float) y) <= circle.radius){
                circle.color[0] = 0;
                circle.color[1] = 1;
                circle.color[2] = 0;
            }
        }

    }
}
//en el main loop, como el keyboard
glutMouseFunc(mouseCallback);
 */


#endif //TAREA_DAMAS_GRAPHICS_H
