# 110 · External Merge Sort (what `work_mem` is about)

> **Difficulty:** Hard · **Languages:** C + Rust · **Topics:** file I/O, buffering, k-way merge with a heap, bounded memory

How do you sort 100 GB with 1 GB of RAM? Postgres hits this every time a sort doesn't fit in
`work_mem` and `EXPLAIN ANALYZE` prints `Sort Method: external merge  Disk: 51200kB`:

1. **Run generation:** read as much as fits in memory, sort it, write it out as a sorted *run*.
   Repeat until the input is exhausted → `ceil(n / mem_limit)` runs.
2. **Merge:** open all runs, keep one "current" value per run in a **min-heap**, repeatedly
   pop the smallest, write it, and refill from the run it came from. (TS 035 merge-k-lists, on disk.)

(`src/backend/utils/sort/tuplesort.c` and `logtape.c` — the header comments explain "tapes",
a term from when runs really lived on tape drives.)

## The task (both languages)

Sort a binary file of little-endian `int64` values into `output_path`:

- Use at most `mem_limit` values of in-memory buffer for run generation.
- Write runs as files inside `tmp_dir`; **delete them all** before returning (the test checks
  the directory is empty again).
- Merge all runs in one pass with a min-heap of size k = number of runs.
- Return the number of runs created (0 for an empty input). An empty input still creates an
  empty output file.

**C** (`c/xsort.c`): use `FILE*` + `fread`/`fwrite`, `qsort` for runs, and your own small heap.
The test measures your **peak heap usage** with the allocation tracker: it must stay around
`mem_limit * 8` bytes (plus a few bytes per run) — reading the whole file into memory fails.

**Rust** (`rust/src/lib.rs`): `BufReader`/`BufWriter`, `sort_unstable`, and
`std::collections::BinaryHeap<Reverse<(i64, usize)>>`. A separate test file installs a
counting `#[global_allocator]` to measure your peak memory the same way.

## Think about

- Why is a **single** merge pass optimal here but not always? With k runs you need k read
  buffers in memory at once. If k is too large, merge in multiple passes (merge 16 runs into 1,
  repeat). Postgres computes the max merge order from `work_mem`.
- **Replacement selection** (an old trick) produces runs about 2x the memory size on random
  input, using a heap during run generation. Postgres used it for years, then switched to
  quicksorting runs (PG 9.6+). Cache-friendly quicksort beat the heap's longer runs. Interviewers love this story.
- Reading 8 bytes per `fread` call vs buffered I/O: measure the difference (`time lc test 110 c`).
- In the SQL lab you'll watch the switch happen: `SET work_mem = '64kB'` vs `'256MB'` on the same `ORDER BY`.
