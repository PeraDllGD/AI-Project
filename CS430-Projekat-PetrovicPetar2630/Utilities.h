#pragma once

#include <stdlib.h>
#include <GL\glut.h>
#include <cmath>
#include "Vector2D.h"

const double DEG2RAD = 3.14159 / 180;

struct Color {
    float r;
    float g;
    float b;

    Color(float r, float g, float b) : r(r), g(g), b(b) {}

    Color(): r(0), g(0), b(0) {};
};

inline void drawCircle(double radius, Vector2D<double> pos) {
    glBegin(GL_LINE_LOOP);

    for (int i = 0; i < 360; i++) {
        double degInRad = i * DEG2RAD;
        glVertex2d(cos(degInRad) * radius + pos.x, sin(degInRad) * radius + pos.y);
    }

    glEnd();
}

inline void drawText(const char* text, double x, double y, Color color) {
    const char* c;
    glPushMatrix();
    glTranslated(x, y, 0);
    glColor3f(color.r, color.g, color.b);
    glScalef(0.1, 0.1, 1);
    for (c = text; *c != '\0'; c++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
    }
    glPopMatrix();
}

