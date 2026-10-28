`l = 0, r = n - 1`. Compute the area, then move the pointer at the **shorter** line inward.
---
Why: the area is capped by the shorter line. Moving the taller one can only shrink the width
while the cap stays the same or gets lower — it can never produce a better answer.
