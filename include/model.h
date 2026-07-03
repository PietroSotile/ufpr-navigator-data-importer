#ifndef MODEL_H
#define MODEL_H

typedef struct {
    double latitude;
    double longitude;
} Coordinate;

typedef struct {
    int id;
    char *name;
    Coordinate coordinate;
    int building_id;
} Classroom;

typedef struct {
    int place_id;
    Coordinate coordinate;
} Polyline;

#define BUILDING_POLYGON_CAPACITY 16

typedef struct {
    int id;
    char *name;
    char code[8]; // Todos predios da UFPR tem menos de 7 digitos Ex: PA
    Coordinate entrance;
    int campus_id;

    Coordinate *polygon;
    int polygon_count;
    int polygon_capacity;
} Building;

/**
 * Desaloca a memória utilizada por *classroom e campos derivados
 */
void classroom_destroy(Classroom *classroom);

/**
 * Desaloca a memória utilizada por *building e campos derivados
 */
void building_destroy(Building *building);

/**
 * Desaloca a memória utilizada por *polyline e campos derivados
 */
void polyline_destroy(Polyline *polyline);

#endif