#include "pgmap.h"

#include <stdlib.h>

struct cx_pgmap {
    uint8_t   cap;
    uint8_t   len;
    uint32_t  npages;
    uint32_t  ncells;
    uint32_t* stats;
    void***   data;
};

static inline uint32_t pageid(cx_pgmap_t* self, uint32_t key) {
    // TODO: implement getting page id from key
    return 0;
}

static inline uint32_t cellid(cx_pgmap_t* self, uint32_t key) {
    // TODO: implement getting cell id from key
    return 0;
}

cx_pgmap_t* pgmap_new(uint8_t page, uint8_t cell, uint32_t size) {
    return NULL;
}

void map_free(cx_pgmap_t* self) {
}

void map_set(cx_pgmap_t* self, uint32_t key, void* value) {
}

bool map_get(cx_pgmap_t* self, uint32_t key, void** value) {
    return false;
}

bool map_del(cx_pgmap_t* self, uint32_t key) {
    return false;
}
