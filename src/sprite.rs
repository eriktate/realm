use crate::gm::{Quad, Rect, Vec2, Vec3, Vertex};
use crate::texture;

#[derive(Clone, Debug)]
pub struct Animation {
    framerate: f32,
    current_frame: f32,
    frames: Vec<texture::Quad>,
}

#[derive(Clone, Debug)]
pub enum Show {
    Tex(texture::Quad),
    Anim(Animation),
}

#[derive(Clone, Debug)]
pub struct Sprite {
    pub id: u32,
    pub pos: Vec3,
    pub width: u32,
    pub height: u32,
    pub show: Show,
    pub solid: bool,
    pub flip: bool,
    hb: Rect,
}

impl Sprite {
    pub fn new(
        id: u32,
        pos: Vec3,
        width: u32,
        height: u32,
        hb: Rect,
        solid: bool,
        show: Show,
    ) -> Sprite {
        Sprite {
            id,
            pos,
            width,
            height,
            hb,
            solid,
            flip: false,
            show,
        }
    }

    pub fn to_quad(&self) -> Quad {
        let mut tex_quad = self.tex_quad();
        if self.flip {
            let tl = tex_quad.tl;
            let bl = tex_quad.bl;
            tex_quad.tl = tex_quad.tr;
            tex_quad.tr = tl;
            tex_quad.bl = tex_quad.br;
            tex_quad.br = bl;
        }

        Quad::new(
            Vertex::new(self.pos, tex_quad.tl),
            Vertex::new(
                Vec3::new(self.pos.x + self.width as f32, self.pos.y, self.pos.z),
                tex_quad.tr,
            ),
            Vertex::new(
                Vec3::new(self.pos.x, self.pos.y + self.height as f32, self.pos.z),
                tex_quad.bl,
            ),
            Vertex::new(
                Vec3::new(
                    self.pos.x + self.width as f32,
                    self.pos.y + self.height as f32,
                    self.pos.z,
                ),
                tex_quad.br,
            ),
        )
    }

    pub fn tex_quad(&self) -> texture::Quad {
        match &self.show {
            Show::Tex(tex) => tex.clone(),
            Show::Anim(anim) => anim.tex_quad(),
        }
    }

    pub fn set_pos(&mut self, pos: Vec3) {
        self.pos = pos;
    }

    pub fn tick(&mut self, delta: f32) {
        match &mut self.show {
            Show::Anim(anim) => anim.tick(delta),
            _ => (),
        }
    }

    pub fn hitbox(&self) -> Rect {
        Rect::new(self.pos + self.hb.pos, self.hb.w, self.hb.h)
    }

    pub fn will_overlap(&self, move_vec: Vec3, hb: &Rect) -> bool {
        let future_hb = Rect::new(self.pos + move_vec + self.hb.pos, self.hb.w, self.hb.h);

        future_hb.overlaps(hb)
    }

    pub fn show(&mut self, show: Show) {
        self.show = show
    }
}

impl Animation {
    pub fn new(atlas: &texture::Atlas, framerate: f32, frames: Vec<usize>) -> Animation {
        Animation {
            framerate,
            current_frame: 0.0,
            frames: frames
                .iter()
                .copied()
                .map(|frame| atlas.get_frame_by_idx(frame))
                .collect(),
        }
    }

    pub fn tex_quad(&self) -> texture::Quad {
        self.frames[self.current_frame as usize]
    }

    pub fn tick(&mut self, delta: f32) {
        self.current_frame += self.framerate * (delta as f32);
        if self.current_frame > self.frames.len() as f32 {
            self.current_frame -= self.frames.len() as f32;
        }
    }
}
