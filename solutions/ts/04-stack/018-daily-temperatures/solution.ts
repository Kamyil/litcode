export function dailyTemperatures(temps: number[]): number[] {
  const out = new Array<number>(temps.length).fill(0);
  const stack: number[] = []; // indices, temperatures decreasing
  for (let i = 0; i < temps.length; i++) {
    while (stack.length && temps[i] > temps[stack[stack.length - 1]]) {
      const j = stack.pop()!;
      out[j] = i - j;
    }
    stack.push(i);
  }
  return out;
}
