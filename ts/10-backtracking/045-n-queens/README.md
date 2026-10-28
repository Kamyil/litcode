# 045 · N-Queens

> **Difficulty:** Hard · **Topics:** backtracking, constraint sets, bitmasks

Place `n` queens on an `n x n` board so that none attack each other.

- `solveNQueens(n)`: all solutions as boards of strings (`"Q"` and `"."`), e.g. for n = 4:
  `[[".Q..","...Q","Q...","..Q."], ["..Q.","Q...","...Q",".Q.."]]`
- `totalNQueens(n)`: just the count — make this one fast with **bitmasks** (n = 12 has
  14200 solutions; the test expects it quickly).

## Think about

- One queen per row; for each row, try each column not attacked. Track attacked columns,
  `row - col` diagonals and `row + col` anti-diagonals in sets or bitmasks.
- Bitmask version: `available = ~(cols | diag | anti) & ((1 << n) - 1)`; iterate set bits
  with `bit = available & -available`. When moving to the next row: `diag << 1`, `anti >> 1`.
