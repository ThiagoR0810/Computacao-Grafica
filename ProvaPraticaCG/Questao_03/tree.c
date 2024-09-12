#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include <stdio.h>
#include <math.h>

// Variáveis para FPS
char windowTitle[200];
int previousTime;
int framesInThisSecond = 0;

// Variável global para intensidade da luz
GLfloat lightIntensity = 1.0f;
const GLfloat maxIntensity = 1.0f;
const GLfloat minIntensity = 0.0f;
const GLfloat intensityStep = 0.1f;

// Variáveis de estado das luzes
GLboolean light1Enabled = GL_TRUE;
GLboolean light2Enabled = GL_TRUE;
GLboolean light3Enabled = GL_TRUE;

// Variáveis para a intensidade da neblina
GLfloat fogDensity = 0.02f;
const GLfloat maxFogDensity = 0.1f;
const GLfloat minFogDensity = 0.0f;
const GLfloat fogDensityStep = 0.01f;

int shadingMode = 1; // 1 para Gouraud (GL_SMOOTH), 0 para Flat (GL_FLAT)

// Função para inicializar o OpenGL
void initGL() {
    glClearColor(0.5f, 0.8f, 1.0f, 1.0f); // Cor de fundo (céu)
    glEnable(GL_DEPTH_TEST);              // Ativa o teste de profundidade
    glEnable(GL_LIGHTING);                // Ativa a iluminação
    glEnable(GL_LIGHT0);                  // Ativa a fonte de luz 0
    glEnable(GL_LIGHT1);                  // Ativa a fonte de luz 1 (spotlight)
    glEnable(GL_COLOR_MATERIAL);          // Ativa cores no material
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE); // Ajusta o material

    // Configuração da luz spotlight (GL_LIGHT1)
    GLfloat lightPos[] = { 0.0f, 4.0f, 0.0f, 1.0f };   // Posição da luz (acima da árvore)
    GLfloat spot1Direction[] = { 0.0f, -1.0f, 0.0f };   // Direção do feixe (para baixo)
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos);        // Define a posição da luz
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot1Direction); // Define a direção do spot
    //glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0f);         // Define o ângulo de cutoff (45 graus)
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 5.0f);        // Define o foco da luz (quanto menor, mais focada)
    glEnable(GL_LIGHT1); // Ativa a luz pontual

    // Configuração da luz pontual (GL_LIGHT2) - lado esquerdo
    GLfloat light2Pos[] = {-5.0f, 2.0f, 0.0f, 1.0f}; // Posição da luz (esquerda)
    glLightfv(GL_LIGHT2, GL_POSITION, light2Pos);
    glEnable(GL_LIGHT2); // Ativa a luz pontual

    // Configuração da luz direcional (GL_LIGHT2) - lado direito
    GLfloat light3Dir[] = {5.0f, -2.0f, 0.0f, 0.0f}; // Luz direcional da direita
    glLightfv(GL_LIGHT3, GL_POSITION, light3Dir);
    glEnable(GL_LIGHT3); // Ativa a luz direcional

    // Configuração padrão de sombreamento (Gouraud)
    glShadeModel(GL_SMOOTH);

    // Configuração da neblina
    glEnable(GL_FOG);                     // Ativa a neblina
    GLfloat fogColor[] = {0.5f, 0.8f, 1.0f, 1.0f}; // Cor da neblina (igual ao céu)
    glFogfv(GL_FOG_COLOR, fogColor);      // Define a cor da neblina
    glFogi(GL_FOG_MODE, GL_EXP2);         // Define o modo da neblina (EXP2)
    adjustFogDensity();                   // Ajusta a densidade inicial da neblina

    adjustLightIntensity(); // Inicializa a intensidade da luz
}

void adjustFogDensity() {
    glFogf(GL_FOG_DENSITY, fogDensity); // Ajusta a densidade da neblina
    glutPostRedisplay(); // Redesenha a tela
}

