use super::*;
use std::cmp::min;

pub fn multiply(a: &Matrix, b: &Matrix) -> Matrix {
    let (a_row, a_col) = a.dim();
    let (b_row, b_col) = b.dim();
    if a_col != b_row {
        panic!(
            "dimenions do not match: ({}x{}) ({}x{})",
            a_row, a_col, b_row, b_col
        );
    }
    let mut c_mat = Matrix::new(a_row, b_col);
    for r in 0..a_row {
        for c in 0..b_col {
            let mut sum = 0.0;
            for k in 0..a_col {
                sum += a.at(r, k) * b.at(k, c);
            }
            c_mat.set(r, c, sum);
        }
    }
    c_mat
}

pub fn multiply_blocked(a: &Matrix, b: &Matrix) -> Matrix {
    let (a_row, a_col) = a.dim();
    let (b_row, b_col) = b.dim();
    if a_col != b_row {
        panic!(
            "dimenions do not match: ({}x{}) ({}x{})",
            a_row, a_col, b_row, b_col
        );
    }
    let mut c_mat = Matrix::new(a_row, b_col);
    const BLOCK_SIZE: usize = 8;
    for ro in (0..a_row).step_by(BLOCK_SIZE) {
        for co in (0..b_col).step_by(BLOCK_SIZE) {
            for ko in (0..a_col).step_by(BLOCK_SIZE) {
                for r in ro..min(ro + BLOCK_SIZE, a_row) {
                    for c in co..min(co + BLOCK_SIZE, b_col) {
                        let mut sum = if ko == 0 { 0.0 } else { c_mat.at(r, c) };
                        for k in ko..min(ko + BLOCK_SIZE, a_col) {
                            sum += a.at(r, k) * b.at(k, c);
                        }
                        c_mat.set(r, c, sum);
                    }
                }
            }
        }
    }
    c_mat
}
