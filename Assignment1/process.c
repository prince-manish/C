#include "process.h"
#include <stdlib.h>
#include <assert.h>

process_t *new_proces() {
    return (process_t *) malloc(sizeof(process_t));
}

void delete_process(process_t *process) {
    assert(process != NULL);
    free(process);
}