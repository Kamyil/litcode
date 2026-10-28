#include "ringbuf.h"

#include <stdlib.h>
#include <string.h>

bool rb_init(RingBuf *rb, size_t cap) {
    (void)rb; (void)cap;
    return false; // TODO
}

void rb_free(RingBuf *rb) {
    (void)rb; // TODO
}

size_t rb_len(const RingBuf *rb) {
    (void)rb;
    return 0; // TODO
}

size_t rb_space(const RingBuf *rb) {
    (void)rb;
    return 0; // TODO
}

size_t rb_write(RingBuf *rb, const void *src, size_t n) {
    (void)rb; (void)src; (void)n;
    return 0; // TODO
}

size_t rb_peek(const RingBuf *rb, void *dst, size_t n) {
    (void)rb; (void)dst; (void)n;
    return 0; // TODO
}

size_t rb_read(RingBuf *rb, void *dst, size_t n) {
    (void)rb; (void)dst; (void)n;
    return 0; // TODO
}
