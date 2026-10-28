For every element `x` you are really asking: "have I already seen `target - x`?"
---
A `Map<number, number>` from value -> index answers that question in O(1).
---
Check the map *before* inserting the current element, so you never pair an element with itself.
