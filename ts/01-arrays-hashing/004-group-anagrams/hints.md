Two strings are anagrams iff their sorted characters are equal: `[...w].sort().join("")`.
---
Use `Map<string, string[]>` from key -> group. For an O(k) key, build a 26-slot count array
and join it with a separator: `"1#0#0#2#..."` (the separator matters — why?).
