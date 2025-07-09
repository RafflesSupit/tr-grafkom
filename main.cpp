#include <GL/freeglut.h>
#include <GL/glut.h>
#include <cmath>
#include <iostream>

#define M_PI 3.14

// Camera variables
float cameraX = 0.0f, cameraY = 5.0f, cameraZ = 15.0f;
float cameraAngleX = 0.0f, cameraAngleY = 0.0f;
float lastMouseX = 0, lastMouseY = 0;
bool mousePressed = false;

// Lighting and colors
void setColor(float r, float g, float b)
{
    glColor3f(r, g, b);
}

void setColorub(unsigned char r, unsigned char g, unsigned char b)
{
    glColor3ub(r, g, b);
}

void drawCube(float x, float y, float z, float width, float height, float depth)
{
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(width, height, depth);
    glutSolidCube(1.0f);
    glPopMatrix();
}

void drawQuad(float x1, float y1, float z1, float x2, float y2, float z2,
    float x3, float y3, float z3, float x4, float y4, float z4)
{
    glBegin(GL_QUADS);
    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y2, z2);
    glVertex3f(x3, y3, z3);
    glVertex3f(x4, y4, z4);
    glEnd();
}

void drawTriangle(float x1, float y1, float z1, float x2, float y2, float z2,
    float x3, float y3, float z3)
{
    glBegin(GL_TRIANGLES);
    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y2, z2);
    glVertex3f(x3, y3, z3);
    glEnd();
}

void drawCylinder(float radius, float height, int segments, float x, float y, float z)
{
    glPushMatrix();
    glTranslatef(x, y, z);

    // Draw the top and bottom
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, height / 2, 0);
    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * M_PI * float(i) / float(segments);
        glVertex3f(radius * cosf(angle), height / 2, radius * sinf(angle));
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, -height / 2, 0);
    for (int i = segments; i >= 0; i--) {
        float angle = 2.0f * M_PI * float(i) / float(segments);
        glVertex3f(radius * cosf(angle), -height / 2, radius * sinf(angle));
    }
    glEnd();

    // Draw the side
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * M_PI * float(i) / float(segments);
        glVertex3f(radius * cosf(angle), height / 2, radius * sinf(angle));
        glVertex3f(radius * cosf(angle), -height / 2, radius * sinf(angle));
    }
    glEnd();

    glPopMatrix();
}



