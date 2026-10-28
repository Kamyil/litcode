//! External merge sort of a file of little-endian i64 values. See ../README.md.
#![allow(dead_code, unused_variables, unused_imports)]

use std::cmp::Reverse;
use std::collections::BinaryHeap;
use std::fs::File;
use std::io::{self, BufReader, BufWriter, Read, Write};
use std::path::Path;

/// Returns the number of runs created. Run files in `tmp_dir` are removed before returning.
pub fn external_sort(input: &Path, output: &Path, tmp_dir: &Path, mem_limit: usize) -> io::Result<usize> {
    todo!()
}
