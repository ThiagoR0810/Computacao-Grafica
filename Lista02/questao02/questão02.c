#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <math.h>


int listaParede;
int listaTelhado;
int listaPorta;
int listaJanela;

// Inicia algumas variáveis de estado
void inicializa() {
    // cor para limpar a tela
    glClearColor(1, 1, 1, 1);      // branco
}

void criaListaParede() {
    listaParede = glGenLists(1);
    glNewList(listaParede, GL_COMPILE);  
        glBegin(GL_POLYGON);
            glVertex3f(-.70, -.15, 0);
            glVertex3f(-.70, .50, 0);
            glVertex3f(0, .50, 0);
            glVertex3f(0, -.15, 0);
        glEnd();
    glEndList();
}

void criaListaTelhado() {
    listaTelhado = glGenLists(1);
    glNewList(listaTelhado, GL_COMPILE);  
        glBegin(GL_POLYGON);
            glVertex3f(-.80, .50, 0);
            glVertex3f(-.70, .70, 0);
            glVertex3f(0, .70, 0);
            glVertex3f(.10, .50, 0);
        glEnd();
    glEndList();
}

void criaListaPorta() {
    listaPorta = glGenLists(1);
    glNewList(listaPorta, GL_COMPILE);  
        glBegin(GL_POLYGON);
            glVertex3f(-.60, -.13, 0);
            glVertex3f(-.60, .15, 0);
            glVertex3f(-.45, .15, 0);
            glVertex3f(-.45, -.13, 0);
        glEnd();
    glEndList();
}

void criaListaJanela() {
    listaJanela = glGenLists(1);
    glNewList(listaJanela, GL_COMPILE);  
        glBegin(GL_POLYGON);
            glVertex3f(-.30, .20, 0);
            glVertex3f(-.30, .35, 0);
            glVertex3f(-.10, .35, 0);
            glVertex3f(-.10, .20, 0);
        glEnd();
    glEndList();
}

void desenhaCena() {
    // Limpa a tela (com a cor definida por glClearColor(r,g,b)) para que
    // possamos desenhar
    glClear(GL_COLOR_BUFFER_BIT);

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

    // Diz ao OpenGL para colocar o que desenhamos na tela
    glutSwapBuffers();
}

// Callback de redimensionamento
void redimensiona(int w, int h) {
    glViewport(0,0,w,h);

    float razaoAspecto = (float) glutGet(GLUT_WINDOW_WIDTH) / (float) glutGet(GLUT_WINDOW_HEIGHT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1*razaoAspecto, 1*razaoAspecto, -1, 1, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


int main(int argc, char **argv) {
    // Configuração inicial da janela do GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGBA);
    glutInitWindowSize(400, 200);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("Casa");
    inicializa();

    glutDisplayFunc(desenhaCena);
    glutReshapeFunc(redimensiona);

    criaListaParede();
    criaListaTelhado();
    criaListaPorta();
    criaListaJanela();

    glutMainLoop();
    return 0;
}