void drawFence()
{
    // Rectangular wall base
    setColorub(150, 150, 150); // Gray concrete color
    float wallWidth = 28.0f;
    float wallDepth = 20.0f;
    float wallHeight = 1.0f;
    float wallY = 0.5f;

    // Draw the rectangular wall base (excluding front entrance area)
    drawCube(8.0f, wallY, 10.0f, 16.0f, wallHeight, 0.5f);     // Front wall right side
    drawCube(-4.0f, wallY, 10.0f, 16.0f, wallHeight, 0.5f);    // Front wall left side
    drawCube(2.0f, wallY, -10.0f, wallWidth, wallHeight, 0.5f); // Back wall
    drawCube(-12.0f, wallY, 0.0f, 0.5f, wallHeight, wallDepth); // Left wall
    drawCube(16.0f, wallY, 0.0f, 0.5f, wallHeight, wallDepth);  // Right wall

    // Entrance gate walls with name sign
    setColorub(120, 120, 120); // Darker gray for entrance walls
    drawCube(-2.0f, wallY + 1.0f, 10.0f, 0.8f, 3.0f, 0.6f);   // Left entrance pillar
    drawCube(6.0f, wallY + 1.0f, 10.0f, 0.8f, 3.0f, 0.6f);    // Right entrance pillar

    // Name sign wall
    setColorub(80, 80, 80); // Dark gray for sign background
    drawCube(2.0f, wallY + 1.5f, 10.2f, 7.0f, 1.5f, 0.3f);   // Sign background

    // Sign text on the dark gray wall
    glPushMatrix();
    glTranslatef(2.0f, wallY + 1.8f, 10.35f); // Position on the dark wall
    setColor(1.0f, 1.0f, 1.0f); // White text
    glScalef(0.006f, 0.006f, 0.006f);

    // First line: RUMAH DUKA
    glPushMatrix();
    glTranslatef(-400.0f, 0.0f, 0.0f);
    const char* text1 = "RUMAH DUKA";
    for (const char* c = text1; *c != '\0'; c++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
    }
    glPopMatrix();

    // Second line: GOTONG ROYONG SEJAHTERA
    glPushMatrix();
    glTranslatef(-300.0f, -150.0f, 0.0f);
    const char* text2 = "SEJAHTERA";
    for (const char* c = text2; *c != '\0'; c++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
    }
    glPopMatrix();

    glPopMatrix();

    // Iron fence - positioned ON TOP of concrete wall
    setColor(0.3f, 0.3f, 0.3f); // Dark gray for iron
    float fenceHeight = 2.0f;
    float postWidth = 0.2f;
    float postDepth = 0.2f;
    float railThickness = 0.05f;
    float fenceBaseY = wallY + wallHeight / 2.0f; // Start fence from top of wall

    float topY = fenceBaseY + fenceHeight - 0.05f;
    float midY = fenceBaseY + fenceHeight / 2.0f;
    float botY = fenceBaseY + 0.05f;
    float postY = fenceBaseY + fenceHeight / 2.0f;

    // Front side - right section (after entrance)
    for (float x = 8.0f; x <= 16.0f; x += 1.5f) {
        drawCube(x, postY, 10.0f, postWidth, fenceHeight, postDepth);
        if (x > 8.0f) {
            drawCube(x - 0.75f, topY, 10.0f, 1.5f, railThickness, railThickness);
            drawCube(x - 0.75f, midY, 10.0f, 1.5f, railThickness, railThickness);
            drawCube(x - 0.75f, botY, 10.0f, 1.5f, railThickness, railThickness);
        }
    }

    // Front side - left section (before entrance)
    for (float x = -12.0f; x <= -4.0f; x += 1.5f) {
        drawCube(x, postY, 10.0f, postWidth, fenceHeight, postDepth);
        if (x > -12.0f) {
            drawCube(x - 0.75f, topY, 10.0f, 1.5f, railThickness, railThickness);
            drawCube(x - 0.75f, midY, 10.0f, 1.5f, railThickness, railThickness);
            drawCube(x - 0.75f, botY, 10.0f, 1.5f, railThickness, railThickness);
        }
    }

    // Back side
    for (float x = -12.0f; x <= 16.0f; x += 1.5f) {
        drawCube(x, postY, -10.0f, postWidth, fenceHeight, postDepth);
        if (x > -12.0f) {
            drawCube(x - 0.75f, topY, -10.0f, 1.5f, railThickness, railThickness);
            drawCube(x - 0.75f, midY, -10.0f, 1.5f, railThickness, railThickness);
            drawCube(x - 0.75f, botY, -10.0f, 1.5f, railThickness, railThickness);
        }
    }

    // Left side
    for (float z = -10.0f; z <= 10.0f; z += 1.5f) {
        drawCube(-12.0f, postY, z, postWidth, fenceHeight, postDepth);
        if (z > -10.0f) {
            drawCube(-12.0f, topY, z - 0.75f, railThickness, railThickness, 1.5f);
            drawCube(-12.0f, midY, z - 0.75f, railThickness, railThickness, 1.5f);
            drawCube(-12.0f, botY, z - 0.75f, railThickness, railThickness, 1.5f);
        }
    }

    // Right side
    for (float z = -10.0f; z <= 10.0f; z += 1.5f) {
        drawCube(16.0f, postY, z, postWidth, fenceHeight, postDepth);
        if (z > -10.0f) {
            drawCube(16.0f, topY, z - 0.75f, railThickness, railThickness, 1.5f);
            drawCube(16.0f, midY, z - 0.75f, railThickness, railThickness, 1.5f);
            drawCube(16.0f, botY, z - 0.75f, railThickness, railThickness, 1.5f);
        }
    }

    // Entrance gate
    setColor(0.25f, 0.25f, 0.25f); // Slightly lighter for gate
    float gateWidth = 3.0f;
    float gateHeight = 1.8f;
    float gateY = fenceBaseY + gateHeight / 2.0f;

    // Gate frame
    drawCube(2.0f, gateY + 0.6f, 10.0f, gateWidth, railThickness, railThickness); // Top rail
    drawCube(2.0f, gateY, 10.0f, gateWidth, railThickness, railThickness);        // Middle rail
    drawCube(2.0f, gateY - 0.6f, 10.0f, gateWidth, railThickness, railThickness); // Bottom rail

    // Gate vertical bars
    for (float x = 0.5f; x <= 3.5f; x += 0.5f) {
        drawCube(x, gateY, 10.0f, 0.08f, gateHeight, 0.08f);
    }
}

