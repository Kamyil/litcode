# litcode

A local LeetCode/Brilliant-style trainer for the terminal: you edit in Neovim, run tests in a side pane, and check progress with one script (`lc`).

| Track | IDs | Language | What you practise |
|---|---|---|---|
| `ts/` | 001–055 | TypeScript (bun test) | Interview patterns: hashing, two pointers, sliding window, stack, binary search, lists, trees, heaps, graphs, backtracking, DP, tries/intervals |
| `sys/` | 101–110 | **C and Rust** (same task twice) | Memory management: vector, arena, hash map, heap, ring buffer, LRU, trie, Postgres-style slotted page, B+tree, external merge sort |
| `sql/` | 201–216 | Postgres 17 | Query writing on a seeded ~600 MB shop database (joins, windows, gaps & islands, recursive CTEs, jsonb...) |
| `sql/labs/` | 251–259 | Postgres 17 | Guided internals labs: EXPLAIN, B-tree selectivity, index pitfalls, MVCC/VACUUM/HOT, isolation, locks, work_mem, partitioning/BRIN, WAL |
| `theory/` | 401–406 | Markdown | Quizzes: Big-O, DS trade-offs, C vs Rust memory, Postgres storage, MVCC/locking, SQL gotchas |

Every challenge directory has a `README.md` (the task), a `hints.md` (progressive hints), a stub to fill in and tests. Reference solutions are in `solutions/` (same relative paths) and `lc solution` asks before showing one.

## Setup

Pick one of these:

```sh
nix develop                 # or: direnv allow (.envrc does `use flake`)
                            # gives bun, clang+ASan, cargo, psql, watchexec, glow, bat; adds lc to PATH
lc box build && lc box shell   # Docker Linux toolbox (gcc ASan/LSan, valgrind, gdb, cargo, bun)
```

Without Nix you need `bun`, `clang`, `cargo` and `psql` on PATH. Then run `./lc` (or put the repo on PATH).

Database (Docker, port 54329, user/pass/db `lc`):

```sh
lc sql up && lc sql seed    # ~45 s, deterministic data
lc sql psql                 # interactive shell
lc sql down | nuke          # stop | delete the volume
```

## Daily loop

```sh
lc next                     # first unfinished challenge
lc show 013                 # render the task (glow)
lc open 013                 # $EDITOR on the stub (+ README)
lc tmux 013                 # window: editor left, `lc watch` right
lc watch 013                # re-run tests on save
lc test 103 c | rust        # sys challenges: choose the language
lc test 207                 # SQL: your answer.sql is diffed against the reference
lc hint 013 [n]             # next hint
lc solution 013             # after you're done (or stuck)
lc done 401                 # mark labs/quizzes done by hand
lc progress | list [track]
lc reset 013                # restore the stub (needs a git commit to exist)
lc valgrind 105             # C under valgrind (Linux / lc box)
lc box test 103 c           # any lc command inside the Linux toolbox
lc verify                   # maintainer check: every reference solution passes
```

Suggested order: alternate a TS problem with one sys challenge or SQL exercise each day, and do the labs and quizzes 404/405 before Postgres-heavy interviews.

## How the C harness works

- `-fsanitize=address,undefined` plus `sys/common/track_alloc.h`, which is force-included and wraps malloc/free. Tests check for leaks (`CHECK_NO_LEAKS`), double frees and peak memory (110 has a memory budget).
- Each test runs in a forked child, so a segfault reports `CRASH` for that test and the rest keep running. Set `LC_NOFORK=1` to debug in-process (gdb/lldb).
- Each sys README ends with **C vs Rust** notes. Solve it in C first, then in Rust, and compare.

## Gotcha: macOS + Nix + Rust panics

When `cc` resolves to Nix's gcc on macOS, every Rust panic aborts with `failed to initiate panic, error 5`. `.cargo/config.toml` fixes this by linking with `/usr/bin/clang`. Copy that file into other Rust projects if you see the same error.
