Build adjacency lists `b -> a` and an in-degree array.
---
Queue every course with in-degree 0. Pop one, append it to the order, decrement in-degrees of its
neighbors; enqueue those that drop to 0.
---
If the order has fewer than `n` courses, there's a cycle.
