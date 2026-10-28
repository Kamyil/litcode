export function characterReplacement(s: string, k: number): number {
  const counts = new Int32Array(26);
  let left = 0;
  let maxCount = 0;
  let best = 0;
  for (let right = 0; right < s.length; right++) {
    maxCount = Math.max(maxCount, ++counts[s.charCodeAt(right) - 65]);
    if (right - left + 1 - maxCount > k) counts[s.charCodeAt(left++) - 65]--;
    best = Math.max(best, right - left + 1);
  }
  return best;
}
