# 108 · Slotted Page (how Postgres stores rows)

> **Difficulty:** Hard · **Languages:** C + Rust · **Topics:** byte-level layout, alignment, fragmentation, stable identifiers

Every Postgres table and index is a file of **8 KB pages**. A heap page looks like this
(`src/include/storage/bufpage.h` — read the big comment at the top):

```
 0      8                      lower                     upper                 8192
 +------+-----+-----+-----+-----+ - - - free - - - - - - -+--------+--------+-----+
 |header| id0 | id1 | id2 | ... |                          | tuple2 | tuple1 |tup0 |
 +------+-----+-----+-----+-----+ - - - - - - - - - - - - +--------+--------+-----+
          line pointers grow  →                     ←  tuples grow down from the end
```

- The **line pointer array** (`ItemIdData`) grows forward from the header; each entry is
  `(offset, length)` of one tuple.
- **Tuples** are packed from the end of the page backward.
- Free space is the gap between `lower` (end of the line pointers) and `upper` (start of the tuples).
- A row's address is `(page number, slot number)` — that's the `ctid` you can `SELECT ctid FROM t`.
  Because indexes store ctids, **the slot number of a live tuple must never change**, even when
  tuples move inside the page.

You'll implement this exact layout (simplified) in both languages, **byte-compatible**:

| bytes | field |
|---|---|
| 0..2 | `lower` (u16, little-endian) = 8 + 4 · nslots |
| 2..4 | `upper` (u16) = start of tuple space, initially 8192 |
| 4..6 | `nslots` (u16) |
| 6..8 | `flags` (u16, always 0 here) |
| 8.. | `nslots` × line pointer: `offset` (u16), `len` (u16); `len == 0` means **unused** slot |

Rules:
1. Tuple starts are **8-byte aligned** (`MAXALIGN` in Postgres): place a tuple at
   `upper = align_down(upper - len, 8)`, which consumes `align_up(len, 8)` bytes.
2. `insert` reuses the **lowest unused slot** if any (costs no line pointer space), else appends a slot.
3. `delete` just marks the slot unused (`offset = len = 0`). Its bytes become *fragmented* free space.
4. If an insert doesn't fit in the contiguous gap but would fit after defragmentation,
   **compact first** (move live tuples together at the end of the page, keep slot numbers), then insert.
   Postgres does this during page pruning (`PageRepairFragmentation`).
5. `update(slot, data)`: if the new data fits in the old tuple's aligned size, overwrite in place.
   Otherwise relocate it within the page (slot number unchanged), compacting if needed. If it can't fit
   even after compaction, return failure and **leave the page unchanged**.
6. Tuples have length ≥ 1. Slots never get removed from the array (`lower` never shrinks).

## Part 1 — C (`c/page.c`)

Operate directly on a `uint8_t page[PAGE_SIZE]`, using the `PageHeader` / `ItemId` structs from
`page.h` via pointer casts (the tests give you an 8-byte-aligned buffer). No `malloc` needed.

## Part 2 — Rust (`rust/src/lib.rs`)

`Page { buf: Box<[u8; PAGE_SIZE]> }`. Read/write fields with `u16::from_le_bytes` /
`to_le_bytes` — fully safe code, no casts. `as_bytes()` exposes the raw page; a test checks the
layout is byte-identical to the spec above.

## Compare when done

- C: `(PageHeader *)page` is a type pun. It works here because the buffer is aligned and the struct
  has no padding, but strict-aliasing and alignment rules are a real minefield. Rust: explicit
  little-endian decoding — slower to write, portable, and no UB. (The `zerocopy` / `bytemuck`
  crates give you the cast safely, with compile-time checks.)
- Why can't Postgres just move a tuple to another page on update? (Index entries point at the old
  ctid.) This is why **HOT updates** exist: if no indexed column changed and the new version fits
  on the *same page*, Postgres chains old → new inside the page and skips index updates.
  `fillfactor < 100` reserves room for exactly that.
- Run `CREATE EXTENSION pageinspect; SELECT * FROM page_header(get_raw_page('t', 0));` and
  `SELECT lp, lp_off, lp_len FROM heap_page_items(get_raw_page('t', 0));` in the SQL track's lab
  and compare with your implementation.
