//! Reference external merge sort.
use std::cmp::Reverse;
use std::collections::BinaryHeap;
use std::fs::{self, File};
use std::io::{self, BufReader, BufWriter, Read, Write};
use std::path::{Path, PathBuf};

fn read_i64(r: &mut impl Read) -> io::Result<Option<i64>> {
    let mut b = [0u8; 8];
    match r.read_exact(&mut b) {
        Ok(()) => Ok(Some(i64::from_le_bytes(b))),
        Err(e) if e.kind() == io::ErrorKind::UnexpectedEof => Ok(None),
        Err(e) => Err(e),
    }
}

fn make_runs(input: &Path, tmp_dir: &Path, mem_limit: usize, paths: &mut Vec<PathBuf>) -> io::Result<()> {
    let mut r = BufReader::new(File::open(input)?);
    let mut buf = Vec::with_capacity(mem_limit);
    loop {
        buf.clear();
        while buf.len() < mem_limit {
            match read_i64(&mut r)? {
                Some(x) => buf.push(x),
                None => break,
            }
        }
        if buf.is_empty() {
            return Ok(());
        }
        buf.sort_unstable();
        let path = tmp_dir.join(format!("run-{}.bin", paths.len()));
        paths.push(path.clone());
        let mut w = BufWriter::new(File::create(&path)?);
        for x in &buf {
            w.write_all(&x.to_le_bytes())?;
        }
        w.flush()?;
    }
}

fn merge(paths: &[PathBuf], output: &Path) -> io::Result<()> {
    let mut readers = paths.iter().map(|p| File::open(p).map(BufReader::new)).collect::<io::Result<Vec<_>>>()?;
    let mut heap = BinaryHeap::with_capacity(readers.len());
    for (i, r) in readers.iter_mut().enumerate() {
        if let Some(x) = read_i64(r)? {
            heap.push(Reverse((x, i)));
        }
    }
    let mut w = BufWriter::new(File::create(output)?);
    while let Some(Reverse((x, i))) = heap.pop() {
        w.write_all(&x.to_le_bytes())?;
        if let Some(next) = read_i64(&mut readers[i])? {
            heap.push(Reverse((next, i)));
        }
    }
    w.flush()
}

pub fn external_sort(input: &Path, output: &Path, tmp_dir: &Path, mem_limit: usize) -> io::Result<usize> {
    let mut paths = Vec::new();
    let result = make_runs(input, tmp_dir, mem_limit, &mut paths).and_then(|()| merge(&paths, output));
    for p in &paths {
        let _ = fs::remove_file(p);
    }
    result.map(|()| paths.len())
}
