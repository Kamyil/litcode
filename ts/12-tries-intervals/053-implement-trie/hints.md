Walk character by character, creating children on insert. `search` = walk + check `end`;
`startsWith` = walk succeeds.
---
`wordsWithPrefix`: walk to the prefix node, then DFS visiting children in sorted key order,
collecting words where `end` is true.
