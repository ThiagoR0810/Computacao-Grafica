
#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "texto.h"

#define LARGURA_DO_MUNDO 800
#define ALTURA_DO_MUNDO 600

struct ponto {
    float x, y;
};
struct tecla {
    unsigned char tecla;
    float x, y;
};
struct mouse {
    int x, y;
    int leftButtonDown, rightButtondown;
};

struct ponto posicaoMouse; // posicao do mouse em seu movimento passivo na tela 
struct ponto posicaoAviao;
struct tecla teclaPressionada;
struct mouse mousePressionado;

int listaNome;
int listaFundo;
int listaAviao;
int listaCirculo;
int listaQuadrado;

GLuint idTexturaFundo;
GLuint idTexturaAviao;

GLuint carregaTextura(const char* arquivo) {
    GLuint idTextura = SOIL_load_OGL_texture(
                           arquivo,
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );

    if (idTextura == 0) {
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    }

    return idTextura;
}

void desenhaCirculo(float raio, float x, float y, float z) {
    float angulo;

    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(x, y, z);
        for (int i = 0; i <= 30; ++i) {
            angulo = 2 * 3.14 * i / 30;
            glVertex3f(x + cos(angulo) * raio, y + sin(angulo) * raio, z);
        }
    glEnd();
}

void escreveTexto(void * font, char *s, float x, float y, float z) {
    int i;
    glRasterPos3f(x, y, z);

    for (i = 0; i < strlen(s); i++) {
        glutBitmapCharacter(font, s[i]);
    }
}

void criaListaNome() {
    listaNome = glGenLists(1);
    glNewList(listaNome, GL_COMPILE);
        escreveTexto(GLUT_BITMAP_HELVETICA_18, "Thiago Ribeiro Correa", -1.8, 1.8, 0); // escreve na tela meu nome
    glEndList();
}

void criaListaFundo() {
    listaFundo = glGenLists(1);
    glNewList(listaFundo, GL_COMPILE);
        // Habilita o uso de texturas
        glEnable(GL_TEXTURE_2D);

        // Começa a usar a textura que criamos
        glBindTexture(GL_TEXTURE_2D, idTexturaFundo);
        glBegin(GL_TRIANGLE_FAN);
            // Associamos um canto da textura para cada vértice
            glTexCoord2f(0, 0);
            glVertex3f(-2, -2,  -1);

            glTexCoord2f(1, 0);
            glVertex3f( 2, -2,  -1);

            glTexCoord2f(1, 1);
            glVertex3f( 2,  2,  -1);

            glTexCoord2f(0, 1);
            glVertex3f(-2,  2,  -1);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    glEndList();
}

void criaListaAviao() {
    listaAviao = glGenLists(1);
    glNewList(listaAviao, GL_COMPILE);
        // Habilita o uso de texturas
        glEnable(GL_TEXTURE_2D);

        // Começa a usar a textura que criamos
        glBindTexture(GL_TEXTURE_2D, idTexturaAviao);
        glBegin(GL_TRIANGLE_FAN);
            // Associamos um canto da textura para cada vértice
            glTexCoord2f(0.4, 0.4);
            glVertex3f(-0.5, 0.5, 0);

            glTexCoord2f(0.6, 0.4);
            glVertex3f(0.5, 0.5, 0);

            glTexCoord2f(0.6, 0.2);
            glVertex3f(0.5, -0.5, 0);

            glTexCoord2f(0.4, 0.2);
            glVertex3f(-0.5, -0.5, 0);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    glEndList();
}

void criaListaCirculo() {
    listaCirculo = glGenLists(1);
    glNewList(listaCirculo, GL_COMPILE);  
        desenhaCirculo(100, mousePressionado.x, mousePressionado.y, 1);
    glEndList();
}

void criaListaQuadrado() {
    listaQuadrado = glGenLists(1);
    glNewList(listaCirculo, GL_COMPILE);  
        glBegin(GL_POLYGON);
            glVertex3f(mousePressionado.x - 50, mousePressionado.y + 50, 1);
            glVertex3f(mousePressionado.x + 50, mousePressionado.y + 50, 1);
            glVertex3f(mousePressionado.x + 50, mousePressionado.y - 50, 1);
            glVertex3f(mousePressionado.x - 50, mousePressionado.y - 50, 1);
        glEnd();
    glEndList();
}

