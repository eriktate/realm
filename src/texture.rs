use crate::gl::{
    bind_texture, gen_textures, tex_image, tex_parameteri, TextureFormat, TextureParameter,
    TextureType, TextureValue,
};
use crate::gm::Vec2;
use image::open;

pub struct Texture {
    pub id: u32,
    pub width: u32,
    pub height: u32,
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
}
