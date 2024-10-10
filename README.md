### Trabalho de Programação II - Processamento de Imagens PGM
#### Feito por Eduarda Saibert
---

Problema: dado uma entrada de um diretório, obter a imagem mais similar ao input dado.

Entrada:
```bash
./LBP -d ./base -i img.pgm
```
ou
```bash
./LBP -i img.pgm -o img.pgm
```

---

### Arquivos

```bash
.
├── bin                 <-- Arquivos binários
├── descriptor          <-- Funções relacionadas à LBP
│   ├── descriptor.c
│   └── descriptor.h
├── includes.h          <-- Header principal
├── io                  <-- Funções relacinadas à entrada/saída e PGM
│   ├── io.c
│   └── io.h
├── lbp.c               <-- Main
├── lbp_img             <-- Imagens geradas por LBP
├── Makefile            <-- Makefile
└── README.md           === Documentação ===
```

---

### Funções Principais
### Main

Na função ```main```, o maior foco era processar os argumentos passados pela linha de comando. Assim, na primeira parte da função, apenas foi implementado um ```switch case``` iterando pelo ```argv[]```.

```c
    while ((next_option = getopt(argc, argv, "d:i:o:")) != -1)
    {
        switch (next_option)
        {
        case 'd':
            directory = malloc(strlen(optarg) + 1);
            ...
        case 'i':
            input = malloc(strlen(optarg) + 1);
            ...
        case 'o':
            ...
        ...
    }
```
Na segunda parte da função é encontrada as chamadas de funções que realizam as operações necessárias para se chegar ao resultado esperado. 

Do jeito que o programa foi estruturado, um comando fora de ordem ou do jeito ```./LBP -d ./base -i img.pgm -o img.pgm``` também funcionariam, pois não achei necessário limitar o uso nesses casos.

```c
   if (isPGM(input))
    {
        // Abre o arquivo PGM, caso exista
        if (!doesLpbExist(input))
        {
            pgm = openPGM(input);
            lbp = createLbp(pgm);

            ...

            createLbpFile(lbp);
        }
        // Se o diretório foi passado, abre o diretório e verifica os arquivos dentro dele
        if (directory)
        {
            ...

            openDirectory(input, directory);
            moreSimilar(input, directory);
        }
        // Se o arquivo de saída foi passado, cria a imagem após a transformação pela máscara LBP
        if (output)
        {
            createLbpImage(lbp, output);
        }
    }
```

---
### Leitura do PGM

Caso exista um diretório no comando, é necessário abrirmos o diretório em um primeiro momento, para então haver a leitura de cada PGM (função ```openDirectory(char *input, char* directory)```). Nota: se o LBP já existir, não é necessário a reeleitura do PGM.

Funções auxiliares importantes:
- ```isPGM(const char *f)```: verifica se o arquivo é PGM dado a extensão de seu path.
- ```isFileEmpty(FILE *file)```: verifica se o arquivo está vazio.
- ```getNameBeforeDot(char *f)```: dado um caminho f, retorna o caminho sem sua extensão.
- ```getNameAfterSlash(char *f)```: dado um caminho f, retorna apenas o nome do arquivo.
- ```createPath(char *name, char *prefix, char *sufix)```: cria um caminho ao receber um nome, um prefixo e um sufixo. Acredito que poderia ter sido mais bem utilizado no trabalho.
- ```removeLastSlash(char *f)```: dado um caminho f, retorna o caminho sem o último slash, nos casos de abertura de um diretório do tipo: ```./img/```.

Para a leitura do PGM foi estruturado uma ```struct``` tal como:
```c
// Estrutura de uma imagem PGM
typedef struct pgm PGM;
struct pgm {
    char type[3]; // P2 ou P5
    char *path; // Caminho ou nome do arquivo
    int **matrix; // Matriz de pixels
    int width; // Largura
    int height; // Altura
    int max_gray; // Máximo de tons de cinza
};
```
Como se pode observar, teve de ser definido na ```struct``` os dois casos possíveis de tipo de PGM (P2 ou P5). Assim, a leitura da matriz de pixels teve de ser adaptada para os dois casos:

