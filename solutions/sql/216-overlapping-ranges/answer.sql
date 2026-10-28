SELECT a.room,
       count(*) AS overlapping_pairs,
       round(sum(extract(epoch FROM upper(a.during * b.during) - lower(a.during * b.during))) / 3600, 1) AS overlap_hours
FROM bookings a
JOIN bookings b ON b.room = a.room AND a.id < b.id AND a.during && b.during
GROUP BY a.room
ORDER BY overlapping_pairs DESC, a.room;
