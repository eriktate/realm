use crate::gm::{Quad, Rect, Vec2, Vec3, Vertex};
use crate::texture::TexQuad;

pub struct Atlas {}

#[derive(Clone)]
pub struct Animation {
    id: u32,
    framerate: f32,
    current_frame: f32,
    frames: Vec<TexQuad>,
}

pub enum Show {
    Tex(TexQuad),
    Anim(Animation),
}

pub struct Sprite {
    id: u32,
    pos: Vec3,
    width: u32,
    height: u32,
    hitbox: Rect,
    show: Show,
}

impl Sprite {
    pub fn new(id: u32, pos: Vec3, width: u32, height: u32, show: Show) -> Sprite {
        Sprite {
            id,
            pos,
            width,
            height,
            hitbox: Rect::new(pos, width as f32, height as f32),
            show,
        }
    }

    pub fn to_quad(&self) -> Quad {
        let tex_quad = self.tex_quad();

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

    pub fn tex_quad(&self) -> TexQuad {
        match &self.show {
            Show::Tex(tex) => tex.clone(),
            Show::Anim(anim) => anim.tex_quad(),
        }
    }

    pub fn set_pos(&mut self, pos: Vec3) {
        self.pos = pos;
    }
}

impl Animation {
    pub fn tex_quad(&self) -> TexQuad {
        TexQuad::new(Vec2::zero(), Vec2::zero(), Vec2::zero(), Vec2::zero())
    }
}
