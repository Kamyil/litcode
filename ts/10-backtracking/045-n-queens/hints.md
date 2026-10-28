`place(row)`: if `row === n` record; for each `col`, skip if `cols.has(col) || diag.has(row - col) || anti.has(row + col)`.
---
Bitmask count: `go(cols, diag, anti)`; `free = ~(cols | diag | anti) & full`; while free: take the
lowest bit `b`, recurse with `(cols | b, (diag | b) << 1, (anti | b) >> 1)`, `free &= free - 1`.
