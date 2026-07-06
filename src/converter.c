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

static void building_write_json(FILE *buildings, Building *building) {
    int i;
    if(!building || !buildings)
        return;

    fprintf(buildings, "\t\t{\n");
    fprintf(buildings, "\t\t\t\"id\": %d,\n", building->id);
    fprintf(buildings, "\t\t\t\"name\": \"%s\",\n", building->name);
    fprintf(buildings, "\t\t\t\"code\": \"%s\",\n", building->code);
    fprintf(buildings, "\t\t\t\"entrance\": { \"latitude\": %lf, \"longitude\": %lf },\n",
        building->entrance.latitude, building->entrance.longitude);
    fprintf(buildings, "\t\t\t\"polygon\": {\n");
    fprintf(buildings, "\t\t\t\t\"coordinates\": [\n");
    for(i = 0; i < building->polygon_count; i++) {
        fprintf(buildings, "\t\t\t\t\t{ \"latitude\": %lf, \"longitude\": %lf }",
            building->polygon[i].latitude,
            building->polygon[i].longitude);
        if(i < building->polygon_count - 1)
            fprintf(buildings, ",");
        fprintf(buildings, "\n");
    }
    fprintf(buildings, "\t\t\t\t]\n");
    fprintf(buildings, "\t\t\t},\n");
    fprintf(buildings, "\t\t\t\"campus_id\": %d\n", building->campus_id);
    fprintf(buildings, "\t\t}");
}

static void campus_write_json(FILE *campuses, Campus *campus) {
    int i;
    if(!campus || !campuses)
        return;

    fprintf(campuses, "\t\t{\n");
    fprintf(campuses, "\t\t\t\"id\": %d,\n", campus->id);
    fprintf(campuses, "\t\t\t\"name\": \"%s\",\n", campus->name);
    fprintf(campuses, "\t\t\t\"entrance\": { \"latitude\": %lf, \"longitude\": %lf },\n",
        campus->entrance.latitude, campus->entrance.longitude);
    fprintf(campuses, "\t\t\t\"polygon\": {\n");
    fprintf(campuses, "\t\t\t\t\"coordinates\": [\n");
    for(i = 0; i < campus->polygon_count; i++) {
        fprintf(campuses, "\t\t\t\t\t{ \"latitude\": %lf, \"longitude\": %lf }",
            campus->polygon[i].latitude,
            campus->polygon[i].longitude);
        if(i < campus->polygon_count - 1)
            fprintf(campuses, ",");
        fprintf(campuses, "\n");
    }
    fprintf(campuses, "\t\t\t\t]\n");
    fprintf(campuses, "\t\t\t}\n");
    fprintf(campuses, "\t\t}");
}

static Building *building_from_row(CsvRow *row) {
    int id, campus_id;
    double latitude, longitude;
    Building *building;
    char *end;

    if(!row)
        return NULL;

    if(row->count != 6) {
        printf("Número de colunas do CSV inválido para a operação\n");
        return NULL;
    }

    id = strtol(row->fields[0], &end, 10);
    if(row->fields[0] == end) {
        printf("Campo id do CSV inválido para a operação\n");
        return NULL;
    }

    latitude = strtod(row->fields[3], &end);
    if(row->fields[3] == end) {
        printf("Campo latitude do CSV inválido para a operação\n");
        return NULL;
    }

    longitude = strtod(row->fields[4], &end);
    if(row->fields[4] == end) {
        printf("Campo longitude do CSV inválido para a operação\n");
        return NULL;
    }

    campus_id = strtol(row->fields[5], &end, 10);
    if(row->fields[5] == end) {
        printf("Campo campus_id do CSV inválido para a operação\n");
        return NULL;
    }

    if(strlen(row->fields[2]) > 7) {
        printf("Campo code do CSV inválido para a operação\n");
        return NULL;
    }

    if(!(building = malloc(sizeof(Building))))
        return NULL;

    building->name = malloc(sizeof(char) * (strlen(row->fields[1]) + 1));
    if(!(building->name)) {
        free(building);
        return NULL;
    }

    building->id = id;
    strcpy(building->name, row->fields[1]);
    strcpy(building->code, row->fields[2]);
    building->entrance.latitude = latitude;
    building->entrance.longitude = longitude;
    building->campus_id = campus_id;

    building->polygon_capacity = POLYGON_CAPACITY;
    building->polygon = malloc(sizeof(Coordinate) * building->polygon_capacity);
    building->polygon_count = 0;

    return building;
}

