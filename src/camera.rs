use crate::gm::{Mat4, Vec3};

pub struct Camera {
    pos: Vec3,
    width: u32,
    height: u32,
    inner_width: u32,
    inner_height: u32,
}

impl Camera {
    pub fn new(
        x: f32,
        y: f32,
        width: u32,
        height: u32,
        inner_width: u32,
        inner_height: u32,
    ) -> Camera {
        Camera {
            pos: Vec3::new(x, y, 0.0),
            width,
            height,
            inner_width,
            inner_height,
        }
    }

    pub fn look_at(&mut self, pos: Vec3) {
        let half_width = (self.inner_width / 2) as f32;
        let half_height = (self.inner_height / 2) as f32;

        let left = self.pos.x - half_width;
        let right = self.pos.x + half_width;
        let top = self.pos.y - half_height;
        let bot = self.pos.y + half_height;

        if pos.x < left {
            self.pos.x = pos.x + half_width;
        }

        if pos.x > right {
            self.pos.x = pos.x - half_width;
        }

        if pos.y < top {
            self.pos.y = pos.y + half_height;
        }

        if pos.y > bot {
            self.pos.y = pos.y - half_height;
        }
    }

    pub fn center_on(&mut self, pos: Vec3) {
        self.pos.x = pos.x - (self.width / 2) as f32;
        self.pos.y = pos.y - (self.height / 2) as f32;
    }

    pub fn transform(&self) -> Mat4 {
        let translation = Mat4::translation(Vec3::new(
            (self.width / 2) as f32 - self.pos.x,
            (self.height / 2) as f32 - self.pos.y,
            0.0,
        ));

        Mat4::ortho(0.0, self.width as f32, 0.0, self.height as f32, 0.0, 1024.0) * translation
    }
}
