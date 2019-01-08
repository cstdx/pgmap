#ifndef CSTDX_PGMAP_H
#define CSTDX_PGMAP_H

#include <stdbool.h>
#include <stdint.h>

typedef struct cx_pgmap cx_pgmap_t;

/**
 * Create new pgmap.
 * @param  page page index size.
 * @param  cell cell index size.
 * @param  size item size.
 * @return      pointer to the new pgmap.
 */
cx_pgmap_t* pgmap_new(uint8_t page, uint8_t cell, uint32_t size);

/**
 * Deallocate pgamap.
 * @param self pgmap to be deallocated.
 */
void map_free(cx_pgmap_t* self);

/**
 * Set key/value pair.
 * Complexity: O(1).
 * @param self pgmap to be modified.
 * @param key  item's key.
 * @param data item's value.
 */
void map_set(cx_pgmap_t* self, uint32_t key, void* value);

/**
 * Get value associated with the key provided. If the key already exists in the
 * pgmap, it's value will be overriden.
 * Complexity: O(1).
 * @param  self pgmap to be looked up.
 * @param  key  item's key.
 * @param  data item's value.
 * @return      true if key was found and value was recorded, otherwise false.
 */
bool map_get(cx_pgmap_t* self, uint32_t key, void** value);

/**
 * Delete value associated with the key provided.
 * Complexity: O(1).
 * @param  self pgmap to be modified.
 * @param  key  item's key.
 * @return      true if key was found and data was removed, otherwise false.
 */
bool map_del(cx_pgmap_t* self, uint32_t key);

#endif // !CSTDX_PGMAP_H
