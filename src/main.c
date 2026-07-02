#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/converter.h"
#include "../include/csv_parser.h"

int main(int argc, char *argv[]) {
    if(argc < 3 || (argc < 4 && (strcmp(argv[1], "-c") == 0 || 
      strcmp(argv[1], "-b") == 0))) {
        printf("Uso: %s <opção> <csv> [csv_poligono]\n", argv[0]);
        return 1;
    }

    const char *option = argv[1];
    const char *csv = argv[2];

    if(strcmp(option, "-r") == 0) {
        classrooms_to_json(csv);
        return 0;
    }
    const char *csvPoly = argv[3];
    if(strcmp(option, "-b") == 0) {
        buildings_to_json(csv, csvPoly);
        return 0;
    }
    if(strcmp(option, "-c") == 0) {
        campuses_to_json(csv, csvPoly);
        return 0;
    }

    printf("Opção inválida\n");

    return 0;
}