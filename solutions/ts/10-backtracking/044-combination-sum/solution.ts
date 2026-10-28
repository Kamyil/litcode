function search(candidates: number[], target: number, reuse: boolean): number[][] {
  const c = [...candidates].sort((a, b) => a - b);
  const out: number[][] = [];
  const cur: number[] = [];
  const go = (start: number, remaining: number) => {
    if (remaining === 0) {
      out.push([...cur]);
      return;
    }
    for (let i = start; i < c.length && c[i] <= remaining; i++) {
      if (!reuse && i > start && c[i] === c[i - 1]) continue;
      cur.push(c[i]);
      go(reuse ? i : i + 1, remaining - c[i]);
      cur.pop();
    }
  };
  go(0, target);
  return out;
}

export function combinationSum(candidates: number[], target: number): number[][] {
  return search(candidates, target, true);
}

export function combinationSum2(candidates: number[], target: number): number[][] {
  return search(candidates, target, false);
}
