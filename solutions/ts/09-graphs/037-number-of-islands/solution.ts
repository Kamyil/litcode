// Returns the size of every island, found with an iterative flood fill.
function islandSizes(grid: string[][]): number[] {
  const rows = grid.length;
  const cols = rows ? grid[0].length : 0;
  const seen = new Uint8Array(rows * cols);
  const sizes: number[] = [];
  for (let start = 0; start < rows * cols; start++) {
    if (seen[start] || grid[Math.floor(start / cols)][start % cols] !== "1") continue;
    seen[start] = 1;
    const stack = [start];
    let size = 0;
    while (stack.length) {
      const cell = stack.pop()!;
      size++;
      const r = Math.floor(cell / cols);
      const c = cell % cols;
      for (const [nr, nc] of [[r + 1, c], [r - 1, c], [r, c + 1], [r, c - 1]]) {
        if (nr < 0 || nc < 0 || nr >= rows || nc >= cols) continue;
        const next = nr * cols + nc;
        if (!seen[next] && grid[nr][nc] === "1") {
          seen[next] = 1;
          stack.push(next);
        }
      }
    }
    sizes.push(size);
  }
  return sizes;
}

export function numIslands(grid: string[][]): number {
  return islandSizes(grid).length;
}

export function maxAreaOfIsland(grid: string[][]): number {
  return Math.max(0, ...islandSizes(grid));
}
