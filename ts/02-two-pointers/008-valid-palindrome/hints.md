Pointers `l = 0`, `r = n - 1`. Skip non-alphanumerics on each side, compare lowercase, move inward.
---
A regex test per char like `/[a-z0-9]/i.test(ch)` is fine; a char-code range check is faster.
---
One-delete: at the first mismatch you have two choices — skip `l` or skip `r`. Check whether
either remaining range `[l+1, r]` or `[l, r-1]` is a plain palindrome.
