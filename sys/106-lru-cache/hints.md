C node: `typedef struct Node { int64_t key, value; struct Node *hnext, *prev, *next; } Node;`
Use a sentinel `Node head` embedded in the cache struct: `head.next` = MRU, `head.prev` = LRU.
---
C bucket lookup: `Node **slot = &c->buckets[hash(key) & mask]; while (*slot && (*slot)->key != key) slot = &(*slot)->hnext;`
The pointer-to-pointer lets you unlink from the chain with `*slot = (*slot)->hnext` — no special case for the first node.
---
C hash for int64: multiply by a big odd constant and take the high bits, e.g.
`(uint64_t)key * 0x9E3779B97F4A7C15ULL >> 32` (Fibonacci hashing).
---
Rust: `struct Node<K, V> { key: K, value: V, prev: usize, next: usize }`. Use index 0 (or a
`NIL = usize::MAX` constant) as the sentinel. Keep `map: HashMap<K, usize>`.
---
Rust eviction: take the LRU index, unlink it, `map.remove(&node.key)`, and reuse the slot for the new
entry with `std::mem::replace(&mut self.nodes[i], new_node)` — which hands you the old key and value to return.
