#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#ifndef _TEXTO_H_
#define _TEXTO_H_

void escreveTexto(void *, char *, float, float, float);

#endif  // _TEXTO_H_

#define MAX_LADOS 30
#define LARGURA_DO_MUNDO 400
#define ALTURA_DO_MUNDO 200

struct ponto {
    float x, y;
};

struct ponto posicaoMouse;


int largura;
int altura;

void escreveTexto(void * font, char *s, float x, float y, float z) {
    int i;
    glRasterPos3f(x, y, z);

    for (i = 0; i < strlen(s); i++) {
        glutBitmapCharacter(font, s[i]);
    }
}

// Inicia algumas variáveis de estado
void inicializa() {
    // cor para limpar a tela
    glClearColor(1, 1, 1, 1);      // branco
}


void desenhaCena() {
    // Limpa a tela (com a cor definida por glClearColor(r,g,b)) para que
    // possamos desenhar
    glClear(GL_COLOR_BUFFER_BIT);

    char texto01[100] = "";
    char texto02[100] = "";

    largura = glutGet(GLUT_WINDOW_WIDTH);
    altura = glutGet(GLUT_WINDOW_HEIGHT);

    snprintf(texto01, sizeof(texto01), "largura da janela = %d; altura da janela = %d", largura, altura); // converte largura e altura em caracteres
    // trasnforma as coordenadas do mouse em caracteres
    snprintf(texto02, sizeof(texto02), "coordenada x do mouse = %.0f; coordenada y do mouse = %.0f", posicaoMouse.x, posicaoMouse.y); 

     glColor3f(0, 0, 0); // escreve na tela com a cor preta
    escreveTexto(GLUT_BITMAP_HELVETICA_18, texto01, 5, 120, 0); // escreve na tela largura e altura da janela
    printf("Largura = %d; Altura = %d\n", largura, altura); // escreve no terminal largura e altura da janela


    glColor3f(1, 0, 0); // escreve na tela com a cor vermelha
    escreveTexto(GLUT_BITMAP_HELVETICA_18, texto02, 5, 100, 0); // escreve na tela as coordenadas do mouse
    printf("Coordenada x do mouse = %.0f; Coordenada y do mouse = %.0f\n", posicaoMouse.x, posicaoMouse.y); // escreve no terminal as coordenadas do mouse

    // Diz ao OpenGL para colocar o que desenhamos na tela
    glutSwapBuffers();
}

// Callback de redimensionamento
void redimensiona(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 200, 0, 200, -1, 1);

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

// Callback de evento de movimento do mouse
void movimentoMouse(int x, int y) {
    posicaoMouse.x = x;
    posicaoMouse.y = y;
}

void atualizaCena(int periodo) {
    // Pede ao GLUT para redesenhar a tela, assim que possível
    glutPostRedisplay();

    // Se registra novamente, para que fique sempre sendo chamada (30 FPS)
    glutTimerFunc(periodo, atualizaCena, periodo);
}


int main(int argc, char **argv) {
    // Configuração inicial da janela do GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGBA);
    glutInitWindowSize(600, 250);
    glutInitWindowPosition(150, 80);

    glutCreateWindow("Questao 01");
    inicializa();

    glutDisplayFunc(desenhaCena);
    glutReshapeFunc(redimensiona);

    // Registra a função "movimentoMouse" para executar sempre que o mouse mexer
    glutPassiveMotionFunc(movimentoMouse);

    // Registra a função "atualiza" para executar daqui a 0 milissegundos
    glutTimerFunc(0, atualizaCena, 33);

    glutMainLoop();
    return 0;
}