// Função para ajustar a intensidade da luz
void adjustLightIntensity() {
    GLfloat lightColor[] = { lightIntensity, lightIntensity, lightIntensity, 1.0f };

    // Ajusta a intensidade para todas as luzes
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, lightColor);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, lightColor);
}

// Função para configurar materiais
void setupMaterials(GLfloat ambient[], GLfloat diffuse[], GLfloat specular[], GLfloat shininess) {
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

// Função para configurar materiais específicos para tronco
void setupTrunkMaterials() {
    GLfloat trunk_ambient[] = {0.55f, 0.27f, 0.07f, 1.0f};
    GLfloat trunk_diffuse[] = {0.55f, 0.27f, 0.07f, 1.0f};
    GLfloat trunk_specular[] = {0.3f, 0.2f, 0.1f, 1.0f};
    GLfloat trunk_shininess = 10.0f; // Brilho
    setupMaterials(trunk_ambient, trunk_diffuse, trunk_specular, trunk_shininess);
}

// Função para configurar materiais específicos para folhas
void setupLeafMaterials() {
    GLfloat leaves_ambient[] = {0.0f, 0.6f, 0.0f, 1.0f};
    GLfloat leaves_diffuse[] = {0.0f, 0.6f, 0.0f, 1.0f};
    GLfloat leaves_specular[] = {0.1f, 0.4f, 0.1f, 1.0f};
    GLfloat leaves_shininess = 8.0f; // Brilho
    setupMaterials(leaves_ambient, leaves_diffuse, leaves_specular, leaves_shininess);
}

// Função para configurar materiais específicos para frutas
void setupFruitMaterials() {
    GLfloat fruit_ambient[] = {1.0f, 0.0f, 0.0f, 1.0f};
    GLfloat fruit_diffuse[] = {1.0f, 0.0f, 0.0f, 1.0f};
    GLfloat fruit_specular[] = {0.8f, 0.0f, 0.0f, 1.0f};
    GLfloat fruit_shininess = 10.0f; // Brilho
    setupMaterials(fruit_ambient, fruit_diffuse, fruit_specular, fruit_shininess);
}

// Função para configurar materiais específicos para o chão (grama)
void setupGroundMaterials() {
    GLfloat ground_ambient[] = {0.0f, 0.6f, 0.0f, 1.0f};
    GLfloat ground_diffuse[] = {0.0f, 0.6f, 0.0f, 1.0f};
    GLfloat ground_specular[] = {0.1f, 0.4f, 0.1f, 1.0f};
    GLfloat ground_shininess = 0.5f; // Brilho
    setupMaterials(ground_ambient, ground_diffuse, ground_specular, ground_shininess);
}

// Função para redimensionar a janela
void resize(int width, int height) {
    if (height == 0) height = 1;
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

// Função para desenhar o tronco (cubo)
void drawTrunk() {
    glPushMatrix();
    
    // Define a cor marrom para o tronco
    glColor3f(0.55f, 0.27f, 0.07f); 
    
    // Rotaciona o tronco em 90 graus no eixo X
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);  // Rotaciona ao redor do eixo X
    
    // Ajusta a posição do tronco
    glTranslatef(0.0f, 0.0f, -1.5f);  // Ajusta conforme necessário
    
    setupTrunkMaterials(); // Configura os materiais do tronco

    // Desenha o tronco da árvore (cilindro)
    GLdouble baseRadius = 0.2;
    GLdouble topRadius = 0.2;
    GLdouble height = 3.5;
    GLint slices = 32;
    GLint stacks = 32;

    glPolygonMode(GL_FRONT, GL_FILL);
    
    // Desenha o cilindro sólido com textura
    glutSolidCylinder(baseRadius, height, slices, stacks);
    
    glPopMatrix();
}

// Função para desenhar as folhas (esfera)
void drawLeaves(double radius, GLfloat x, GLfloat y, GLfloat z) {
    glPushMatrix();
    glColor3f(0.0f, 0.6f, 0.0f); // Verde
    glTranslatef(x, y, z); 
    setupLeafMaterials(); // Configura os materiais das folhas
    glutSolidSphere(radius, 20, 20);   // Esfera como as folhas
    glPopMatrix();
}

// Função para desenhar frutas (esferas pequenas)
void drawFruits(double radius, GLfloat x, GLfloat y, GLfloat z) {
    glPushMatrix();
    glColor3f(1.0f, 0.0f, 0.0f); // Vermelho
    glTranslatef(x, y, z); // Posiciona no topo do tronco
    setupFruitMaterials(); // Configura os materiais das frutas
    glutSolidSphere(radius, 20, 20);   // Esfera como as frutas
    glPopMatrix();
}

// Função para desenhar o chão (plano)
void drawGround() {
    glPushMatrix();
    glColor3f(0.0f, 0.6f, 0.0f); // Verde
    glTranslatef(0.0f, -1.1f, 0.0f);  // Coloca abaixo do tronco
    setupGroundMaterials(); // Configura os materiais do chão
    glScalef(5.0f, 0.1f, 5.0f);       // Escala para ser um chão grande
    glutSolidCube(1.0f);               // Plano representado por um cubo fino
    glPopMatrix();
}

// Função para desenhar a árvore
void drawTree() {
    glPushMatrix();
    drawTrunk();
    drawLeaves(0.3f, 0.0f, 1.5f, 0.0f);
    drawLeaves(0.3f, 0.0f, 1.5f, 0.5f);
    drawLeaves(0.3f, 0.5f, 1.5f, 0.0f);
    drawLeaves(0.3f, 0.0f, 1.5f, -0.5f);
    drawLeaves(0.3f, -0.5f, 1.5f, 0.0f);
    drawLeaves(0.3f, 0.3f, 1.5f, 0.3f);
    drawLeaves(0.3f, -0.3f, 1.5f, 0.3f);
    drawLeaves(0.3f, -0.3f, 1.5f, -0.3f);
    drawLeaves(0.3f, 0.3f, 1.5f, -0.3f);//
    drawLeaves(0.3f, 0.0f, 1.2f, 0.3f);
    drawLeaves(0.3f, 0.3f, 1.2f, 0.0f);
    drawLeaves(0.3f, 0.0f, 1.2f, -0.3f);
    drawLeaves(0.3f, -0.3f, 1.2f, 0.0f);
    drawLeaves(0.3f, 0.15f, 1.2f, 0.15f);
    drawLeaves(0.3f, -0.2f, 1.2f, 0.2f);
    drawLeaves(0.3f, -0.2f, 1.2f, -0.2f);
    drawLeaves(0.3f, 0.2f, 1.2f, -0.2f);//
    drawLeaves(0.3f, 0.0f, 1.7f, 0.3f);
    drawLeaves(0.3f, 0.3f, 1.7f, 0.0f);
    drawLeaves(0.3f, 0.0f, 1.7f, -0.3f);
    drawLeaves(0.3f, -0.3f, 1.7f, 0.0f);
    drawLeaves(0.3f, 0.15f, 1.7f, 0.15f);
    drawLeaves(0.3f, -0.2f, 1.7f, 0.2f);
    drawLeaves(0.3f, -0.2f, 1.7f, -0.2f);//
    drawLeaves(0.3f, 0.1f, 1.35f, 0.2f);
    drawLeaves(0.3f, 0.1f, 1.35f, -0.2f);
    drawLeaves(0.3f, 0.1f, 1.35f, 0.0f);
    drawLeaves(0.3f, 0.0f, 1.35f, 0.2f);//
    drawLeaves(0.3f, 0.0f, 1.8f, 0.0f);
    drawFruits(0.1, 0.25f, 1.25f, 0.5f);
    drawFruits(0.1, 0.25f, 1.25f, -0.5f);
    drawFruits(0.1, -0.25f, 1.25f, -0.5f);
    drawFruits(0.1, -0.25f, 1.25f, 0.5f);
    drawFruits(0.1, 0.5f, 1.25f, 0.25f);
    drawFruits(0.1, 0.5f, 1.25f, -0.25f);
    drawFruits(0.1, -0.5f, 1.25f, -0.25f);
    drawFruits(0.1, -0.5f, 1.25f, 0.25f);
    glPopMatrix();
}

// Função para exibir o a intensidade da luz
void displayWindowTitle() {
    sprintf(windowTitle, "3D Tree | Light Intensity: %.2f | Fog Density: %.2f", lightIntensity, fogDensity);
    glutSetWindowTitle(windowTitle);
}

// Função principal de desenho
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    gluLookAt(0.0, 2.5, 8.5, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0);

    drawTree();
    drawGround();

    glutSwapBuffers();
    displayWindowTitle(); // Atualiza o FPS

    // Atualiza o modo de sombreamento conforme o estado atual
    if (shadingMode == 1) {
        glShadeModel(GL_SMOOTH); // Gouraud
    } else {
        glShadeModel(GL_FLAT);   // Flat
    }
}

