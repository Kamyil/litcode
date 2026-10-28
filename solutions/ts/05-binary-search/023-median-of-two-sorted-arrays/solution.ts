export function findMedianSortedArrays(a: number[], b: number[]): number {
  if (a.length > b.length) [a, b] = [b, a];
  const m = a.length;
  const n = b.length;
  const half = (m + n + 1) >> 1;
  let lo = 0;
  let hi = m;
  while (lo <= hi) {
    const i = (lo + hi) >> 1;
    const j = half - i;
    const aLeft = i > 0 ? a[i - 1] : -Infinity;
    const aRight = i < m ? a[i] : Infinity;
    const bLeft = j > 0 ? b[j - 1] : -Infinity;
    const bRight = j < n ? b[j] : Infinity;
    if (aLeft > bRight) hi = i - 1;
    else if (bLeft > aRight) lo = i + 1;
    else {
      const left = Math.max(aLeft, bLeft);
      if ((m + n) % 2) return left;
      return (left + Math.min(aRight, bRight)) / 2;
    }
  }
  throw new Error("inputs not sorted");
}
