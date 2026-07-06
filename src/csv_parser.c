#include <stdlib.h>
#include <string.h>
#include "../include/csv_parser.h"

int read_line(FILE *file, char **line) {
    int capacity, line_length;
    char c, *temp;
    if(!file)
        return -1;

    capacity = LINE_SIZE;
    line_length = 0;
    
    *line = malloc(sizeof(char) * capacity);
    if(!(*line))
        return -1;

    c = getc(file);
    if(c == EOF) {
        free(*line);
        *line = NULL;
        return 0;
    }

    while(c != EOF && c != '\n') {
        if(line_length >= capacity - 1) {
            capacity = capacity * 2;
            temp = realloc(*line, capacity);
            
            if(!(temp)) {
                free(*line);
                *line = NULL;
                return -1;
            }

            *line = temp;
        }

        (*line)[line_length] = c;
        line_length++;
        c = getc(file);
    }

    if(line_length == 0) {
        free(*line);
        *line = NULL;
        return 0;
    }

    (*line)[line_length] = '\0';

    return line_length;
}

CsvRow *csv_parse_line(const char *line) {
    int i, j, field_index, char_index, field_count, line_length, capacity;
    char *temp;
    CsvRow *row;

    if(!line)
        return NULL;

    if(!(row = malloc(sizeof(CsvRow))))
        return NULL;

    i = 0;
    field_count = 0;

    while(line[i] != '\n' && line[i] != '\r' && line[i] != '\0') {
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
    capacity = BUFFER_SIZE;
    for(i = 0; i < line_length; i++) {
        if(line[i] == ',') {
            row->fields[field_index][char_index] = '\0';
            field_index++;
            capacity = BUFFER_SIZE;
            char_index = 0;

            continue;
        } else if(capacity - char_index == 1) {
            capacity = capacity * 2;
            temp = realloc(row->fields[field_index], capacity);

            if(!temp) {
                for(j = 0; j < field_count + 1; j++)
                    free(row->fields[j]);
                free(row->fields);
                free(row);

                return NULL;
            }

            row->fields[field_index] = temp;
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