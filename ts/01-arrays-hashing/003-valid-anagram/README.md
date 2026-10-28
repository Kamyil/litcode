# 003 · Valid Anagram

> **Difficulty:** Easy · **Topics:** counting, hash map · **Target:** O(n) time

Given strings `s` and `t`, return `true` if `t` is an anagram of `s` (same characters with
the same multiplicities, in any order).

## Examples

```
s = "anagram", t = "nagaram"  -> true
s = "rat",     t = "car"      -> false
```

## Constraints

- Strings contain lowercase `a-z` only.

## Think about

- With a fixed alphabet of 26, a counting array beats a hash map. Why? (memory locality,
  no hashing, no allocation per key)
- Follow-up: what if the input can contain any Unicode characters? What breaks in JS if you
  index the string with `s[i]`? (look up surrogate pairs; `for (const ch of s)` iterates code points)
