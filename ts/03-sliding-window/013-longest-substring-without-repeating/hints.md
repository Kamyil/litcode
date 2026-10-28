Window `[left, right]`. Keep a `Map<char, lastIndex>`.
---
When `s[right]` was last seen at index `j >= left`, the window is invalid: set `left = j + 1`.
---
Answer = max over all steps of `right - left + 1`.
