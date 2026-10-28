#include "page.h"

#include <string.h>

static size_t align_up(size_t n) { return (n + TUPLE_ALIGN - 1) & ~(size_t)(TUPLE_ALIGN - 1); }
static PageHeader *header(uint8_t *page) { return (PageHeader *)page; }
static const PageHeader *cheader(const uint8_t *page) { return (const PageHeader *)page; }
static ItemId *items(uint8_t *page) { return (ItemId *)(page + sizeof(PageHeader)); }
static const ItemId *citems(const uint8_t *page) { return (const ItemId *)(page + sizeof(PageHeader)); }

void page_init(uint8_t *page) {
    memset(page, 0, PAGE_SIZE);
    *header(page) = (PageHeader){.lower = sizeof(PageHeader), .upper = PAGE_SIZE};
}

size_t page_free_space(const uint8_t *page) { return (size_t)(cheader(page)->upper - cheader(page)->lower); }

uint16_t page_num_slots(const uint8_t *page) { return cheader(page)->nslots; }

// Free space the page would have after compaction, ignoring slot `skip` (pass -1 for none).
static size_t reclaimable(const uint8_t *page, int skip) {
    size_t used = 0;
    for (int i = 0; i < cheader(page)->nslots; i++)
        if (i != skip && citems(page)[i].len) used += align_up(citems(page)[i].len);
    return PAGE_SIZE - cheader(page)->lower - used;
}

void page_compact(uint8_t *page) {
    uint8_t copy[PAGE_SIZE];
    memcpy(copy, page, PAGE_SIZE);
    PageHeader *h = header(page);
    h->upper = PAGE_SIZE;
    for (int i = 0; i < h->nslots; i++) {
        ItemId *it = &items(page)[i];
        if (!it->len) continue;
        h->upper = (uint16_t)(h->upper - align_up(it->len));
        memcpy(page + h->upper, copy + it->offset, it->len);
        it->offset = h->upper;
    }
}

// Reserves align_up(len) bytes at the top of the free gap (compacting if needed), returns the offset.
static uint16_t place(uint8_t *page, const void *data, uint16_t len) {
    PageHeader *h = header(page);
    if (page_free_space(page) < align_up(len)) page_compact(page);
    h->upper = (uint16_t)(h->upper - align_up(len));
    memcpy(page + h->upper, data, len);
    return h->upper;
}

int page_insert(uint8_t *page, const void *data, uint16_t len) {
    PageHeader *h = header(page);
    if (len == 0) return -1;
    int slot = 0;
    while (slot < h->nslots && items(page)[slot].len) slot++;
    bool new_slot = slot == h->nslots;
    size_t need = align_up(len) + (new_slot ? sizeof(ItemId) : 0);
    if (reclaimable(page, -1) < need) return -1;
    if (new_slot) {
        if (page_free_space(page) < need) page_compact(page);
        items(page)[slot] = (ItemId){0};
        h->nslots++;
        h->lower = (uint16_t)(h->lower + sizeof(ItemId));
    }
    uint16_t off = place(page, data, len);
    items(page)[slot] = (ItemId){.offset = off, .len = len};
    return slot;
}

const uint8_t *page_get(const uint8_t *page, uint16_t slot, uint16_t *len) {
    if (slot >= cheader(page)->nslots || citems(page)[slot].len == 0) return NULL;
    if (len) *len = citems(page)[slot].len;
    return page + citems(page)[slot].offset;
}

bool page_delete(uint8_t *page, uint16_t slot) {
    if (slot >= header(page)->nslots || items(page)[slot].len == 0) return false;
    items(page)[slot] = (ItemId){0};
    return true;
}

bool page_update(uint8_t *page, uint16_t slot, const void *data, uint16_t len) {
    if (len == 0 || slot >= header(page)->nslots || items(page)[slot].len == 0) return false;
    ItemId *it = &items(page)[slot];
    if (align_up(len) <= align_up(it->len)) {
        memcpy(page + it->offset, data, len);
        it->len = len;
        return true;
    }
    if (reclaimable(page, slot) < align_up(len)) return false;
    *it = (ItemId){0}; // old bytes become garbage; slot stays reserved for us
    uint16_t off = place(page, data, len);
    items(page)[slot] = (ItemId){.offset = off, .len = len};
    return true;
}
