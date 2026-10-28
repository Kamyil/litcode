# 055 · Meeting Rooms I & II

> **Difficulty:** Medium · **Topics:** sweep line, heap · **Target:** O(n log n)

Meetings are half-open intervals `[start, end)` — a meeting ending at 10 and one starting at
10 don't conflict.

- `canAttendAll(meetings)`: can one person attend every meeting?
- `minMeetingRooms(meetings)`: the minimum number of rooms needed.

## Think about

- Rooms = maximum number of meetings overlapping at any instant.
- Sweep line: split into `+1` events at starts and `-1` events at ends, sort (ends before
  starts at the same time!), and track the running sum. Or: sort starts and ends separately
  and walk both with two pointers. Or: min-heap of end times.
- SQL version of the same question (SQL track): `count(*) OVER (ORDER BY t)` over a union of
  start/end events — window functions are sweep lines.
