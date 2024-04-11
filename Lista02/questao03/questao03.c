#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <math.h>

#define MAX_LADOS 30
#define LARGURA_DO_MUNDO 400
#define ALTURA_DO_MUNDO 200


int listaGrama;
int listaParede;
int listaTelhado;
int listaPorta;
int listaJanela;
int listaSol;
int listaNuvem;
int listaTronco;
int listaFolhas;

// Inicia algumas variáveis de estado
void inicializa() {
    // cor para limpar a tela
    glClearColor(0, 0, 1, 1);      // azul
}

void desenhaCirculo(float raio, float x, float y, float z) {
    float angulo;

    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(x, y, z);
        for (int i = 0; i <= MAX_LADOS; ++i) {
            angulo = 2 * 3.14 * i / MAX_LADOS;
            glVertex3f(x + cos(angulo) * raio, y + sin(angulo) * raio, z);
        }
    glEnd();
}

void criaListaGrama() {
    listaGrama = glGenLists(1);
    glNewList(listaGrama, GL_COMPILE);  
        glBegin(GL_POLYGON);
            glVertex3f(-200, -10, 0);
            glVertex3f(200, -10, 0);
            glVertex3f(200, -100, 0);
            glVertex3f(-200, -100, 0);
        glEnd();
    glEndList();
}

void criaListaParede() {
    listaParede = glGenLists(1);
    glNewList(listaParede, GL_COMPILE);  
        glBegin(GL_POLYGON);
            glVertex3f(-70, -15, 0);
            glVertex3f(-70, 50, 0);
            glVertex3f(0, 50, 0);
            glVertex3f(0, -15, 0);
        glEnd();
    glEndList();
}

void criaListaTelhado() {
    listaTelhado = glGenLists(1);
    glNewList(listaTelhado, GL_COMPILE);  
        glBegin(GL_POLYGON);
            glVertex3f(-80, 50, 0);
            glVertex3f(-70, 70, 0);
            glVertex3f(0, 70, 0);
            glVertex3f(10, 50, 0);
        glEnd();
    glEndList();
}

void criaListaPorta() {
    listaPorta = glGenLists(1);
    glNewList(listaPorta, GL_COMPILE);  
        glBegin(GL_POLYGON);
            glVertex3f(-60, -13, 0);
            glVertex3f(-60, 15, 0);
            glVertex3f(-45, 15, 0);
            glVertex3f(-45, -13, 0);
        glEnd();
    glEndList();
}

void criaListaJanela() {
    listaJanela = glGenLists(1);
    glNewList(listaJanela, GL_COMPILE);  
        glBegin(GL_POLYGON);
            glVertex3f(-30, 20, 0);
            glVertex3f(-30, 35, 0);
            glVertex3f(-10, 35, 0);
            glVertex3f(-10, 20, 0);
        glEnd();
    glEndList();
}

void criaListaSol() {
    listaSol = glGenLists(5);
    glNewList(listaSol, GL_COMPILE);  
        desenhaCirculo(15, -150, 80, 0);
    glEndList();
}

void criaListaNuvem() {
    listaNuvem = glGenLists(1);
    glNewList(listaNuvem, GL_COMPILE);  
        desenhaCirculo(10, 50, 75, 0);  
        desenhaCirculo(13, 65, 75, 0);  
        desenhaCirculo(9, 80, 75, 0);
    glEndList();
}

void criaListaTronco() {
    listaTronco = glGenLists(1);
    glNewList(listaTronco, GL_COMPILE);  
        glBegin(GL_POLYGON);
            glVertex3f(140, -30, 0);
            glVertex3f(140, 40, 0);
            glVertex3f(160, 40, 0);
            glVertex3f(160, -30, 0);
        glEnd();
    glEndList();
}

void criaListaFolhas() {
    listaFolhas = glGenLists(1);
    glNewList(listaFolhas, GL_COMPILE);  
        desenhaCirculo(20, 140, 40, 0);  
        desenhaCirculo(20, 130, 45, 0);  
        desenhaCirculo(20, 140, 50, 0);  
        desenhaCirculo(20, 150, 55, 0);  
        desenhaCirculo(20, 170, 45, 0);  
        desenhaCirculo(20, 160, 35, 0);
    glEndList();
}

void desenhaCena() {
    // Limpa a tela (com a cor definida por glClearColor(r,g,b)) para que
    // possamos desenhar
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0, 1, 0);       // verde
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glCallList(listaGrama);    // chama a lista

    glColor3f(1, 0, 0);       // vermelho
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glCallList(listaParede);    // chama a lista

    glColor3f(1, 0.55, 0.01);       // Laranja
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glCallList(listaTelhado);    // chama a lista

    glColor3f(0.37, 0.16, 0.01);       // Marrom
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glCallList(listaPorta);    // chama a lista

    glColor3f(0.61, 0.60, 0.59);       // Cinza
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glCallList(listaJanela);    // chama a lista

    glColor3f(1, 0.92, 0.01);       // Amarelo
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glCallList(listaSol);    // chama a lista

    glColor3f(1, 1, 1);       // Branco
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glCallList(listaNuvem);    // chama a lista

    glColor3f(0.37, 0.16, 0.01);       // Marrom
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glCallList(listaTronco);    // chama a lista

    glColor3f(0.11, 0.39, 0.02);       // Verde escuro
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glCallList(listaFolhas);    // chama a lista

    // Diz ao OpenGL para colocar o que desenhamos na tela
    glutSwapBuffers();
}

// Callback de redimensionamento
void redimensiona(int w, int h) {
    glViewport(0,0,w,h);

    float razaoAspecto = (float) glutGet(GLUT_WINDOW_WIDTH) / (float) glutGet(GLUT_WINDOW_HEIGHT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glFrustum(razaoAspecto, razaoAspecto, -100, 100, -1, 1);
    glOrtho(-100*razaoAspecto, 100*razaoAspecto, -100, 100, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


int main(int argc, char **argv) {
    // Configuração inicial da janela do GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGBA);
    glutInitWindowSize(400, 200);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("Paisagem");
    inicializa();

    glutDisplayFunc(desenhaCena);
    glutReshapeFunc(redimensiona);

    criaListaGrama();
    criaListaParede();
    criaListaTelhado();
    criaListaPorta();
    criaListaJanela();
    criaListaSol();
    criaListaNuvem();
    criaListaTronco();
    criaListaFolhas();

    glutMainLoop();
    return 0;
}