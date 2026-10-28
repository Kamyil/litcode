use arena_allocator::Arena;

#[test]
fn lazy() {
    let a = Arena::new(1024);
    assert_eq!(a.block_count(), 0);
}

#[test]
fn values_survive_and_are_mutable() {
    let a = Arena::new(256);
    let mut refs = Vec::new();
    for i in 0..1000u64 {
        refs.push(a.alloc(i));
    }
    for (i, r) in refs.iter_mut().enumerate() {
        assert_eq!(**r, i as u64);
        **r += 1;
    }
    assert_eq!(*refs[999], 1000);
    assert!(a.block_count() > 1);
}

#[test]
fn alignment() {
    #[derive(Clone, Copy)]
    #[repr(align(64))]
    struct CacheLine([u8; 64]);

    let a = Arena::new(4096);
    for _ in 0..100 {
        let b = a.alloc(1u8);
        let w = a.alloc(7u128);
        let c = a.alloc(CacheLine([3; 64]));
        assert_eq!(*b, 1);
        assert_eq!(*w, 7);
        assert_eq!((w as *const u128 as usize) % std::mem::align_of::<u128>(), 0);
        assert_eq!((c as *const CacheLine as usize) % 64, 0);
        assert_eq!(c.0[63], 3);
    }
}

#[test]
fn one_block_for_small_things() {
    let a = Arena::new(64 * 1024);
    for i in 0..1000u64 {
        a.alloc(i);
    }
    assert_eq!(a.block_count(), 1);
}

#[test]
fn strings_and_slices() {
    let a = Arena::new(32);
    let s1 = a.alloc_str("hello");
    let s2 = a.alloc_str("a string longer than the thirty-two byte block size");
    let ys = a.alloc_slice_copy(&[1i32, 2, 3]);
    ys[0] = 10;
    assert_eq!(s1, "hello");
    assert_eq!(s2, "a string longer than the thirty-two byte block size");
    assert_eq!(ys, &[10, 2, 3]);
    assert_eq!(a.alloc_str(""), "");
    let empty: &mut [u64] = a.alloc_slice_copy(&[]);
    assert!(empty.is_empty());
}

#[test]
fn big_allocation() {
    let a = Arena::new(64);
    let big = a.alloc([7u8; 10_000]);
    assert_eq!(big[9_999], 7);
}

#[test]
fn reset() {
    let mut a = Arena::new(128);
    for i in 0..200u64 {
        a.alloc(i);
    }
    assert!(a.block_count() > 5);
    a.reset();
    assert_eq!(a.block_count(), 1);
    let x = a.alloc(5u32);
    assert_eq!(*x, 5);
    assert_eq!(a.block_count(), 1);
}

// This must NOT compile — uncomment to see the borrow checker stop a use-after-reset:
//
// #[test]
// fn use_after_reset() {
//     let mut a = Arena::new(128);
//     let x = a.alloc(1u8);
//     a.reset();
//     println!("{x}");
// }
