#include "ringbuf.h"
#include "test.h"

TEST(test_init_rejects_non_power_of_two) {
    RingBuf rb;
    CHECK(!rb_init(&rb, 0));
    CHECK(!rb_init(&rb, 12));
    CHECK(rb_init(&rb, 16));
    CHECK_EQ(rb_len(&rb), 0);
    CHECK_EQ(rb_space(&rb), 16);
    rb_free(&rb);
    CHECK_NO_LEAKS();
}

TEST(test_write_read) {
    RingBuf rb;
    rb_init(&rb, 8);
    CHECK_EQ(rb_write(&rb, "hello", 5), 5);
    CHECK_EQ(rb_len(&rb), 5);
    char out[16] = {0};
    CHECK_EQ(rb_peek(&rb, out, 2), 2);
    CHECK(memcmp(out, "he", 2) == 0);
    CHECK_EQ(rb_len(&rb), 5); // peek doesn't consume
    CHECK_EQ(rb_read(&rb, out, 16), 5);
    CHECK(memcmp(out, "hello", 5) == 0);
    CHECK_EQ(rb_len(&rb), 0);
    CHECK_EQ(rb_read(&rb, out, 1), 0);
    rb_free(&rb);
    CHECK_NO_LEAKS();
}

TEST(test_full_partial_write) {
    RingBuf rb;
    rb_init(&rb, 8);
    CHECK_EQ(rb_write(&rb, "0123456789", 10), 8);
    CHECK_EQ(rb_space(&rb), 0);
    CHECK_EQ(rb_write(&rb, "x", 1), 0);
    char out[8];
    rb_read(&rb, out, 8);
    CHECK(memcmp(out, "01234567", 8) == 0);
    rb_free(&rb);
    CHECK_NO_LEAKS();
}

TEST(test_wrap_around) {
    RingBuf rb;
    rb_init(&rb, 8);
    char out[8];
    rb_write(&rb, "abcdef", 6);
    rb_read(&rb, out, 4);              // head=4, tail=6
    CHECK_EQ(rb_write(&rb, "GHIJKL", 6), 6); // wraps: 2 bytes at the end, 4 at the start
    CHECK_EQ(rb_len(&rb), 8);
    CHECK_EQ(rb_read(&rb, out, 8), 8);
    CHECK(memcmp(out, "efGHIJKL", 8) == 0);
    rb_free(&rb);
    CHECK_NO_LEAKS();
}

TEST(test_stream_against_model) {
    // push a long byte stream through a small ring in odd-sized chunks
    RingBuf rb;
    rb_init(&rb, 64);
    unsigned char in[37], out[53];
    unsigned next_in = 0, next_out = 0;
    int bad = 0;
    for (int step = 0; step < 20000; step++) {
        size_t n = (size_t)(step * 7 % 37) + 1;
        for (size_t i = 0; i < n; i++) in[i] = (unsigned char)(next_in + i);
        next_in += (unsigned)rb_write(&rb, in, n);
        size_t m = rb_read(&rb, out, (size_t)(step * 11 % 53) + 1);
        for (size_t i = 0; i < m; i++)
            if (out[i] != (unsigned char)next_out++) bad++;
    }
    CHECK_EQ(bad, 0);
    CHECK(next_in - next_out == rb_len(&rb));
    rb_free(&rb);
    CHECK_NO_LEAKS();
}

TEST(test_counter_overflow) {
    RingBuf rb;
    rb_init(&rb, 8);
    rb.head = rb.tail = (size_t)-3; // counters about to wrap around
    CHECK_EQ(rb_write(&rb, "abcdef", 6), 6);
    CHECK_EQ(rb_len(&rb), 6);
    char out[6];
    CHECK_EQ(rb_read(&rb, out, 6), 6);
    CHECK(memcmp(out, "abcdef", 6) == 0);
    rb_free(&rb);
    CHECK_NO_LEAKS();
}

int main(void) {
    RUN(test_init_rejects_non_power_of_two);
    RUN(test_write_read);
    RUN(test_full_partial_write);
    RUN(test_wrap_around);
    RUN(test_stream_against_model);
    RUN(test_counter_overflow);
    TEST_SUMMARY();
}
