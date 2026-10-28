function isAlnum(c: number): boolean {
  return (c >= 48 && c <= 57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122);
}

function lower(c: number): number {
  return c >= 65 && c <= 90 ? c + 32 : c;
}

export function isPalindrome(s: string): boolean {
  let l = 0;
  let r = s.length - 1;
  while (l < r) {
    if (!isAlnum(s.charCodeAt(l))) l++;
    else if (!isAlnum(s.charCodeAt(r))) r--;
    else if (lower(s.charCodeAt(l++)) !== lower(s.charCodeAt(r--))) return false;
  }
  return true;
}

function isRangePalindrome(s: string, l: number, r: number): boolean {
  while (l < r) if (s[l++] !== s[r--]) return false;
  return true;
}

export function validPalindromeOneDelete(s: string): boolean {
  let l = 0;
  let r = s.length - 1;
  while (l < r) {
    if (s[l] !== s[r]) return isRangePalindrome(s, l + 1, r) || isRangePalindrome(s, l, r - 1);
    l++;
    r--;
  }
  return true;
}
