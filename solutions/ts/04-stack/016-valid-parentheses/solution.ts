const PAIRS: Record<string, string> = { ")": "(", "]": "[", "}": "{" };

export function isValid(s: string): boolean {
  const stack: string[] = [];
  for (const c of s) {
    if (c in PAIRS) {
      if (stack.pop() !== PAIRS[c]) return false;
    } else stack.push(c);
  }
  return stack.length === 0;
}
