`JOIN bookings b ON b.room = a.room AND a.id < b.id AND a.during && b.during`.
---
`extract(epoch FROM upper(a.during * b.during) - lower(a.during * b.during)) / 3600`.
