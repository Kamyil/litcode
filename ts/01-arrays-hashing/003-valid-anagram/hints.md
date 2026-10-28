Different lengths -> immediately `false`.
---
One `Int32Array(26)`: increment for each char of `s`, decrement for each char of `t`.
`s.charCodeAt(i) - 97` gives the slot. All zeros at the end means anagram.
