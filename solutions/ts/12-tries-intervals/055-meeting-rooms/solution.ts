export function canAttendAll(meetings: [number, number][]): boolean {
  const sorted = [...meetings].sort((a, b) => a[0] - b[0]);
  for (let i = 1; i < sorted.length; i++) if (sorted[i][0] < sorted[i - 1][1]) return false;
  return true;
}

export function minMeetingRooms(meetings: [number, number][]): number {
  const starts = meetings.map((m) => m[0]).sort((a, b) => a - b);
  const ends = meetings.map((m) => m[1]).sort((a, b) => a - b);
  let rooms = 0;
  let j = 0;
  for (const s of starts) {
    if (s >= ends[j]) j++; // reuse the room that freed up earliest
    else rooms++;
  }
  return rooms;
}
