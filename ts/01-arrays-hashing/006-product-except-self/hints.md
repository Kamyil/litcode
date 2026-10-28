First pass left-to-right: `out[i] = product of nums[0..i-1]`.
---
Second pass right-to-left with a single running variable `suffix`: `out[i] *= suffix; suffix *= nums[i]`.
---
Watch out: JS has `-0`. `expect([-0]).toEqual([0])` fails in some matchers — the tests here
normalize it, but know that `0 * -3 === -0`.
