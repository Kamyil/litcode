#include "ringbuf.h"

#include <stdlib.h>
#include <string.h>

bool rb_init(RingBuf *rb, size_t cap) {
    *rb = (RingBuf){0};
    if (cap == 0 || (cap & (cap - 1)) != 0) return false;
    rb->buf = malloc(cap);
    if (!rb->buf) return false;
    rb->cap = cap;
    return true;
}

void rb_free(RingBuf *rb) {
    free(rb->buf);
    *rb = (RingBuf){0};
}

size_t rb_len(const RingBuf *rb) { return rb->tail - rb->head; }

size_t rb_space(const RingBuf *rb) { return rb->cap - rb_len(rb); }

size_t rb_write(RingBuf *rb, const void *src, size_t n) {
    if (n > rb_space(rb)) n = rb_space(rb);
    size_t t = rb->tail & (rb->cap - 1);
    size_t first = n < rb->cap - t ? n : rb->cap - t;
    memcpy(rb->buf + t, src, first);
    memcpy(rb->buf, (const unsigned char *)src + first, n - first);
    rb->tail += n;
    return n;
}

size_t rb_peek(const RingBuf *rb, void *dst, size_t n) {
    if (n > rb_len(rb)) n = rb_len(rb);
    size_t h = rb->head & (rb->cap - 1);
    size_t first = n < rb->cap - h ? n : rb->cap - h;
    memcpy(dst, rb->buf + h, first);
    memcpy((unsigned char *)dst + first, rb->buf, n - first);
    return n;
}

size_t rb_read(RingBuf *rb, void *dst, size_t n) {
    n = rb_peek(rb, dst, n);
    rb->head += n;
    return n;
}
