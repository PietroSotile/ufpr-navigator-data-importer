#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/csv_parser.h"
#include "../include/model.h"
#include "../include/converter.h"

static Classroom *classroom_from_row(CsvRow *row) {
    int id, building_id;
    double latitude, longitude;
    Classroom *room;
    char *end;

    if(!row)
        return NULL;

    if(row->count != 5) {
        printf("Número de colunas do CSV inválido para a operação\n");
        return NULL;
    }

    id = strtol(row->fields[0], &end, 10);
    if(row->fields[0] == end) {
        printf("Campo id do CSV inválido para a operação\n");
        return NULL;
    }

    latitude = strtod(row->fields[2], &end);
    if(row->fields[2] == end) {
        printf("Campo latitude do CSV inválido para a operação\n");
        return NULL;
    }

    longitude = strtod(row->fields[3], &end);
    if(row->fields[3] == end) {
        printf("Campo longitude do CSV inválido para a operação\n");
        return NULL;
    }

    building_id = strtol(row->fields[4], &end, 10);
    if(row->fields[4] == end) {
        printf("Campo building_id do CSV inválido para a operação\n");
        return NULL;
    }
        
    
    if(!(room = malloc(sizeof(Classroom))))
        return NULL;

    room->name = malloc(sizeof(char) * (strlen(row->fields[1]) + 1));
    if(!(room->name)) {
        free(room);
        return NULL;
    }

    room->id = id;
    strcpy(room->name, row->fields[1]);
    room->coordinate.latitude = latitude;
    room->coordinate.longitude = longitude;
    room->building_id = building_id;

    return room;
}

static void classroom_write_json(FILE *classrooms, Classroom *classroom) {
    if(!classroom || !classrooms)
        return;

    fprintf(classrooms, "{ \"id\": %d, \"name\": \"%s\", \"coordinate\": { \"latitude\": %lf, \"longitude\": %lf }, \"building_id\": %d }",
        classroom->id, classroom->name, classroom->coordinate.latitude, classroom->coordinate.longitude, classroom->building_id);
}

void classroom_destroy(Classroom *classroom) {
    if(classroom) {
        free(classroom->name);
        free(classroom);
    }
}

int classrooms_to_json(const char *csv) {
    char *word;
    FILE *classrooms_csv ;
    CsvRow *row;
    Classroom *classroom;
    FILE *classrooms;
    if(!csv)
        return 0;

    classrooms = fopen(CLASSROOM_FILENAME, "w+");
    if(!classrooms) {
        perror("Erro ao abrir/criar arquivo .json");
        exit (1);
    }

    classrooms_csv = fopen(csv, "r");

    if(!classrooms_csv) {
        perror("Erro ao abrir o arquivo csv");
        fclose(classrooms);
        return 1;
    }
    
    fprintf(classrooms, "{\n");
    fprintf(classrooms, "\t\"classrooms\": [\n\t");

    read_line(classrooms_csv, &word); // remove o cabeçalho
    free(word);

    if(read_line(classrooms_csv, &word) != 0) {
        row = csv_parse_line(word);
        free(word);

        classroom = classroom_from_row(row);

        fprintf(classrooms, "\t");
        classroom_write_json(classrooms, classroom);

        classroom_destroy(classroom);
        csv_row_free(row);
    }

    while(read_line(classrooms_csv, &word) != 0) {
        row = csv_parse_line(word);
        free(word);

        classroom = classroom_from_row(row);

        fprintf(classrooms, ",\n\t\t");
        classroom_write_json(classrooms, classroom);

        classroom_destroy(classroom);
        csv_row_free(row);
    }

    fprintf(classrooms, "\n\t]\n");
    fprintf(classrooms, "}\n");

    fclose(classrooms);
    fclose(classrooms_csv);
    return 1;
}

int buildings_to_json(const char *csv, const char *csv_poly) {
    if(!csv)
        return 0;
    printf("GERA buildings.json\n");

    return 1;
}

int campuses_to_json(const char *csv, const char *csv_poly) {
    if(!csv)
        return 0;
    printf("GERA campuses.json\n");

    return 1;
}