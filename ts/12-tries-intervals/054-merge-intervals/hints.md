merge: sort by start; `if (cur.start <= last.end) last.end = max(last.end, cur.end) else push cur`.
---
insert: three phases — copy intervals ending before `new.start`; merge all intervals with
`start <= new.end` into `new`; copy the rest.
