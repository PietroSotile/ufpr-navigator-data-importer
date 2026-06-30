#include <stdio.h>
#include <string.h>
#include "../include/converter.h"
#include "../include/csv_parser.h"

int main(int argc, char *argv[]) {
    CsvRow *row;
    const char *word = "1,John Doe,25,1.82,New York";
    row = csv_parse_line(word);
    printf("%s\n", word);

    for(int i = 0; i < row->count; i++) {
        printf("%s\n", row->fields[i]);
    }

    csv_row_free(row);
    // if(argc < 3 || (argc < 4 && (strcmp(argv[1], "-c") == 0 || 
    //   strcmp(argv[1], "-b") == 0))) {
    //     printf("Uso: %s <opção> <csv> [csv_poligono]\n", argv[0]);
    //     return 1;
    // }

    // const char *option = argv[1];
    // const char *csv = argv[2];

    // if(strcmp(option, "-r") == 0) {
    //     classrooms_to_json(csv);
    //     return 0;
    // }
    // const char *csvPoly = argv[3];
    // if(strcmp(option, "-b") == 0) {
    //     buildings_to_json(csv, csvPoly);
    //     return 0;
    // }
    // if(strcmp(option, "-c") == 0) {
    //     campuses_to_json(csv, csvPoly);
    //     return 0;
    // }

    // printf("Opção inválida\n");

    return 0;
}