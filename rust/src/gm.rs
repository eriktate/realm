pub struct Vec2 {
    x: f32,
    y: f32,
}

pub struct Vec3 {
    x: f32,
    y: f32,
    z: f32,
}

pub struct Vec4 {
    x: f32,
    y: f32,
    z: f32,
    w: f32,
}

// Mat4 is stored in column major format.
pub struct Mat4 {
    data: [f32; 16],
}

pub struct Rect {
    pos: Vec2,
    w: f32,
    h: f32,
}

pub struct Vertex {
    pos: Vec3,
    tex_coord: Vec2,
}

pub struct Quad {
    tl: Vertex,
    tr: Vertex,
    bl: Vertex,
    br: Vertex,
}

impl Vec2 {
    pub fn new(x: f32, y: f32) -> Vec2 {
        Vec2 { x, y }
    }

    pub fn zero() -> Vec2 {
        Vec2 { x: 0.0, y: 0.0 }
    }

    pub fn mag(&self) -> f32 {
        (self.x.powi(2) + self.x.powi(2)).sqrt()
    }

    pub fn unit(&self) -> Vec2 {
        let mag = self.mag();
        if f32_eq(mag, 0.0) {
            return Vec2::zero();
        }

        Vec2::new(self.x / mag, self.y / mag)
    }
}

impl Vec3 {
    pub fn new(x: f32, y: f32, z: f32) -> Vec3 {
        Vec3 { x, y, z }
    }

    pub fn zero() -> Vec3 {
        Vec3 {
            x: 0.0,
            y: 0.0,
            z: 0.0,
        }
    }

    pub fn mag(&self) -> f32 {
        (self.x.powi(2) + self.x.powi(2) + self.z.powi(2)).sqrt()
    }

    pub fn unit(&self) -> Vec3 {
        let mag = self.mag();
        if f32_eq(mag, 0.0) {
            return Vec3::zero();
        }

        Vec3::new(self.x / mag, self.y / mag, self.z / mag)
    }
}

impl Vec4 {
    pub fn new(x: f32, y: f32, z: f32, w: f32) -> Vec4 {
        Vec4 { x, y, z, w }
    }

    pub fn zero() -> Vec4 {
        Vec4 {
            x: 0.0,
            y: 0.0,
            z: 0.0,
            w: 0.0,
        }
    }
}

impl Vertex {
    pub fn new(pos: Vec3, tex_coord: Vec2) -> Vertex {
        Vertex { pos, tex_coord }
    }
}

// column major ordering reference
// 0, 4, 8, 12,
// 1, 5, 9, 13,
// 2, 6, 10, 14,
// 3, 7, 11, 15
impl Mat4 {
    pub fn identity() -> Mat4 {
        Mat4 {
            data: [
                1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0,
            ],
        }
    }

    pub fn translation(vec: Vec3) -> Mat4 {
        let mut mat = Mat4::identity();
        mat.data[12] = vec.x;
        mat.data[13] = vec.y;
        mat.data[14] = vec.z;
        mat
    }

    pub fn translate(self, vec: Vec3) -> Mat4 {
        self * Mat4::translation(vec)
    }

    pub fn ortho(l: f32, r: f32, t: f32, b: f32, f: f32, n: f32) -> Mat4 {
        Mat4 {
            data: [
                2.0 / (r - l),
                0.0,
                0.0,
                0.0,
                0.0,
                2.0 / (t - b),
                0.0,
                0.0,
                0.0,
                0.0,
                -2.0 / (f - n),
                0.0,
                -(r + l) / (r - l),
                -(t + b) / (t - b),
                -(f + n) / (f - n),
                1.0,
            ],
        }
    }
}

impl std::ops::Mul for Mat4 {
    type Output = Self;

    fn mul(self, rhs: Self) -> Self {
        let mut res = Mat4::identity();
        for i in 0..4 {
            for j in 0..4 {
                res.data[i + (4 * j)] = (self.data[i] * rhs.data[j * 4])
                    + (self.data[i + 4] * rhs.data[(j * 4) + 1])
                    + (self.data[i + 8] * rhs.data[(j * 4) + 2])
                    + (self.data[i + 12] * rhs.data[(j * 4) + 3]);
            }
        }

        res
    }
}

impl Rect {
    pub fn new(pos: Vec2, w: f32, h: f32) -> Rect {
        Rect { pos, w, h }
    }

    pub fn from_parts(x: f32, y: f32, w: f32, h: f32) -> Rect {
        Rect::new(Vec2::new(x, y), w, h)
    }

    fn overlaps(&self, other: &Rect) -> bool {
        // flipping arguments captures containment
        overlaps(&self, other) || overlaps(other, &self)
    }
}

// determine if a rectangle overlaps another rectangle
fn overlaps(lhs: &Rect, rhs: &Rect) -> bool {
    let lhs_top = lhs.pos.y;
    let lhs_bottom = lhs.pos.y + lhs.h;
    let lhs_left = lhs.pos.x;
    let lhs_right = lhs.pos.x + lhs.w;

    let rhs_top = rhs.pos.y;
    let rhs_bottom = rhs.pos.y + rhs.h;
    let rhs_left = rhs.pos.x;
    let rhs_right = rhs.pos.x + rhs.w;

    let y_overlap = (lhs_top >= rhs_top && lhs_top <= rhs_bottom)
        || (lhs_bottom >= rhs_top && lhs_bottom <= rhs_bottom);
    let x_overlap = (lhs_left >= rhs_left && lhs_left <= rhs_right)
        || (lhs_right >= rhs_left && lhs_right <= rhs_right);

    return x_overlap && y_overlap;
}

impl From<&Vec2> for Vec3 {
    fn from(v: &Vec2) -> Vec3 {
        Vec3::new(v.x, v.y, 0.0)
    }
}

impl From<&Vec3> for Vec4 {
    fn from(v: &Vec3) -> Vec4 {
        Vec4::new(v.x, v.y, v.z, 0.0)
    }
}

fn f32_eq(lhs: f32, rhs: f32) -> bool {
    (lhs - rhs) < 0.0000001
}
