# 008 · Valid Palindrome

> **Difficulty:** Easy · **Topics:** two pointers · **Target:** O(n) time, O(1) space

A phrase is a palindrome if, after lowercasing and removing every non-alphanumeric
character, it reads the same forwards and backwards. Return whether `s` is one.

Do it **without** building a cleaned copy of the string (O(1) extra space).

Then: `validPalindromeOneDelete(s)` — can `s` (plain lowercase letters) become a palindrome
by deleting **at most one** character?

## Examples

```
isPalindrome("A man, a plan, a canal: Panama") -> true
isPalindrome("race a car")                     -> false
validPalindromeOneDelete("abca")               -> true   (delete 'c' or 'b')
validPalindromeOneDelete("abc")                -> false
```
