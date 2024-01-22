//Mateus Freitas Cintra - N°USP 14597677
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_FILENAME 256

typedef struct {
    int **matrix;
    int *count1;
    int *count0;
} Imagem;

int verificarMatriz(Imagem *img, int l, int c, int d, int v) {
  if (d == 1) {
    return (img->matrix[l][c] == v);
  } else if (d == 2) {
    return (img->matrix[l][c] == v && img->matrix[l][c + 1] == v &&
            img->matrix[l + 1][c] == v && img->matrix[l + 1][c + 1] == v);
  } else {
    int half = d / 2;
    return (verificarMatriz(img, l, c, half, v) && verificarMatriz(img, l, c + half, half, v) &&
            verificarMatriz(img, l + half, c, half, v) &&
            verificarMatriz(img, l + half, c + half, half, v));
  }
}

void quad(Imagem *img, int l, int c, int n, int *count1, int *count0) {
  if (n == 1) {
    if (verificarMatriz(img, l, c, n, 1)) {
      count1[0]++;
    } else if (verificarMatriz(img, l, c, n, 0)) {
      count0[0]++;
    }
  } else {
    int half = n / 2;
    if (verificarMatriz(img, l, c, n, 1)) {
      count1[n - 1]++;
    } else if (verificarMatriz(img, l, c, n, 0)) {
      count0[n - 1]++;
    } else {
      quad(img, l, c, half, count1, count0);
      quad(img, l, c + half, half, count1, count0);
      quad(img, l + half, c, half, count1, count0);
      quad(img, l + half, c + half, half, count1, count0);
    }
  }
}

void processarImagem(int n, char *filename) {
  FILE *arquivo = fopen(filename, "r");
  if (arquivo == NULL) {
    fprintf(stderr, "Erro ao abrir o arquivo");
    exit(-1);
  }

  Imagem img;
  img.matrix = (int **)malloc(n * sizeof(int *));
  for (int i = 0; i < n; i++) {
    img.matrix[i] = (int *)malloc(n * sizeof(int));
    for (int j = 0; j < n; j++) {
      fscanf(arquivo, "%1d", &img.matrix[i][j]);
    }
  }
  fclose(arquivo);

  img.count1 = (int *)calloc(n, sizeof(int));
  img.count0 = (int *)calloc(n, sizeof(int));

  quad(&img, 0, 0, n, img.count1, img.count0);

  FILE *arquivoSaida = fopen("saida.txt", "w");
  if (arquivoSaida == NULL) {
    perror("Erro ao abrir o arquivo");
    exit(1);
  }

  for (int i = n; i >= 1; i = i / 2) {
    fprintf(arquivoSaida, "%dx%d %d %d", i, i, img.count1[i - 1],
            img.count0[i - 1]);
    if (i != 1) {
      fprintf(arquivoSaida, "\n");
    }
  }
  fclose(arquivoSaida);
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Este programa exige dois argumentos: (1) um número inteiro N (potência de 2 e no máximo 1024, a dimensão da matriz que representa a imagem); (2) o nome do arquivo de entrada.\n");
    exit(1);
  }

  int n = atoi(argv[1]);
  char nomeArquivo[MAX_FILENAME] = "";
  strcpy(nomeArquivo, argv[2]);

  processarImagem(n, nomeArquivo);

  return 0;
}