/// Função de teclado para aumentar/diminuir a intensidade da luz
void handleKeyboard(unsigned char key, int x, int y) {
    switch (key) {
        case '+':
            if (lightIntensity < maxIntensity) {
                lightIntensity += intensityStep;
                if (lightIntensity > maxIntensity) {
                    lightIntensity = maxIntensity;
                }
            }
            adjustLightIntensity(); // Atualiza a intensidade
            break;

        case '-':
            if (lightIntensity > minIntensity) {
                lightIntensity -= intensityStep;
                if (lightIntensity < minIntensity) {
                    lightIntensity = minIntensity;
                }
            }
            adjustLightIntensity(); // Atualiza a intensidade
            break;

        case '1':
            light1Enabled = !light1Enabled;
            if (light1Enabled) glEnable(GL_LIGHT1);
            else glDisable(GL_LIGHT1);
            break;
            
        case '2':
            light2Enabled = !light2Enabled;
            if (light2Enabled) glEnable(GL_LIGHT2);
            else glDisable(GL_LIGHT2);
            break;

        case '3':
            light3Enabled = !light3Enabled;
            if (light3Enabled) glEnable(GL_LIGHT3);
            else glDisable(GL_LIGHT3);
            break;
        
        case 'p': // Alternar sombreamento
            shadingMode = !shadingMode;
            if (shadingMode == 1) {
                glShadeModel(GL_SMOOTH); // Gouraud
            } else {
                glShadeModel(GL_FLAT);   // Flat
            }
            break;

        case 'n': // Aumenta a densidade da neblina
            if (fogDensity < maxFogDensity) {
                fogDensity += fogDensityStep;
                if (fogDensity > maxFogDensity) {
                    fogDensity = maxFogDensity;
                }
            }
            adjustFogDensity(); // Ajusta a intensidade da neblina
            break;

        case 'm': // Diminui a densidade da neblina
            if (fogDensity > minFogDensity) {
                fogDensity -= fogDensityStep;
                if (fogDensity < minFogDensity) {
                    fogDensity = minFogDensity;
                }
            }
            adjustFogDensity(); // Ajusta a intensidade da neblina
            break;

        default:
            break;
    }
    glutPostRedisplay(); // Atualiza a tela
}

// Função principal
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Tree");

    // Inicializa GLEW
    glewInit();
    
    // Inicializa OpenGL
    initGL();
    
    // Configurações de callback
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutKeyboardFunc(handleKeyboard); // Adiciona o callback de teclado

    // Inicia o loop principal do GLUT
    previousTime = glutGet(GLUT_ELAPSED_TIME);
    glutMainLoop();

    return 0;
}