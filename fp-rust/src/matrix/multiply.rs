use super::*;

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
