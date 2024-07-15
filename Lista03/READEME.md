Lista 03 - Computação Gráfica

Crie um projeto em C/C++ com OpenGL, freeglut e SOIL que simule uma cena de um jogo de avião 2D, de modo que:

  - Tenha modelado no fundo um cenário estático top-down com objetos geométricos (ver  para se inspirar). Use lista de apresentação para tal.
  - Tenha o seu nome completo, sem acento e caracteres especiais escrito no topo da cena.
  - Tenha um avião modelado como um retângulo, que se movimente para cima, esquerda, baixo e direita usando as teclas w, a, s, d.
        - Deve-se usar um dos modelos de avião do arquivo avioes.png.
        - Deve-se trocar a exibição do recorte da sprite de acordo com a direção acionada (para a esquerda, direita e parado).
  - Quando for clicado e segurado o clique do botão esquerdo do mouse em uma posição da tela,
    deve-se desenhar um círculo cujo centro é a posição do mouse e o valor do raio é 100. 
    Ao largar o clique do mouse, o circulo deve desaparecer. Escolha a cor de preferência. 
    Faça a mesma coisa para o clique do botão direito do mouse, só que para desenhar um quadrado de lado 100. A posição do clique será o centro do quadrado.
  - Corrija a visualização de cena quando a janela for redimensionada.
  - A tecla esc deve finalizar a aplicação.
  - Crie um arquivo makefile com as cláusulas de compilação, execução e limpeza de arquivos auxiliares.
  - Compacte o projeto como zip, sem arquivos executáveis e intermediários, e submeta a lista 3.
