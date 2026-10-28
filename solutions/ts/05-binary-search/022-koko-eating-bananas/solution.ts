export function minEatingSpeed(piles: number[], h: number): number {
  const hours = (k: number) => piles.reduce((sum, p) => sum + Math.ceil(p / k), 0);
  let lo = 1;
  let hi = Math.max(...piles);
  while (lo < hi) {
    const mid = Math.floor((lo + hi) / 2);
    if (hours(mid) <= h) hi = mid;
    else lo = mid + 1;
  }
  return lo;
}