static Campus *campus_from_row(CsvRow *row) {
    int id;
    double latitude, longitude;
    Campus *campus;
    char *end;

    if(!row)
        return NULL;

    if(row->count != 4) {
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

    if(!(campus = malloc(sizeof(Campus))))
        return NULL;

    campus->name = malloc(sizeof(char) * (strlen(row->fields[1]) + 1));
    if(!(campus->name)) {
        free(campus);
        return NULL;
    }

    campus->id = id;
    strcpy(campus->name, row->fields[1]);
    campus->entrance.latitude = latitude;
    campus->entrance.longitude = longitude;

    campus->polygon_capacity = POLYGON_CAPACITY;
    campus->polygon = malloc(sizeof(Coordinate) * campus->polygon_capacity);
    campus->polygon_count = 0;

    return campus;
}

static Polyline *polyline_from_row(CsvRow *row) {
    int place_id;
    double latitude, longitude;
    Polyline *polyline;
    char *end;

    if(!row)
        return NULL;

    if(row->count != 3) {
        printf("Número de colunas do CSV inválido para a operação\n");
        return NULL;
    }

    place_id = strtol(row->fields[0], &end, 10);
    if(row->fields[0] == end) {
        printf("Campo id do CSV inválido para a operação\n");
        return NULL;
    }

    latitude = strtod(row->fields[1], &end);
    if(row->fields[1] == end) {
        printf("Campo latitude do CSV inválido para a operação\n");
        return NULL;
    }

    longitude = strtod(row->fields[2], &end);
    if(row->fields[2] == end) {
        printf("Campo longitude do CSV inválido para a operação\n");
        return NULL;
    }

    if(!(polyline = malloc(sizeof(Polyline))))
        return NULL;

    polyline->place_id = place_id;
    polyline->coordinate.latitude = latitude;
    polyline->coordinate.longitude = longitude;

    return polyline;
}

static Building *find_building(Building **buildings, int count, int id) {
    int i;

    for(i = 0; i < count; i++) {
        if(buildings[i]->id == id)
            return buildings[i];
    }
    return NULL;
}

static void building_append_coordinate(Building *building, Coordinate coord) {
    Coordinate *temp;
    if(!building)
        return;

    if(building->polygon_count >= building->polygon_capacity) {
        building->polygon_capacity = building->polygon_capacity * 2;
        temp = realloc(building->polygon, sizeof(Coordinate) * building->polygon_capacity);

        if(!(temp))
            return;

        building->polygon = temp;
    }

    building->polygon[building->polygon_count] = coord;
    building->polygon_count++;
}

static Campus *find_campus(Campus **campuses, int count, int id) {
    int i;

    for(i = 0; i < count; i++) {
        if(campuses[i]->id == id)
            return campuses[i];
    }
    return NULL;
}

static void campus_append_coordinate(Campus *campus, Coordinate coord) {
    Coordinate *temp;
    if(!campus)
        return;

    if(campus->polygon_count >= campus->polygon_capacity) {
        campus->polygon_capacity = campus->polygon_capacity * 2;
        temp = realloc(campus->polygon, sizeof(Coordinate) * campus->polygon_capacity);

        if(!(temp))
            return;

        campus->polygon = temp;
    }

    campus->polygon[campus->polygon_count] = coord;
    campus->polygon_count++;
}

static Building **load_buildings(FILE *buildings_csv, int *count) {
    int index;
    char *word;
    Building **buildings;
    CsvRow *row;

    if(!buildings_csv)
        return NULL;

    read_line(buildings_csv, &word);
    free(word);

    *count = 0;
    while(read_line(buildings_csv, &word) != 0) {
        (*count)++;
        free(word);
    }
    rewind(buildings_csv);

    if(!(buildings = malloc(sizeof(Building *) * (*count))))
        return NULL;

    read_line(buildings_csv, &word);
    free(word);

    index = 0;
    while(read_line(buildings_csv, &word) != 0) {
        row = csv_parse_line(word);
        free(word);

        buildings[index] = building_from_row(row);

        csv_row_free(row);
        index++;
    }

    return buildings;
}

static Campus **load_campuses(FILE *campuses_csv, int *count) {
    int index;
    char *word;
    Campus **campuses;
    CsvRow *row;

    if(!campuses_csv)
        return NULL;

    read_line(campuses_csv, &word);
    free(word);

    *count = 0;
    while(read_line(campuses_csv, &word) != 0) {
        (*count)++;
        free(word);
    }
    rewind(campuses_csv);

    if(!(campuses = malloc(sizeof(Campus *) * (*count))))
        return NULL;

    read_line(campuses_csv, &word);
    free(word);

    index = 0;
    while(read_line(campuses_csv, &word) != 0) {
        row = csv_parse_line(word);
        free(word);

        campuses[index] = campus_from_row(row);

        csv_row_free(row);
        index++;
    }

    return campuses;
}

static void load_building_polygons(FILE *csv_poly, Building **buildings, int buildings_count) {
    char *word;
    Polyline *polyline;
    Building *building;
    CsvRow *row;

    if(!csv_poly || !buildings)
        return;

    read_line(csv_poly, &word);
    free(word);

    while(read_line(csv_poly, &word) != 0) {
        row = csv_parse_line(word);
        if((polyline = polyline_from_row(row)) == NULL) {
            free(word);
            csv_row_free(row);
            continue;
        }

        free(word);
        csv_row_free(row);

        building = find_building(buildings, buildings_count, polyline->place_id);
        if(building)
            building_append_coordinate(building, polyline->coordinate);
        polyline_destroy(polyline);
    }
}

static void load_campus_polygons(FILE *csv_poly, Campus **campuses, int campuses_count) {
    char *word;
    Polyline *polyline;
    Campus *campus;
    CsvRow *row;

    if(!csv_poly || !campuses)
        return;

    read_line(csv_poly, &word);
    free(word);

    while(read_line(csv_poly, &word) != 0) {
        row = csv_parse_line(word);
        if((polyline = polyline_from_row(row)) == NULL) {
            free(word);
            csv_row_free(row);
            continue;
        }

        free(word);
        csv_row_free(row);

        campus = find_campus(campuses, campuses_count, polyline->place_id);
        if(campus)
            campus_append_coordinate(campus, polyline->coordinate);
        polyline_destroy(polyline);
    }
}

int classrooms_to_json(const char *csv) {
    char *word;
    FILE *classrooms_csv, *classrooms_json;
    CsvRow *row;
    Classroom *classroom;

    if(!csv)
        return 0;

    classrooms_json = fopen(CLASSROOM_FILENAME, "w+");
    if(!classrooms_json) {
        perror("Erro ao abrir/criar arquivo .json");
        exit(1);
    }

    classrooms_csv = fopen(csv, "r");
    if(!classrooms_csv) {
        perror("Erro ao abrir o arquivo csv");
        fclose(classrooms_json);
        return 1;
    }

    fprintf(classrooms_json, "{\n");
    fprintf(classrooms_json, "\t\"classrooms\": [\n\t");

    read_line(classrooms_csv, &word);
    free(word);

    if(read_line(classrooms_csv, &word) != 0) {
        row = csv_parse_line(word);
        free(word);

        classroom = classroom_from_row(row);

        fprintf(classrooms_json, "\t");
        classroom_write_json(classrooms_json, classroom);

        classroom_destroy(classroom);
        csv_row_free(row);
    }

    while(read_line(classrooms_csv, &word) != 0) {
        row = csv_parse_line(word);
        free(word);

        classroom = classroom_from_row(row);

        fprintf(classrooms_json, ",\n\t\t");
        classroom_write_json(classrooms_json, classroom);

        classroom_destroy(classroom);
        csv_row_free(row);
    }

    fprintf(classrooms_json, "\n\t]\n");
    fprintf(classrooms_json, "}\n");

    fclose(classrooms_json);
    fclose(classrooms_csv);
    return 1;
}

int buildings_to_json(const char *csv, const char *csv_poly) {
    FILE *buildings_csv, *polylines_csv, *buildings_json;
    Building **buildings;
    int buildings_count, i;

    if(!csv || !csv_poly)
        return 0;

    buildings_json = fopen(BUILDING_FILENAME, "w+");
    if(!buildings_json) {
        perror("Erro ao abrir/criar arquivo .json");
        exit(1);
    }

    buildings_csv = fopen(csv, "r");
    if(!buildings_csv) {
        fclose(buildings_json);
        perror("Erro ao abrir o arquivo csv de prédios\n");
        exit(1);
    }

    polylines_csv = fopen(csv_poly, "r");
    if(!polylines_csv) {
        fclose(buildings_json);
        fclose(buildings_csv);
        perror("Erro ao abrir o arquivo csv de contorno dos prédios\n");
        exit(1);
    }

    buildings = load_buildings(buildings_csv, &buildings_count);
    load_building_polygons(polylines_csv, buildings, buildings_count);

    fprintf(buildings_json, "{\n");
    fprintf(buildings_json, "\t\"buildings\": [\n");

    building_write_json(buildings_json, buildings[0]);
    for(i = 1; i < buildings_count; i++) {
        fprintf(buildings_json, ",\n");
        building_write_json(buildings_json, buildings[i]);
    }

    fprintf(buildings_json, "\n\t]\n");
    fprintf(buildings_json, "}\n");

    for(i = 0; i < buildings_count; i++)
        building_destroy(buildings[i]);
    free(buildings);

    fclose(buildings_json);
    fclose(buildings_csv);
    fclose(polylines_csv);
    return 1;
}

int campuses_to_json(const char *csv, const char *csv_poly) {
    FILE *campuses_csv, *polylines_csv, *campuses_json;
    Campus **campuses;
    int campuses_count, i;

    if(!csv || !csv_poly)
        return 0;

    campuses_json = fopen(CAMPUSES_FILENAME, "w+");
    if(!campuses_json) {
        perror("Erro ao abrir/criar arquivo .json");
        exit(1);
    }

    campuses_csv = fopen(csv, "r");
    if(!campuses_csv) {
        fclose(campuses_json);
        perror("Erro ao abrir o arquivo csv dos campi\n");
        exit(1);
    }

    polylines_csv = fopen(csv_poly, "r");
    if(!polylines_csv) {
        fclose(campuses_json);
        fclose(campuses_csv);
        perror("Erro ao abrir o arquivo csv de contorno dos campi\n");
        exit(1);
    }

    campuses = load_campuses(campuses_csv, &campuses_count);
    load_campus_polygons(polylines_csv, campuses, campuses_count);

    fprintf(campuses_json, "{\n");
    fprintf(campuses_json, "\t\"campuses\": [\n");

    campus_write_json(campuses_json, campuses[0]);
    for(i = 1; i < campuses_count; i++) {
        fprintf(campuses_json, ",\n");
        campus_write_json(campuses_json, campuses[i]);
    }

    fprintf(campuses_json, "\n\t]\n");
    fprintf(campuses_json, "}\n");

    for(i = 0; i < campuses_count; i++)
        campus_destroy(campuses[i]);
    free(campuses);

    fclose(campuses_json);
    fclose(campuses_csv);
    fclose(polylines_csv);
    return 1;
}