use crate::gl;
use crate::gm;

// A Shader
pub struct Shader {
    id: u32,
}

impl Shader {
    pub fn new(vert_src: &str, frag_src: &str) -> Result<Shader, String> {
        gl::use_program(0);
        let vert_id = gl::create_shader(gl::ShaderType::VertexShader);
        let frag_id = gl::create_shader(gl::ShaderType::FragmentShader);

        gl::shader_source(vert_id, vert_src);
        gl::shader_source(frag_id, frag_src);

        gl::compile_shader(vert_id)?;
        gl::compile_shader(frag_id)?;

        let program_id = gl::create_program();
        gl::attach_shader(program_id, vert_id);
        gl::attach_shader(program_id, frag_id);

        gl::link_program(program_id)?;

        gl::delete_shader(vert_id);
        gl::delete_shader(frag_id);

        Ok(Shader { id: program_id })
    }

    // load shaders from file system
    // pub fn load(vert_fname: &str, frag_fname: &str) -> Result<Shader, String> {
    // }

    pub fn use_program(&self) {
        gl::use_program(self.id);
    }

    pub fn set_vec2(&self, uniform: &str, vec: gm::Vec2) {
        let loc = gl::get_uniform_location(self.id, uniform);
        gl::program_uniform2f(self.id, loc, vec.x, vec.y);
    }

    pub fn set_i32(&self, uniform: &str, val: i32) {
        let loc = gl::get_uniform_location(self.id, uniform);
        gl::program_uniform1i(self.id, loc, val);
    }

    pub fn set_u32(&self, uniform: &str, val: u32) {
        let loc = gl::get_uniform_location(self.id, uniform);
        gl::program_uniform1u(self.id, loc, val);
    }

    pub fn set_mat4(&self, uniform: &str, mat: gm::Mat4) {
        let loc = gl::get_uniform_location(self.id, uniform);
        gl::program_uniform_matrix4fv(self.id, loc, mat.data);
    }
}
