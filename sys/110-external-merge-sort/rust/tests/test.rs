mod common;

use common::Scratch;
use external_merge_sort::external_sort;
use std::path::Path;

#[test]
fn empty_input() {
    let s = Scratch::new("empty");
    s.write_input(0, 0, 1);
    assert_eq!(external_sort(&s.input, &s.output, &s.runs, 100).unwrap(), 0);
    s.check_output(0, 0);
}

#[test]
fn fits_in_memory() {
    let s = Scratch::new("fits");
    let sum = s.write_input(500, 1000, 2);
    assert_eq!(external_sort(&s.input, &s.output, &s.runs, 1000).unwrap(), 1);
    s.check_output(500, sum);
    assert_eq!(s.leftover_runs(), 0);
}

#[test]
fn many_runs_with_duplicates() {
    let s = Scratch::new("dups");
    let sum = s.write_input(100_000, 50, 3);
    assert_eq!(external_sort(&s.input, &s.output, &s.runs, 7000).unwrap(), 15);
    s.check_output(100_000, sum);
    assert_eq!(s.leftover_runs(), 0, "run files must be deleted");
}

#[test]
fn missing_input_is_an_error() {
    let s = Scratch::new("missing");
    assert!(external_sort(Path::new("/nonexistent/x.bin"), &s.output, &s.runs, 10).is_err());
}
