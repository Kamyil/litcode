export function minDistance(a: string, b: string): number {
  const row = Array.from({ length: b.length + 1 }, (_, j) => j);
  for (let i = 1; i <= a.length; i++) {
    let diag = row[0]; // dp[i-1][j-1]
    row[0] = i;
    for (let j = 1; j <= b.length; j++) {
      const up = row[j]; // dp[i-1][j]
      row[j] = a[i - 1] === b[j - 1] ? diag : 1 + Math.min(diag, up, row[j - 1]);
      diag = up;
    }
  }
  return row[b.length];
}
