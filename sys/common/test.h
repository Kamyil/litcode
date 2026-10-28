// Tiny test harness for the C challenges. No dependencies, just macros.
#ifndef LC_TEST_H
#define LC_TEST_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

static int lc_failures = 0;
static int lc_tests = 0;
static int lc_failed_tests = 0;
static const char *lc_current = "";

#define CHECK(cond)                                                              \
    do {                                                                         \
        if (!(cond)) {                                                           \
            lc_failures++;                                                       \
            fprintf(stderr, "    %s:%d: CHECK(%s) failed\n", __FILE__, __LINE__, \
                    #cond);                                                      \
        }                                                                        \
    } while (0)

#define CHECK_EQ(actual, expected)                                                  \
    do {                                                                            \
        long long lc_a = (long long)(actual), lc_e = (long long)(expected);         \
        if (lc_a != lc_e) {                                                         \
            lc_failures++;                                                          \
            fprintf(stderr, "    %s:%d: %s == %s  (got %lld, expected %lld)\n",     \
                    __FILE__, __LINE__, #actual, #expected, lc_a, lc_e);            \
        }                                                                           \
    } while (0)

#define CHECK_STR(actual, expected)                                                 \
    do {                                                                            \
        const char *lc_a = (actual), *lc_e = (expected);                            \
        if (lc_a == NULL || strcmp(lc_a, lc_e) != 0) {                              \
            lc_failures++;                                                          \
            fprintf(stderr, "    %s:%d: %s == \"%s\"  (got \"%s\")\n", __FILE__,     \
                    __LINE__, #actual, lc_e, lc_a ? lc_a : "(null)");               \
        }                                                                           \
    } while (0)

// Every test should end with this: all memory the test (and your code) allocated
// must be freed again. Uses the allocation tracker from track_alloc.h.
#define CHECK_NO_LEAKS()                                                            \
    do {                                                                            \
        if (ta_live_allocs() != lc_live_at_start) {                                 \
            lc_failures++;                                                          \
            fprintf(stderr, "    %s:%d: memory leak: %zu allocation(s) still live\n",\
                    __FILE__, __LINE__, ta_live_allocs() - lc_live_at_start);       \
        }                                                                           \
    } while (0)

#define TEST(name) static void name(__attribute__((unused)) size_t lc_live_at_start)

// Each test runs in a forked child, so a segfault or ASan abort in one test is
// reported as CRASH and the remaining tests still run. Set LC_NOFORK=1 to run
// in-process (useful under lldb/gdb).
#define RUN(name)                                                                  \
    do {                                                                           \
        lc_current = #name;                                                        \
        lc_tests++;                                                                \
        fflush(stdout);                                                            \
        fflush(stderr);                                                            \
        int lc_status = 0;                                                         \
        if (getenv("LC_NOFORK")) {                                                 \
            int lc_before = lc_failures;                                           \
            name(ta_live_allocs());                                                \
            lc_status = lc_failures != lc_before;                                  \
        } else {                                                                   \
            pid_t lc_pid = fork();                                                 \
            if (lc_pid == 0) {                                                     \
                name(ta_live_allocs());                                            \
                fflush(stdout);                                                    \
                fflush(stderr);                                                    \
                _exit(lc_failures ? 1 : 0);                                        \
            }                                                                      \
            int lc_wstatus;                                                        \
            waitpid(lc_pid, &lc_wstatus, 0);                                       \
            if (WIFSIGNALED(lc_wstatus)) lc_status = 128 + WTERMSIG(lc_wstatus);   \
            else lc_status = WEXITSTATUS(lc_wstatus);                              \
        }                                                                          \
        if (lc_status == 0) {                                                      \
            printf("  \033[32mok\033[0m    %s\n", #name);                          \
        } else {                                                                   \
            lc_failed_tests++;                                                     \
            if (lc_status > 1) printf("  \033[31mCRASH\033[0m %s\n", #name);       \
            else printf("  \033[31mFAIL\033[0m  %s\n", #name);                     \
        }                                                                          \
        fflush(stdout);                                                            \
    } while (0)

#define TEST_SUMMARY()                                                           \
    do {                                                                         \
        printf("\n%d/%d tests passed\n", lc_tests - lc_failed_tests, lc_tests); \
        return lc_failed_tests ? 1 : 0;                                          \
    } while (0)

#endif
