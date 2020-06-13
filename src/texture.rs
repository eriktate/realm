use crate::gl::{
    bind_texture, gen_textures, tex_image, tex_parameteri, TextureFormat, TextureParameter,
    TextureType, TextureValue,
};
use crate::gm::Vec2;
use image::open;

#[derive(Copy, Clone, Debug)]
pub struct Texture {
    pub id: u32,
    pub width: u32,
    pub height: u32,
}

#[derive(Copy, Clone, Debug)]
pub struct Quad {
    pub tl: Vec2,
    pub tr: Vec2,
    pub bl: Vec2,
    pub br: Vec2,
}

#[derive(Clone)]
pub struct Atlas {
    tex: Texture,
    offset_x: u32,
    offset_y: u32,
    width: u32,
    height: u32,
    x_sep: u32,
    y_sep: u32,
    rows: u32,
    cols: u32,
    frames: Vec<Quad>,
}

impl Texture {
    pub fn load(fname: &str) -> Texture {
        let img = match open(fname) {
            Ok(i) => i,
            Err(err) => panic!(err),
        };

        let data = img.flipv().to_rgba();
        let id = gen_textures(0);
        bind_texture(TextureType::Tex2D, id);
        tex_parameteri(
            TextureType::Tex2D,
            TextureParameter::WrapS,
            TextureValue::Repeat,
        );

        tex_parameteri(
            TextureType::Tex2D,
            TextureParameter::WrapT,
            TextureValue::Repeat,
        );

        tex_parameteri(
            TextureType::Tex2D,
            TextureParameter::MinFilter,
            TextureValue::Nearest,
        );

        tex_parameteri(
            TextureType::Tex2D,
            TextureParameter::MagFilter,
            TextureValue::Nearest,
        );

        let tex = Texture {
            id,
            width: data.width(),
            height: data.height(),
        };

        tex_image(
            TextureType::Tex2D,
            0,
            TextureFormat::RGBA,
            tex.width,
            tex.height,
            TextureFormat::RGBA,
            &data,
        );

        bind_texture(TextureType::Tex2D, 0);
        tex
    }

    pub fn coord(&self, src: Vec2) -> Vec2 {
        Vec2::new(
            src.x / self.width as f32,
            1 as f32 - (src.y / self.height as f32),
        )
    }

    pub fn tex_quad(&self, x: u32, y: u32, w: u32, h: u32) -> Quad {
        let xx = x as f32;
        let yy = y as f32;
        let ww = w as f32;
        let hh = h as f32;

        Quad::new(
            self.coord(Vec2::new(xx, yy)),
            self.coord(Vec2::new(xx + ww, yy)),
            self.coord(Vec2::new(xx, yy + hh)),
            self.coord(Vec2::new(xx + ww, yy + hh)),
        )
    }
}

impl Quad {
    pub fn new(tl: Vec2, tr: Vec2, bl: Vec2, br: Vec2) -> Quad {
        Quad { tl, tr, bl, br }
    }
}

impl Atlas {
    pub fn new(
        tex: Texture,
        offset_x: u32,
        offset_y: u32,
        width: u32,
        height: u32,
        x_sep: u32,
        y_sep: u32,
        rows: u32,
        cols: u32,
    ) -> Atlas {
        let mut frames: Vec<Quad> = Vec::with_capacity((rows * cols) as usize);
        for r in 0..rows {
            for c in 0..cols {
                let tl = Vec2::new(
                    (offset_x + (x_sep * (c + 1) + (c * width))) as f32,
                    (offset_y + (y_sep * (r + 1)) + (r * height)) as f32,
                );
                frames.push(Quad::new(
                    tex.coord(tl),
                    tex.coord(Vec2::new(tl.x + width as f32, tl.y)),
                    tex.coord(Vec2::new(tl.x, tl.y + height as f32)),
                    tex.coord(Vec2::new(tl.x + width as f32, tl.y + height as f32)),
                ));
            }
        }

        Atlas {
            tex,
            offset_x,
            offset_y,
            width,
            height,
            x_sep,
            y_sep,
            rows,
            cols,
            frames,
        }
    }

    pub fn get_frame_by_idx(&self, idx: usize) -> Quad {
        self.frames[idx]
    }

    pub fn get_frame_by_pos(&self, row: u32, col: u32) -> Quad {
        self.frames[(row * self.cols + col) as usize]
    }
}