void drawMainBuilding()
{
    setColorub(235, 218, 174);
    // security guard booth
    // left wall
    drawCube(-9.0f, 2.5f, 2.0f, 0.2f, 5.0f, 4.0f);
    // Back wall
    drawCube(-7.5f, 2.5f, 0.0f, 3.0f, 5.0f, 0.2f);
    // front wall
    drawCube(-7.5f, 4.0f, 4.0f, 3.0f, 2.0f, 0.2f);
    drawCube(-8.125f, 0.5f, 4.0f, 1.75f, 2.0f, 0.2f);

    // Main building walls (front wall removed to show interior)
    // Left wall
    drawCube(-6.0f, 2.5f, 0.0f, 0.2f, 5.0f, 8.0f);
    // Right wall
    drawCube(10.0f, 2.5f, 0.0f, 0.2f, 5.0f, 8.0f);
    // Back wall
    drawCube(2.0f, 2.5f, -4.0f, 16.0f, 5.0f, 0.2f);
    // front wall
    drawCube(2.0f, 4.0f, 4.0f, 16.0f, 2.0f, 0.2f);
}

void drawRoof()
{
    // Main roof structure - dark metal
    setColor(0.4f, 0.4f, 0.4f);

    // Sloped roof using triangular sections
    // Front slope
    drawQuad(-7.0f, 5.0f, 4.5f, 11.0f, 5.0f, 4.5f,
        11.0f, 7.0f, 0.0f, -7.0f, 7.0f, 0.0f);

    // Back slope
    drawQuad(-7.0f, 7.0f, 0.0f, 11.0f, 7.0f, 0.0f,
        11.0f, 5.0f, -4.5f, -7.0f, 5.0f, -4.5f);

    // Side triangles
    setColorub(235, 218, 174);
    drawTriangle(-6.0f, 5.0f, 4.5f, -6.0f, 7.0f, 0.0f, -6.0f, 5.0f, -4.5f);
    drawTriangle(10.0f, 5.0f, 4.5f, 10.0f, 5.0f, -4.5f, 10.0f, 7.0f, 0.0f);

    // Roof ridge
    setColor(0.3f, 0.3f, 0.3f);
    drawCube(2.0f, 7.0f, 0.0f, 18.2f, 0.2f, 0.3f);
}

void drawInterior()
{
    // Interior floor
    setColor(0.8f, 0.8f, 0.8f);
    drawCube(2.0f, 0.1f, 0.0f, 16.0f, 0.5f, 7.5f);
    drawCube(-7.5f, 0.1f, 2.0f, 3.0f, 0.5f, 4.0f);

    // stairs
    setColor(0.1f, 0.1f, 0.1f);
    drawCube(0.5f, 0.1f, 4.0f, 19.0f, 0.5f, 0.5f);
    drawCube(0.5f, 0.1f, 4.3f, 19.0f, 0.2f, 0.2f);

    // Interior walls/partitions
    setColor(0.9f, 0.9f, 0.9f);
    drawCube(-2.0f, 1.5f, -1.0f, 0.2f, 3.0f, 6.0f);
    drawCube(2.0f, 1.5f, -1.0f, 0.2f, 3.0f, 6.0f);
    drawCube(6.0f, 1.5f, -1.0f, 0.2f, 3.0f, 6.0f);
    drawCube(10.0f, 1.5f, -1.0f, 0.2f, 3.0f, 6.0f);

    // Ceiling
    setColor(0.95f, 0.95f, 0.95f);
    drawCube(2.0f, 3.8f, 0.0f, 16.0f, 0.2f, 7.5f);
}

void drawCanopy()
{
    setColor(0.5f, 0.5f, 0.5f);

    // left canopy
    // Canopy roof
    drawCube(-7.5f, 5.0f, 0.5f, 6.0f, 0.2f, 9.0f);

    // Support beams
    setColor(0.4f, 0.4f, 0.4f);
    drawCube(-7.5f, 4.9f, -3.5f, 6.0f, 0.2f, 0.3f);
    drawCube(-7.5f, 4.9f, -0.6f, 6.0f, 0.2f, 0.3f);

    // front canopy
    // Canopy roof
    drawCube(2.0f, 5.0f, 6.0f, 18.0f, 0.2f, 5.0f);

    // Support columns
    setColor(0.3f, 0.3f, 0.3f);
    drawCube(-6.0f, 2.5f, 4.0f, 0.3f, 5.0f, 0.3f);
    drawCube(-2.0f, 2.5f, 4.0f, 0.3f, 5.0f, 0.3f);
    drawCube(2.0f, 2.5f, 4.0f, 0.3f, 5.0f, 0.3f);
    drawCube(6.0f, 2.5f, 4.0f, 0.3f, 5.0f, 0.3f);
    drawCube(10.0f, 2.5f, 4.0f, 0.3f, 5.0f, 0.3f);

    // Support beams
    setColor(0.4f, 0.4f, 0.4f);
    drawCube(2.0f, 4.9f, 8.0f, 16.0f, 0.2f, 0.3f);

    drawCube(-6.0f, 4.9f, 6.0f, 0.3f, 0.2f, 5.0f);
    drawCube(-2.0f, 4.9f, 6.0f, 0.3f, 0.2f, 5.0f);
    drawCube(2.0f, 4.9f, 6.0f, 0.3f, 0.2f, 5.0f);
    drawCube(6.0f, 4.9f, 6.0f, 0.3f, 0.2f, 5.0f);
    drawCube(10.0f, 4.9f, 6.0f, 0.3f, 0.2f, 5.0f);
}

