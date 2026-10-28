#include "page.h"

#include <string.h>

void page_init(uint8_t *page) {
    (void)page; // TODO
}

size_t page_free_space(const uint8_t *page) {
    (void)page;
    return 0; // TODO
}

uint16_t page_num_slots(const uint8_t *page) {
    (void)page;
    return 0; // TODO
}

void page_compact(uint8_t *page) {
    (void)page; // TODO
}

int page_insert(uint8_t *page, const void *data, uint16_t len) {
    (void)page; (void)data; (void)len;
    return -1; // TODO
}

const uint8_t *page_get(const uint8_t *page, uint16_t slot, uint16_t *len) {
    (void)page; (void)slot; (void)len;
    return NULL; // TODO
}

bool page_delete(uint8_t *page, uint16_t slot) {
    (void)page; (void)slot;
    return false; // TODO
}

bool page_update(uint8_t *page, uint16_t slot, const void *data, uint16_t len) {
    (void)page; (void)slot; (void)data; (void)len;
    return false; // TODO
}
