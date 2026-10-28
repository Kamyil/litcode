type Node = { key: number; value: number; prev: Node; next: Node };

export class LRUCache {
  private map = new Map<number, Node>();
  private head: Node; // sentinel, head.next = most recent
  private tail: Node; // sentinel, tail.prev = least recent

  constructor(private capacity: number) {
    this.head = { key: 0, value: 0 } as Node;
    this.tail = { key: 0, value: 0 } as Node;
    this.head.next = this.tail;
    this.tail.prev = this.head;
  }

  private unlink(n: Node): void {
    n.prev.next = n.next;
    n.next.prev = n.prev;
  }

  private pushFront(n: Node): void {
    n.prev = this.head;
    n.next = this.head.next;
    this.head.next.prev = n;
    this.head.next = n;
  }

  get(key: number): number {
    const n = this.map.get(key);
    if (!n) return -1;
    this.unlink(n);
    this.pushFront(n);
    return n.value;
  }

  put(key: number, value: number): void {
    let n = this.map.get(key);
    if (n) {
      n.value = value;
      this.unlink(n);
    } else {
      n = { key, value } as Node;
      this.map.set(key, n);
    }
    this.pushFront(n);
    if (this.map.size > this.capacity) {
      const lru = this.tail.prev;
      this.unlink(lru);
      this.map.delete(lru.key);
    }
  }
}
