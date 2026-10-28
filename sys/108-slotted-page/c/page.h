#ifndef PAGE_H
#define PAGE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define PAGE_SIZE 8192
#define TUPLE_ALIGN 8

typedef struct {
    uint16_t lower;  // end of line pointer array = sizeof(PageHeader) + nslots * sizeof(ItemId)
    uint16_t upper;  // start of tuple space
    uint16_t nslots; // line pointers in use (live or unused)
    uint16_t flags;  // always 0
} PageHeader;

typedef struct {
    uint16_t offset; // byte offset of the tuple in the page
    uint16_t len;    // tuple length; 0 = unused slot
} ItemId;

_Static_assert(sizeof(PageHeader) == 8, "header layout");
_Static_assert(sizeof(ItemId) == 4, "line pointer layout");

void page_init(uint8_t *page);
size_t page_free_space(const uint8_t *page); // contiguous: upper - lower
uint16_t page_num_slots(const uint8_t *page);

int page_insert(uint8_t *page, const void *data, uint16_t len); // slot number, or -1 if it can't fit
const uint8_t *page_get(const uint8_t *page, uint16_t slot, uint16_t *len); // NULL if unused/out of range
bool page_delete(uint8_t *page, uint16_t slot);                 // false if unused/out of range
bool page_update(uint8_t *page, uint16_t slot, const void *data, uint16_t len);
void page_compact(uint8_t *page);

#endif