```c
// Se é do tipo binário (P5)
if (strcmp(pgm->type, "P5") == 0)
{
    for (int i = 1; i < pgm->height + 1; i++)
        for (int j = 1; j < pgm->width + 1; j++)
            fread(&pgm->matrix[i][j], sizeof(unsigned char), 1, file);
}
// Se é do tipo ASCII (P2)
else if (strcmp(pgm->type, "P2") == 0)
{

    for (int i = 1; i < pgm->height + 1; i++)
        for (int j = 1; j < pgm->width + 1; j++)
            fscanf(file, "%d", &pgm->matrix[i][j]);
}
```
Além disso, também foi considerado a necessidade de um "padding" para a matriz, a fim de posteriormente facilitar o calculo dos casos de borda:

```c
for (int i = 0; i < pgm->height + 2; i++)
{
    pgm->matrix[i] = malloc((pgm->width + 2) * sizeof(int));
    ... 
    memset(pgm->matrix[i], 0, (pgm->width + 2) * sizeof(int)); // padding
}
```

---
### Criação da máscara e do histograma

Função auxiliar importante:
- ```doesLbpExist(char *f)```: dado um nome de arquivo f, checa se esse nome existe na pasta ```./bin```. Interessante para prevenir uso de recurso desnecessário.

A fim de criar a máscara e posteriormente o histograma, foi-se necessário criar também um objeto LBP. Note que, em comparação com o PGM, a estrutura LBP também possui um histograma.

```c
// Estrutura de um LBP
typedef struct lbp LBP;
struct lbp {
    char type[3]; // P2 ou P5
    char *path; // Caminho ou nome do arquivo
    int **matrix; // Matriz de pixels
    int *histogram; // ===Histograma===
    int width;  // Largura
    int height; // Altura
    int max_gray; // Máximo de tons de cinza
};
```
Após a criação desse objeto, é necessário o cálculo da máscara que fará a transformação da matriz do objeto PGM pela função ```createMask(PGM pgm)```. Isso foi feito se baseando na visão da máscara como um vetor, representando no seu início o vizinho superior esquerdo da instância da matriz ```matriz[i][j]```.

```c
{
    int l = 0;
    for (int k = j - 1; k <= j + 1; k++) // vizinhos da linha de cima
    {
        if (pgm->matrix[i - 1][k] > pgm->matrix[i][j])
            mask[l] = 1;
        else
            mask[l] = 0;
        l++;
    }

    if (pgm->matrix[i][j + 1] > pgm->matrix[i][j]) // vizinho direito da linha atual
        mask[l] = 1;
    else
        mask[l] = 0;
    l++;

    for (int k = j + 1; k >= j - 1; k--) // vizinhos da linha de baixo
    {

        if (pgm->matrix[i + 1][k] > pgm->matrix[i][j])
            mask[l] = 1;
        else
            mask[l] = 0;
        l++;
    }

    if (pgm->matrix[i][j - 1] > pgm->matrix[i][j]) // vizinho esquerdo da linha atual
        mask[l] = 1;
    else
        mask[l] = 0;
    l++;

    translateBinary(mask, binary, i - 1, j - 1);
}
```

Após isso, a máscara passa pela função ```translateBinary```que irá transformar os dados binários da atual ```mask[i]``` em um número decimal. E só então será possível a criação do histograma dentro do objeto:

```c
...
lbp->matrix = createMask(pgm);
lbp->histogram = createHistogram(lbp->height, lbp->width, lbp->matrix);
...
```

---
### Imagem mais semelhante

A fim de calcular a imagem mais semelhante, é necessário abrir o diretório passado mais uma vez (por conta dos casos em que não é preciso criar o PGM e um novo arquivo LBP).

Assim, são necessárias duas funções:
- ```eucDistance(char* path1, char* path2)```: retorna a distância euclidiana entre os dois arquivos, encontrados na pasta ```./bin```.
- ```moreSimilar(char* input, char* directory)```: realiza a iteração da função anterior para cada imagem do diretório e imprime na tela as informações da imagem com menor distância ao input fornecido.

---
### Output da imagem LBP
---

Para a criação da imagem transformada é preciso de apenas uma função: ```createLbpImage(LBP *lbp, char* output)```. Ela é responsável em abrir um novo arquivo pgm em ```./lbp_img``` com todas as informações do LBP (histograma, tipo, max_gray, altura, etc...).

