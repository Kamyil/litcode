import { expect, test } from "bun:test";
import { groupAnagrams } from "./solution";

const norm = (groups: string[][]) =>
  groups.map((g) => [...g].sort()).sort((a, b) => a.join(",").localeCompare(b.join(",")));

test("example", () => {
  expect(norm(groupAnagrams(["eat", "tea", "tan", "ate", "nat", "bat"]))).toEqual(
    norm([["bat"], ["nat", "tan"], ["ate", "eat", "tea"]]),
  );
});

test("edge cases", () => {
  expect(groupAnagrams([""])).toEqual([[""]]);
  expect(groupAnagrams(["a"])).toEqual([["a"]]);
  // count keys must not collide: "bdddddddddd" vs "bbbbbbbbbbc"
  expect(norm(groupAnagrams(["bdddddddddd", "bbbbbbbbbbc"]))).toEqual(norm([["bdddddddddd"], ["bbbbbbbbbbc"]]));
});
