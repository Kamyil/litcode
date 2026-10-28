# 004 · Group Anagrams

> **Difficulty:** Medium · **Topics:** hash map, key design · **Target:** O(n · k)

Given an array of strings, group the anagrams together. Return the groups in any order;
the tests normalize order for you.

## Examples

```
["eat","tea","tan","ate","nat","bat"]  ->  [["bat"],["nat","tan"],["ate","eat","tea"]]
[""]                                   ->  [[""]]
```

## Think about

- The whole problem is: **what is the hash key** for "set of strings that are anagrams"?
- Sorted string as key: O(k log k) per word. Character-count signature: O(k) per word.
  Which is faster in practice for short words? Measure it.
