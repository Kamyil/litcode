import { expect, test } from "bun:test";
import { canAttendAll, minMeetingRooms } from "./solution";

test("canAttendAll", () => {
  expect(canAttendAll([[0, 30], [5, 10], [15, 20]])).toBe(false);
  expect(canAttendAll([[7, 10], [2, 4]])).toBe(true);
  expect(canAttendAll([[1, 5], [5, 10]])).toBe(true);
  expect(canAttendAll([])).toBe(true);
});

test("minMeetingRooms", () => {
  expect(minMeetingRooms([[0, 30], [5, 10], [15, 20]])).toBe(2);
  expect(minMeetingRooms([[7, 10], [2, 4]])).toBe(1);
  expect(minMeetingRooms([[1, 5], [5, 10], [10, 15]])).toBe(1);
  expect(minMeetingRooms([[1, 10], [2, 9], [3, 8], [4, 7]])).toBe(4);
  expect(minMeetingRooms([])).toBe(0);
});
