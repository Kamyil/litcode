type Node = { children: Map<string, Node>; end: boolean };

const node = (): Node => ({ children: new Map(), end: false });

export class Trie {
  private root = node();

  private walk(s: string): Node | null {
    let n: Node | undefined = this.root;
    for (const c of s) {
      n = n.children.get(c);
      if (!n) return null;
    }
    return n;
  }

  insert(word: string): void {
    let n = this.root;
    for (const c of word) {
      let next = n.children.get(c);
      if (!next) n.children.set(c, (next = node()));
      n = next;
    }
    n.end = true;
  }

  search(word: string): boolean {
    return this.walk(word)?.end ?? false;
  }

  startsWith(prefix: string): boolean {
    return this.walk(prefix) !== null;
  }

  wordsWithPrefix(prefix: string): string[] {
    const out: string[] = [];
    const dfs = (n: Node, acc: string) => {
      if (n.end) out.push(acc);
      for (const c of [...n.children.keys()].sort()) dfs(n.children.get(c)!, acc + c);
    };
    const start = this.walk(prefix);
    if (start) dfs(start, prefix);
    return out;
  }
}
