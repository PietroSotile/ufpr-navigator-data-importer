#ifndef CSV_PARSER_H
#define CSV_PARSER_H

typedef struct {
    char **fields;
    int count;
} CsvRow;

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