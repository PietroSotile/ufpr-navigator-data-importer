#include <stdlib.h>
#include <string.h>
#include "../include/csv_parser.h"

#define BUFFER_SIZE 32

CsvRow *csv_parse_line(const char *line) {
    int i, j, field_index, char_index, field_count, line_length;
    size_t capacidade;
    CsvRow *row;

    if(!line)
        return NULL;

    if(!(row = malloc(sizeof(CsvRow))))
        return NULL;

    i = 0;
    field_count = 0;

    while(line[i] != '\n' && line[i] != '\r') {
        if(line[i] == ',')
            field_count++;
        i++;
    }
    line_length = i;

    if(!(row->fields = malloc(sizeof(char *) * (field_count + 1)))) {
        free(row);
        return NULL;
    }

    for(j = 0; j < field_count + 1; j++) {
        if(!(row->fields[j] = malloc(sizeof(char) * BUFFER_SIZE))) {
            free(row->fields);
            free(row);
            return NULL;
        }
    }

    field_index = 0;
    char_index = 0;
    capacidade = BUFFER_SIZE;
    for(i = 0; i < line_length; i++) {
        if(line[i] == ',') {
            row->fields[field_index][char_index] = '\0';
            field_index++;
            char_index = 0;

            continue;
        } else if(char_index > capacidade - 1) {
            capacidade = capacidade * 2;
            row->fields[field_index] = realloc(row->fields[field_index], capacidade);
        }
            
        row->fields[field_index][char_index] = line[i];
        char_index++;
    }
    row->fields[field_index][char_index] = '\0';

    row->count = field_count + 1;

    return row;
}

void csv_row_free(CsvRow *row) {
    int i;
    if(row) {
        for(i=0; i<row->count; i++)
            free(row->fields[i]);
        free(row->fields);
        free(row);
    }
}