# Monty-Plython

Visualizador de arquivos *.ply*


### Objetivos 

1. Implementar um visualizador de objetos 3D:
   - Construa uma estrutura de dados para representação de coordenadas, vértices e faces da estrutura.
   - Implemente um leitor de arquivos do formato ply (http://en.wikipedia.org/wiki/PLY_%28file_format%29 e http://paulbourke.net/dataformats/ply/ ) para fazer a leitura dos objetos.
   - O sistema deve possibilitar a manipulação por parte do usuário através de rotação, translação e escala do objeto na cena.

2. Incluir 2 luzes na cena gráfica com iluminação difusa e especular. Permita que o usuário modifique a posição e as cores das luzes na cena.

### Manual

>Configurei algumas tarefas pelo **vscode** para facilitar o uso.

Execute as tarefas nessa ordem:

1. cmake
2. make

Esta ultima deve gerar o arquivo **MontyPlython** na pasta *Build* (crie se ela não existir) e logo você poderá executar-lo com           
        
    ./Build/MontyPlython some.ply

>Isso mesmo, só funciona em sistemas operacionais linux.

Isso plotará o objeto 3d e duas luzes (1ª direita e 2ª esquerda).

Os controles são:

- Esc: Sair da tela cheia. (Começa em tela cheia)
- Enter: Tela cheia.
- r ou R: reseta valores.
- i ou I: zoom in.
- o ou O: zoom out.
- q: Decrementa eixo X da luz 1.
- Q: Incrementa eixo X da luz 1.
- w: Decrementa eixo Y da luz 1.
- W: Incrementa eixo Y da luz 1.
- e: Decrementa eixo Z da luz 1.
- E: Incrementa eixo Z da luz 1.
- a: Decrementa eixo X da luz 2.
- A: Incrementa eixo X da luz 2.
- s: Decrementa eixo Y da luz 2.
- S: Incrementa eixo Y da luz 2.
- d: Decrementa eixo Z da luz 2.
- D: Incrementa eixo Z da luz 2.
- t: Decrementa eixo X da translação.
- T: Incrementa eixo X da translação.
- y: Decrementa eixo Y da translação.
- Y: Incrementa eixo Y da translação.
- u: Decrementa eixo Z da translação.
- U: Incrementa eixo Z da translação.
- g: Decrementa eixo X da rotação.
- G: Incrementa eixo X da rotação.
- h: Decrementa eixo Y da rotação.
- H: Incrementa eixo Y da rotação.
- j: Decrementa eixo Z da rotação.
- J: Incrementa eixo Z da rotação.
- b: Decrementa eixo X da escala.
- B: Incrementa eixo X da escala.
- n: Decrementa eixo Y da escala.
- N: Incrementa eixo Y da escala.
- m: Decrementa eixo Z da escala.
- M: Incrementa eixo Z da escala.
- Clicar com botão esquerdo do mouse e arrastar move a imagem em seu eixo.
- Clicar com botão direito do mouse abre menu com opções.


### Referências

- [PLY-and-Lighting-In-OpenGL](https://github.com/tarun1325/PLY-and-Lighting-In-OpenGL)
- [ANSI C Library for PLY file format input and output](http://w3.impa.br/~diego/software/rply/)
