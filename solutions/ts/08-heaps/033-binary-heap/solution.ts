export class Heap<T> {
  private a: T[] = [];

  constructor(private cmp: (a: T, b: T) => number) {}

  static from<T>(items: T[], cmp: (a: T, b: T) => number): Heap<T> {
    const h = new Heap(cmp);
    h.a = [...items];
    for (let i = (h.a.length >> 1) - 1; i >= 0; i--) h.siftDown(i);
    return h;
  }

  get size(): number {
    return this.a.length;
  }

  peek(): T | undefined {
    return this.a[0];
  }

  push(x: T): void {
    this.a.push(x);
    this.siftUp(this.a.length - 1);
  }

  pop(): T | undefined {
    const a = this.a;
    if (a.length === 0) return undefined;
    const top = a[0];
    const last = a.pop()!;
    if (a.length) {
      a[0] = last;
      this.siftDown(0);
    }
    return top;
  }

  private siftUp(i: number): void {
    const a = this.a;
    while (i > 0) {
      const p = (i - 1) >> 1;
      if (this.cmp(a[i], a[p]) >= 0) break;
      [a[i], a[p]] = [a[p], a[i]];
      i = p;
    }
  }

  private siftDown(i: number): void {
    const a = this.a;
    const n = a.length;
    for (;;) {
      const l = 2 * i + 1;
      const r = l + 1;
      let m = i;
      if (l < n && this.cmp(a[l], a[m]) < 0) m = l;
      if (r < n && this.cmp(a[r], a[m]) < 0) m = r;
      if (m === i) return;
      [a[i], a[m]] = [a[m], a[i]];
      i = m;
    }
  }
}
