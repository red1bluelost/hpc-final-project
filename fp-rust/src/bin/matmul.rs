use fp_rust::matrix::{*, multiply::*};
use std::fs::File;
use std::time::Instant;

fn main() {
    let start = Instant::now();

    let mut f = File::open("A.mat").expect("failed to open file for matrix A");
    let a = Matrix::import(&mut f).expect("failed to import matrix A");
    let mut f = File::open("B.mat").expect("failed to open file for matrix B");
    let b = Matrix::import(&mut f).expect("failed to import matrix B");
    let end_import = Instant::now();

    let c = multiply(&a, &b);
    let end_multiply = Instant::now();

    let mut f = File::create("C.mat").expect("failed to open file for matrix C");
    Matrix::export(&mut f, &c).expect("failed to export matrix C");
    let end_export = Instant::now();


    println!("-- Timing MatMul Rust --");
    for (s, d) in [
        ("Import", end_import - start),
        ("Multiply", end_multiply - end_import),
        ("Export", end_export - end_multiply),
        ("Total", end_export - start),
    ] {
        println!("{}: {} us", s, d.as_micros());
    }
}
