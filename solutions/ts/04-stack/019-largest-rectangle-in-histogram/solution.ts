export function largestRectangleArea(heights: number[]): number {
  const stack: number[] = [];
  let best = 0;
  for (let i = 0; i <= heights.length; i++) {
    const h = i === heights.length ? 0 : heights[i];
    while (stack.length && h < heights[stack[stack.length - 1]]) {
      const j = stack.pop()!;
      const width = stack.length ? i - stack[stack.length - 1] - 1 : i;
      best = Math.max(best, heights[j] * width);
    }
    stack.push(i);
  }
  return best;
}