void inicializa() {
    glClearColor(1, 1, 1, 1);

    // habilita mesclagem de cores, para termos suporte a texturas
    // com transparência
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    idTexturaAviao = carregaTextura("avioes.png");
    //idTexturaFundo = carregaTextura("fundo.png");
}


void desenha() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f (1, 1, 1);

    //glCallList(listaFundo); // chama a lista da textura de fundo

    glCallList(listaAviao);

    glColor3f(1, 0, 0); // escreve na tela com a cor preta
    glCallList(listaNome);    // chama a lista

    glutSwapBuffers();
}

// Callback de redimensionamento
void redimensiona(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2, 2, -2, 2, -1, 1);

    float razaoAspectoJanela = ((float)width)/height;
    float razaoAspectoMundo = ((float) LARGURA_DO_MUNDO)/ ALTURA_DO_MUNDO;

    // se a janela está menos larga do que o mundo (16:9)...
    if (razaoAspectoJanela < razaoAspectoMundo) {
        // vamos colocar barras verticais (acima e abaixo)
        float hViewport = width / razaoAspectoMundo;
        float yViewport = (height - hViewport)/2;
        glViewport(0, yViewport, width, hViewport);
    }
    // se a janela está mais larga (achatada) do que o mundo (16:9)...
    else if (razaoAspectoJanela > razaoAspectoMundo) {
        // vamos colocar barras horizontais (esquerda e direita)
        float wViewport = ((float)height) * razaoAspectoMundo;
        float xViewport = (width - wViewport)/2;
        glViewport(xViewport, 0, wViewport, height);
    } else {
        glViewport(0, 0, width, height);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Função para lidar com o clique do mouse
void cliqueMouseBotao(int botao, int estado, int x, int y) {
    char *estadoTexto = (estado == GLUT_DOWN) ? "pressionado" : "liberado";
    mousePressionado.leftButtonDown = (botao == 0 && estado == GLUT_DOWN) ? 1 : 0;
    mousePressionado.rightButtondown = (botao == 2 && estado == GLUT_DOWN) ? 1 : 0;
    mousePressionado.x = x;
    mousePressionado.y = y;
    char evento[100];
    snprintf(evento, sizeof(evento), "Botão %d %s, Posição do mouse: (%d, %d)", botao, estadoTexto, x, y);
    if (botao == 0 && estado == GLUT_DOWN) {
        printf("chamou@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
        criaListaCirculo();
    }
    printf("%s\n", evento);
    //registrarEvento(evento);
}

// Função para lidar com o movimento do mouse
void movimentoMouse(int x, int y) {
    char evento[100];
    snprintf(evento, sizeof(evento), "Posição do mouse: (%d, %d)", x, y);
    printf("%s\n", evento);
    //registrarEvento(evento);
}

// Callback de evento de teclado
void teclado(unsigned char key, int x, int y) {
    switch(key) {
        // Tecla ESC
        case 27:
            exit(0);
            break;
        case 'w':

        case 'a':

        case 's':

        case 'd':

        default:
            teclaPressionada.x = x;
            teclaPressionada.y = y;
            teclaPressionada.tecla = key;
            break;
   }
}

void atualizaCena(int periodo) {
    // Pede ao GLUT para redesenhar a tela, assim que possível
    glutPostRedisplay();

    // Se registra novamente, para que fique sempre sendo chamada (30 FPS)
    glutTimerFunc(periodo, atualizaCena, periodo);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(400, 100);

    glutCreateWindow("Carregando textura com SOIL");

    inicializa();

    glutDisplayFunc(desenha);
    glutReshapeFunc(redimensiona);
    glutKeyboardFunc(teclado);
    glutMouseFunc(cliqueMouseBotao);
    glutPassiveMotionFunc(movimentoMouse);
    // Registra a função "atualiza" para executar daqui a 0 milissegundos
    glutTimerFunc(0, atualizaCena, 33);

    criaListaFundo();
    criaListaNome();
    criaListaAviao();
    criaListaCirculo();
    criaListaQuadrado();

    glutMainLoop();

    return 0;
}
