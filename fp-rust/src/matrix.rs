use std::io::{self, BufRead, Write};

pub mod multiply;

#[derive(Debug)]
pub struct Matrix {
    rows: usize,
    cols: usize,
    data: Vec<f64>,
}

impl Matrix {
    #[inline]
    pub fn new(rows: usize, cols: usize) -> Self {
        Matrix {
            rows,
            cols,
            data: vec![0f64; rows * cols],
        }
    }

    #[inline]
    pub fn from_vec(rows: usize, cols: usize, data: Vec<f64>) -> Self {
        if rows * cols != data.len() {
            panic!("dimensions do not match provided vector")
        }
        Matrix { rows, cols, data }
    }

    pub fn identity(s: usize) -> Self {
        let mut m = Matrix::new(s, s);
        for i in 0..s {
            m.set(i, i, 1.0);
        }
        m
    }

    #[inline]
    pub fn dim(&self) -> (usize, usize) {
        (self.rows, self.cols)
    }

    #[inline]
    pub fn at(&self, r: usize, c: usize) -> f64 {
        self.data[r * self.cols + c]
    }

    #[inline]
    pub fn set(&mut self, r: usize, c: usize, v: f64) {
        self.data[r * self.cols + c] = v;
    }

    pub fn export(w: &mut impl io::Write, m: &Self) -> io::Result<()> {
        let bw = &mut io::BufWriter::new(w);
        write!(bw, "{} {}\n", m.rows, m.cols)?;
        for r in 0..m.rows {
            for c in 0..m.cols {
                write!(bw, "{:.6} ", m.at(r, c))?;
            }
            write!(bw, "\n")?;
        }
        Ok(())
    }

    pub fn import(r: &mut impl io::Read) -> io::Result<Self> {
        let br = &mut io::BufReader::new(r);
        let mut line = String::new();
        br.read_line(&mut line).expect("failed to read dimensions");
        let mut line_iter = line
            .trim()
            .split(' ')
            .map(|w| w.parse().expect("failed to parse dimensions"));
        let rows = line_iter.next().expect("missing rows");
        let cols = line_iter.next().expect("missing columns");
        let mut data: Vec<f64> = Vec::with_capacity(rows * cols);
        for line in br.lines() {
            for word in line.expect("missing data row").split_whitespace() {
                data.push(word.parse().expect("failed to parse data"));
            }
        }
        Ok(Self::from_vec(rows, cols, data))
    }
}

impl Eq for Matrix {}
impl PartialEq for Matrix {
    fn eq(&self, other: &Self) -> bool {
        self.dim() == other.dim()
            && std::iter::zip(&self.data, &other.data).all(|(&a, &b)| (a - b).abs() <= 0.0001)
    }
}
