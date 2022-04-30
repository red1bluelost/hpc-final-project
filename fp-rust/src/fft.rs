use num_complex::Complex64;
use std::io::{self, BufRead, Write};

pub mod transform;

pub type Cmplx = Complex64;
pub struct Vector(Vec<Cmplx>);

impl Vector {
    pub fn new(size: usize) -> Self {
        Vector(vec![0.0.into(); size])
    }

    pub fn from_vec(size: usize, data: Vec<Cmplx>) -> Self {
        if size != data.len() {
            panic!("dimensions do not match provided vector")
        }
        Vector(data)
    }

    pub fn export(&self, w: &mut impl io::Write) -> io::Result<()> {
        let bw = &mut io::BufWriter::new(w);
        let Vector(data) = self;
        write!(bw, "{}\n", data.len())?;
        for &c in data {
            write!(bw, "({:.6})\n", c)?;
        }
        Ok(())
    }

    pub fn import(r: &mut impl io::Read) -> io::Result<Self> {
        let br = &mut io::BufReader::new(r);
        let mut lines = br.lines();
        let size = lines
            .next()
            .expect("missing size")?
            .parse()
            .expect("failed to parse size");
        let data = lines
            .map(|l| {
                l.unwrap()
                    .strip_prefix("(")
                    .unwrap()
                    .strip_suffix(")")
                    .unwrap()
                    .parse()
                    .expect("failed to parse value")
            })
            .collect();
        Ok(Self::from_vec(size, data))
    }
}
