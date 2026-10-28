use slotted_page::{Page, PAGE_SIZE};

fn u16_at(p: &Page, off: usize) -> u16 {
    u16::from_le_bytes([p.as_bytes()[off], p.as_bytes()[off + 1]])
}

fn align8(n: usize) -> usize {
    (n + 7) & !7
}

/// Free space after compaction, computed from the raw bytes.
fn free_after_compact(p: &Page) -> usize {
    let lower = u16_at(p, 0) as usize;
    let n = u16_at(p, 4) as usize;
    let used: usize = (0..n).map(|i| align8(u16_at(p, 8 + 4 * i + 2) as usize)).sum();
    PAGE_SIZE - lower - used
}

#[test]
fn init() {
    let p = Page::new();
    assert_eq!(u16_at(&p, 0), 8);
    assert_eq!(u16_at(&p, 2) as usize, PAGE_SIZE);
    assert_eq!(p.num_slots(), 0);
    assert_eq!(p.free_space(), PAGE_SIZE - 8);
    assert_eq!(p.get(0), None);
}

#[test]
fn byte_layout_matches_spec() {
    let mut p = Page::new();
    assert_eq!(p.insert(b"abc"), Some(0));
    assert_eq!(p.insert(b"hello world"), Some(1));
    assert_eq!(u16_at(&p, 0), 16); // lower
    assert_eq!(u16_at(&p, 2) as usize, PAGE_SIZE - 8 - 16); // upper
    assert_eq!(u16_at(&p, 4), 2); // nslots
    assert_eq!(u16_at(&p, 8) as usize, PAGE_SIZE - 8); // slot 0 offset
    assert_eq!(u16_at(&p, 10), 3); // slot 0 len
    assert_eq!(u16_at(&p, 12) as usize, PAGE_SIZE - 24);
    assert_eq!(&p.as_bytes()[PAGE_SIZE - 8..PAGE_SIZE - 5], b"abc");
    assert_eq!(p.get(1), Some(&b"hello world"[..]));
}

#[test]
fn fill_until_full() {
    let mut p = Page::new();
    let mut n = 0;
    while p.insert(&[b'x'; 100]).is_some() {
        n += 1;
    }
    assert_eq!(n, (PAGE_SIZE - 8) / 108);
}

#[test]
fn delete_reuse_auto_compaction() {
    let mut p = Page::new();
    for i in 0..75u8 {
        p.insert(&[i; 100]).unwrap();
    }
    assert!(p.delete(3));
    assert!(!p.delete(3));
    assert!(!p.delete(500));
    assert_eq!(p.insert(&[255; 100]), Some(3));
    for i in 0..75u16 {
        let want = if i == 3 { 255 } else { i as u8 };
        assert_eq!(p.get(i), Some(&[want; 100][..]));
    }
}

#[test]
fn compact_keeps_slots() {
    let mut p = Page::new();
    for i in 0..40 {
        p.insert(format!("tuple number {i}").as_bytes());
    }
    for i in (0..40).step_by(3) {
        p.delete(i);
    }
    let expected = free_after_compact(&p);
    p.compact();
    assert_eq!(p.free_space(), expected);
    for i in 0..40u16 {
        let got = p.get(i);
        if i % 3 == 0 {
            assert_eq!(got, None);
        } else {
            assert_eq!(got, Some(format!("tuple number {i}").as_bytes()));
        }
    }
}

#[test]
fn update() {
    let mut p = Page::new();
    p.insert(b"aaaa");
    p.insert(b"bbbb");
    let upper = u16_at(&p, 2);
    assert!(p.update(0, b"AAAAAAA"));
    assert_eq!(u16_at(&p, 2), upper, "fits in the old aligned space: in place");
    assert!(p.update(0, b"a much longer tuple value"));
    assert_eq!(p.get(0), Some(&b"a much longer tuple value"[..]));
    assert_eq!(p.get(1), Some(&b"bbbb"[..]));
    assert!(!p.update(9, b"x"));
}

#[test]
fn failed_update_leaves_page_unchanged() {
    let mut p = Page::new();
    p.insert(&[b'z'; 4000]);
    p.insert(b"keep me");
    let before = *p.as_bytes();
    assert!(!p.update(1, &[0; 8000]));
    assert_eq!(&before[..], &p.as_bytes()[..]);
    p.delete(0);
    assert!(p.update(1, &[b'm'; 4100]));
    assert_eq!(p.get(1).map(|t| t.len()), Some(4100));
}

#[test]
fn random_against_model() {
    let mut p = Page::new();
    let mut model: Vec<Option<Vec<u8>>> = Vec::new();
    let mut s: u64 = 1;
    for step in 0..20_000u64 {
        s = s.wrapping_mul(6364136223846793005).wrapping_add(1442695040888963407);
        let len = ((s >> 40) % 40 + 1) as usize;
        let data: Vec<u8> = (0..len).map(|i| b'a' + ((step as usize + i) % 26) as u8).collect();
        let n = p.num_slots() as usize;
        match (s >> 20) % 3 {
            0 => match p.insert(&data) {
                Some(slot) => {
                    let slot = slot as usize;
                    if slot == model.len() {
                        model.push(None);
                    }
                    model[slot] = Some(data);
                }
                None => assert!(free_after_compact(&p) < align8(len) + 4, "refused an insert that fits after compaction"),
            },
            _ if n == 0 => {}
            1 => {
                let slot = (s >> 4) as usize % n;
                assert_eq!(p.delete(slot as u16), model[slot].is_some());
                model[slot] = None;
            }
            _ => {
                let slot = (s >> 4) as usize % n;
                if p.update(slot as u16, &data) {
                    model[slot] = Some(data);
                }
            }
        }
        if step % 97 == 0 {
            for (i, m) in model.iter().enumerate() {
                assert_eq!(p.get(i as u16), m.as_deref(), "slot {i} at step {step}");
            }
        }
    }
}
