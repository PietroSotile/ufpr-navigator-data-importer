#ifndef CSV_PARSER_H
#define CSV_PARSER_H
#include <stdio.h>

#define BUFFER_SIZE 32
#define LINE_SIZE 64

typedef struct {
    char **fields;
    int count;
} CsvRow;

/**
 * Lê a linha de um arquivo e aloca memória dinâmicamente um buffer para armazena-la
 * Retorno: tamanho de char lidos, ou -1 se erro
 */
int read_line(FILE *file, char **line);

/**
 *  Recebe uma string de valores separados por vírgula 
 * e a divide em subconjuntos em cada vírgula que aparece
 * Retorno: CsvRow * contendo os campos separados em caso de sucesso, NULL se erro
 */
CsvRow *csv_parse_line(const char *line);

/**
 * Desaloca a memória utilizada por *row e campos derivados
 */
void csv_row_free(CsvRow *row);

#endif