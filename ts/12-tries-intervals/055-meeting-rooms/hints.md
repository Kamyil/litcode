canAttendAll: sort by start; any `meetings[i].start < meetings[i-1].end` is a conflict.
---
minMeetingRooms: `starts` and `ends` sorted separately. For each start, if `start >= ends[j]`
a room freed up (`j++`), otherwise you need a new room.
