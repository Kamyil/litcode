`parent[i] = i`, `size[i] = 1`. `find`: walk up to the root, then point every node on the way at the root.
---
`union`: find both roots; if equal return false; attach the smaller tree under the bigger; `count--`.
---
Redundant connection: the first edge whose endpoints are already connected.
