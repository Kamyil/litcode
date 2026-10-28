#ifndef RINGBUF_H
#define RINGBUF_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    unsigned char *buf;
    size_t cap;  // power of two
    size_t head; // total bytes ever read  (physical index: head & (cap - 1))
    size_t tail; // total bytes ever written
} RingBuf;

bool rb_init(RingBuf *rb, size_t cap);
void rb_free(RingBuf *rb);

size_t rb_len(const RingBuf *rb);   // bytes available to read
size_t rb_space(const RingBuf *rb); // bytes available to write

size_t rb_write(RingBuf *rb, const void *src, size_t n);
size_t rb_read(RingBuf *rb, void *dst, size_t n);
size_t rb_peek(const RingBuf *rb, void *dst, size_t n);

#endif
