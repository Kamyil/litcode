use ring_buffer::RingBuffer;
use std::cell::Cell;
use std::rc::Rc;

#[test]
#[should_panic]
fn rejects_non_power_of_two() {
    RingBuffer::<u8>::with_capacity(12);
}

#[test]
fn fifo() {
    let mut rb = RingBuffer::with_capacity(4);
    assert!(rb.is_empty());
    for i in 0..4 {
        assert!(rb.push(i).is_ok());
    }
    assert!(rb.is_full());
    assert_eq!(rb.push(99), Err(99));
    assert_eq!(rb.peek(), Some(&0));
    assert_eq!(rb.pop(), Some(0));
    assert!(rb.push(4).is_ok()); // wraps
    assert_eq!(rb.iter().copied().collect::<Vec<_>>(), vec![1, 2, 3, 4]);
    assert_eq!(std::iter::from_fn(|| rb.pop()).collect::<Vec<_>>(), vec![1, 2, 3, 4]);
    assert_eq!(rb.pop(), None);
}

#[test]
fn long_stream() {
    let mut rb = RingBuffer::with_capacity(8);
    let (mut next_in, mut next_out) = (0u32, 0u32);
    for step in 0..10_000u32 {
        for _ in 0..(step % 5) {
            if rb.push(next_in).is_ok() {
                next_in += 1;
            }
        }
        for _ in 0..(step % 4) {
            if let Some(x) = rb.pop() {
                assert_eq!(x, next_out);
                next_out += 1;
            }
        }
        assert_eq!(rb.len() as u32, next_in - next_out);
    }
}

#[test]
fn owns_strings() {
    let mut rb = RingBuffer::with_capacity(2);
    rb.push(String::from("a")).unwrap();
    rb.push(String::from("b")).unwrap();
    let rejected = rb.push(String::from("c")).unwrap_err();
    assert_eq!(rejected, "c");
    assert_eq!(rb.pop().as_deref(), Some("a"));
}

struct DropCounter(Rc<Cell<usize>>);
impl Drop for DropCounter {
    fn drop(&mut self) {
        self.0.set(self.0.get() + 1);
    }
}

#[test]
fn drops_only_live_elements() {
    let drops = Rc::new(Cell::new(0));
    {
        let mut rb = RingBuffer::with_capacity(8);
        for _ in 0..6 {
            let _ = rb.push(DropCounter(drops.clone()));
        }
        drop(rb.pop());
        drop(rb.pop());
        assert_eq!(drops.get(), 2);
        for _ in 0..3 {
            let _ = rb.push(DropCounter(drops.clone()));
        } // wrapped: 7 live
    }
    assert_eq!(drops.get(), 9, "every element dropped exactly once");
}
