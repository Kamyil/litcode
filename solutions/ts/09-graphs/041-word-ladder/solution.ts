export function ladderLength(begin: string, end: string, wordList: string[]): number {
  const unvisited = new Set(wordList);
  if (!unvisited.has(end)) return 0;
  let level = [begin];
  for (let steps = 1; level.length; steps++) {
    const next: string[] = [];
    for (const word of level) {
      if (word === end) return steps;
      for (let i = 0; i < word.length; i++) {
        for (let c = 97; c <= 122; c++) {
          const cand = word.slice(0, i) + String.fromCharCode(c) + word.slice(i + 1);
          if (unvisited.delete(cand)) next.push(cand);
        }
      }
    }
    level = next;
  }
  return 0;
}
