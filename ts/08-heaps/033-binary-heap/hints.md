push: append to the end, then sift up while `cmp(item, parent) < 0`.
---
pop: save `a[0]`, move the last element to index 0, then sift down: swap with the smaller child
while that child is smaller than the element.
---
from: copy the array, then call siftDown for `i = (n >> 1) - 1` down to `0`.