void drawPavedArea()
{
    // Paved courtyard
    setColor(0.14f, 0.14f, 0.14f);
    drawCube(0.5f, 0.05f, 3.0f, 23.0f, 0.1f, 18.0f);

    // Paving pattern lines
    setColor(0.15f, 0.15f, 0.15f);
    for (float i = -11; i <= 12; i += 0.5)
    {
        drawCube(i, 0.06f, 3.0f, 0.1f, 0.12f, 18.0f);
    }
    for (float j = -6; j <= 12; j += 0.5)
    {
        drawCube(0.5f, 0.06f, j, 23.0f, 0.12f, 0.1f);
    }
}

void drawEnvironment()
{
    // Ground plane
    setColor(0.3f, 0.6f, 0.3f);
    drawCube(0.0f, -0.5f, 0.0f, 50.0f, 1.0f, 50.0f);

    // Palm trees
    setColor(0.4f, 0.2f, 0.1f); // Brown trunk
    drawCube(-12.0f, 2.0f, -5.0f, 0.5f, 4.0f, 0.5f);
    drawCube(10.0f, 2.0f, -8.0f, 0.5f, 4.0f, 0.5f);

    setColor(0.1f, 0.7f, 0.1f); // Green leaves
    drawCube(-12.0f, 4.5f, -5.0f, 2.0f, 1.0f, 2.0f);
    drawCube(10.0f, 4.5f, -8.0f, 2.0f, 1.0f, 2.0f);
}

void drawScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0.0f, 10.0f, 40.0f, // Eye position
        0.0f, 0.0f, 0.0f,   // Look at position
        0.0f, 1.0f, 0.0f);  // Up vector

    glTranslatef(cameraX, cameraY, cameraZ);
    glRotatef(cameraAngleX, 1.0f, 0.0f, 0.0f);
    glRotatef(cameraAngleY, 0.0f, 1.0f, 0.0f);

    drawEnvironment();
    drawPavedArea();
    drawFence(); // Draw the fence before the main building
    drawMainBuilding();
    drawInterior();
    drawRoof();
    drawCanopy();

    glutSwapBuffers();
}

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)width / height, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'w':
        cameraZ -= 0.5f;
        break;
    case 's':
        cameraZ += 0.5f;
        break;
    case 'a':
        cameraX -= 0.5f;
        break;
    case 'd':
        cameraX += 0.5f;
        break;
    case 'q':
        cameraY += 0.5f;
        break;
    case 'e':
        cameraY -= 0.5f;
        break;
    case 27:
        exit(0);
        break; // ESC key
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            mousePressed = true;
            lastMouseX = x;
            lastMouseY = y;
        }
        else
        {
            mousePressed = false;
        }
    }
}

void mouseMotion(int x, int y)
{
    if (mousePressed)
    {
        cameraAngleY += (x - lastMouseX) * 0.5f;
        cameraAngleX += (y - lastMouseY) * 0.5f;

        // Clamp vertical rotation
        if (cameraAngleX > 90.0f)
            cameraAngleX = 90.0f;
        if (cameraAngleX < -90.0f)
            cameraAngleX = -90.0f;

        lastMouseX = x;
        lastMouseY = y;
        glutPostRedisplay();
    }
}

void setupLighting()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // Light position (simulating evening lighting)
    GLfloat lightPos[] = { 10.0f, 10.0f, 10.0f, 1.0f };
    GLfloat lightAmbient[] = { 0.13f, 0.1f, 0.1f, 0.3f };
    GLfloat lightDiffuse[] = { 0.8f, 0.8f, 0.7f, 0.2f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1024, 768);
    glutCreateWindow("Tugas Rancang AST - Tiong Ting Salatiga");

    glEnable(GL_DEPTH_TEST);
    setupLighting();

    glClearColor(0.6f, 0.7f, 0.9f, 1.0f);

    glutDisplayFunc(drawScene);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);

    std::cout << "Controls:" << std::endl;
    std::cout << "WASD - Move camera" << std::endl;
    std::cout << "Q/E - Move camera up/down" << std::endl;
    std::cout << "Mouse - Look around" << std::endl;
    std::cout << "ESC - Exit" << std::endl;

    glutMainLoop();
    return 0;
}

