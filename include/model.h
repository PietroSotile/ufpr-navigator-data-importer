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

#endif