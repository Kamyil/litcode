`Map<number, Node>` where `Node = { key, value, prev, next }`. Keep the key in the node — you
need it to delete the map entry when evicting.
---
Two helpers make everything trivial: `unlink(node)` and `pushFront(node)`.
---
Evict = `tail.prev` (the node just before the tail sentinel).
