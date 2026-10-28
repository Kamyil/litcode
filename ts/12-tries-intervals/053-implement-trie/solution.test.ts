import { expect, test } from "bun:test";
import { Trie } from "./solution";

test("basic", () => {
  const t = new Trie();
  t.insert("apple");
  expect(t.search("apple")).toBe(true);
  expect(t.search("app")).toBe(false);
  expect(t.startsWith("app")).toBe(true);
  t.insert("app");
  expect(t.search("app")).toBe(true);
  expect(t.startsWith("b")).toBe(false);
  expect(t.startsWith("")).toBe(true);
});

test("autocomplete", () => {
  const t = new Trie();
  for (const w of ["car", "card", "care", "careful", "cat", "dog", "ca"]) t.insert(w);
  expect(t.wordsWithPrefix("car")).toEqual(["car", "card", "care", "careful"]);
  expect(t.wordsWithPrefix("ca")).toEqual(["ca", "car", "card", "care", "careful", "cat"]);
  expect(t.wordsWithPrefix("x")).toEqual([]);
  expect(t.wordsWithPrefix("").length).toBe(7);
});
