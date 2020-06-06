use crate::gl;

// A Shader
pub struct Shader {
    id: u32,
}

impl Shader {
    pub fn new(vert_src: &str, frag_src: &str) -> Result<Shader, String> {
        println!("Clearing program!");
        gl::use_program(0);
        println!("Creating shaders!");
        let vert_id = gl::create_shader(gl::ShaderType::VertexShader);
        let frag_id = gl::create_shader(gl::ShaderType::FragmentShader);

        println!("Loading shader sources!");
        gl::shader_source(vert_id, vert_src);
        println!("Loading shader sources!");
        gl::shader_source(frag_id, frag_src);

        println!("Compiling shaders!");
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

    pub fn use_program(&self) {
        gl::use_program(self.id);
    }

    // load shaders from file system
    // pub fn load(vert_fname: &str, frag_fname: &str) -> Result<Shader, String> {
    // }
}
