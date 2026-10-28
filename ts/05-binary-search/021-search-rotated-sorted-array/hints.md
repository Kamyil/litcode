findMin: if `nums[mid] > nums[hi]` the minimum is strictly right of mid; else it is at mid or left.
---
search: if `nums[lo] <= nums[mid]`, the left half is sorted — check `nums[lo] <= target < nums[mid]`.
Otherwise the right half is sorted — check `nums[mid] < target <= nums[hi]`.
