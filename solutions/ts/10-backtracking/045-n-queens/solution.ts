export function solveNQueens(n: number): string[][] {
  const out: string[][] = [];
  const queens: number[] = []; // queens[row] = col
  const cols = new Set<number>();
  const diag = new Set<number>();
  const anti = new Set<number>();
  const place = (row: number) => {
    if (row === n) {
      out.push(queens.map((c) => ".".repeat(c) + "Q" + ".".repeat(n - c - 1)));
      return;
    }
    for (let col = 0; col < n; col++) {
      if (cols.has(col) || diag.has(row - col) || anti.has(row + col)) continue;
      cols.add(col);
      diag.add(row - col);
      anti.add(row + col);
      queens.push(col);
      place(row + 1);
      queens.pop();
      cols.delete(col);
      diag.delete(row - col);
      anti.delete(row + col);
    }
  };
  place(0);
  return out;
}

export function totalNQueens(n: number): number {
  const full = (1 << n) - 1;
  const go = (cols: number, diag: number, anti: number): number => {
    if (cols === full) return 1;
    let count = 0;
    let free = ~(cols | diag | anti) & full;
    while (free) {
      const bit = free & -free;
      free &= free - 1;
      count += go(cols | bit, ((diag | bit) << 1) & full, (anti | bit) >> 1);
    }
    return count;
  };
  return go(0, 0, 0);
}
