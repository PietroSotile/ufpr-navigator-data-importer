#include <stdlib.h>
#include "include/model.h"

void classroom_destroy(Classroom *classroom) {
    if(classroom) {
        free(classroom->name);
        free(classroom);
    }
}
void building_destroy(Building *building) {
    if(building) {
        free(building->name);
        free(building->polygon);
        free(building);
    }
}