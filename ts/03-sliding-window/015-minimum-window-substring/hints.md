`need`: map char -> how many more of it the window needs. `missing = t.length`.
---
Adding `s[right]`: if `need[c] > 0` then `missing--`; always `need[c]--` (it may go negative = surplus).
---
While `missing === 0`: record window, then remove `s[left]`: `need[c]++`; if it becomes `> 0`, `missing++`.
