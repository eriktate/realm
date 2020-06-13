use glfw::Glfw;
use std::ffi::CString;

#[repr(u32)]
pub enum ShaderType {
    VertexShader = gl::VERTEX_SHADER,
    FragmentShader = gl::FRAGMENT_SHADER,
}

#[repr(u32)]
pub enum DrawMode {
    Points = gl::POINTS,
    LineStrip = gl::LINE_STRIP,
    LineLoop = gl::LINE_LOOP,
    Lines = gl::LINES,
    Triangles = gl::TRIANGLES,
}

#[repr(u32)]
pub enum BufferBit {
    Color = gl::COLOR_BUFFER_BIT,
}

#[repr(u32)]
pub enum TextureType {
    Tex2D = gl::TEXTURE_2D,
}

#[repr(u32)]
pub enum TextureParameter {
    WrapS = gl::TEXTURE_WRAP_S,
    WrapT = gl::TEXTURE_WRAP_T,
    MinFilter = gl::TEXTURE_MIN_FILTER,
    MagFilter = gl::TEXTURE_MAG_FILTER,
}

#[repr(u32)]
pub enum TextureFormat {
    Red = gl::RED,
    RG = gl::RG,
    RGB = gl::RGB,
    RGBA = gl::RGBA,
    DepthComponent = gl::DEPTH_COMPONENT,
    DepthStencil = gl::DEPTH_STENCIL,
}

#[repr(u32)]
pub enum TextureValue {
    // filters
    Nearest = gl::NEAREST,
    Linear = gl::LINEAR,
    NearestMipmapNearest = gl::NEAREST_MIPMAP_NEAREST,
    LinearMipmapNearest = gl::LINEAR_MIPMAP_NEAREST,
    NearestMipmapLinear = gl::NEAREST_MIPMAP_LINEAR,
    LinearMipmapLinear = gl::LINEAR_MIPMAP_LINEAR,
    // wrapping
    Repeat = gl::REPEAT,
}

#[repr(u32)]
pub enum Textures {
    Tex0 = gl::TEXTURE0,
    Tex1 = gl::TEXTURE1,
}

#[repr(u32)]
pub enum Capability {
    Blend = gl::BLEND,
}

#[repr(u32)]
pub enum BlendFactor {
    SrcAlpha = gl::SRC_ALPHA,
    OneMinusSrcAlpha = gl::ONE_MINUS_SRC_ALPHA,
}

// initialization
pub fn init(glfw: &Glfw) {
    gl::load_with(|s| glfw.get_proc_address_raw(s));
}

pub fn viewport(x: u32, y: u32, width: u32, height: u32) {
    unsafe { gl::Viewport(x as i32, y as i32, width as i32, height as i32) }
}

// vertices
pub fn create_vao() -> u32 {
    let mut id: u32 = 0;
    unsafe { gl::GenVertexArrays(1, &mut id) };
    id
}

pub fn bind_vao(id: u32) {
    unsafe {
        gl::BindVertexArray(id);
    }
}

pub fn bind_vbo(id: u32) {
    unsafe { gl::BindBuffer(gl::ARRAY_BUFFER, id) };
}

pub fn bind_ebo(id: u32) {
    unsafe { gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, id) };
}

pub fn create_vbo<T>(data: &Vec<T>) -> u32 {
    let usage = gl::DYNAMIC_DRAW;
    let mut id: u32 = 0;

    unsafe { gl::GenBuffers(1, &mut id) };
    bind_vbo(id);

    let size = data.len() * std::mem::size_of::<T>();
    let ptr = data.as_ptr() as *const std::ffi::c_void;

    // let size = 9 * 4;
    // let data: [f32; 9] = [0.0, -0.5, 0.0, 0.5, -0.5, 0.0, -0.5, 0.5, 0.0];
    unsafe {
        // let ptr = data.as_ptr() as *const std::ffi::c_void;
        gl::BufferData(gl::ARRAY_BUFFER, size as isize, ptr, usage);
    };

    bind_vbo(0);

    id
}

pub fn update_vbo<T>(id: u32, data: &Vec<T>) {
    let usage = gl::DYNAMIC_DRAW;
    bind_vbo(id);

    let size = data.len() * std::mem::size_of::<T>();
    let ptr = data.as_ptr() as *const std::ffi::c_void;

    // let size = 9 * 4;
    // let data: [f32; 9] = [0.0, -0.5, 0.0, 0.5, -0.5, 0.0, -0.5, 0.5, 0.0];
    unsafe {
        // let ptr = data.as_ptr() as *const std::ffi::c_void;
        gl::BufferData(gl::ARRAY_BUFFER, size as isize, ptr, usage);
    };
}

pub fn create_ebo(indices: &Vec<u32>) -> u32 {
    let usage = gl::DYNAMIC_DRAW;
    let mut id: u32 = 0;
    unsafe { gl::GenBuffers(1, &mut id) };
    bind_ebo(id);
    let size = std::mem::size_of::<u32>() * indices.len();
    unsafe {
        gl::BufferData(
            gl::ELEMENT_ARRAY_BUFFER,
            size as isize,
            indices.as_ptr() as *const std::ffi::c_void,
            usage as u32,
        )
    }

    bind_ebo(0);
    id
}

// TODO (etate): Support more than GL_FLOAT
pub fn vertex_attrib_pointer(index: u32, size: usize, normalized: bool, stride: u32, offset: u32) {
    let norm = if normalized { gl::TRUE } else { gl::FALSE };
    unsafe {
        gl::VertexAttribPointer(
            index,
            size as i32,
            gl::FLOAT,
            norm,
            stride as i32,
            offset as *const std::ffi::c_void,
        )
    }
}

pub fn enable_vertex_attrib_array(index: u32) {
    unsafe { gl::EnableVertexAttribArray(index) }
}

// shaders
pub fn use_program(id: u32) {
    unsafe { gl::UseProgram(id) }
}

pub fn create_shader(kind: ShaderType) -> u32 {
    return unsafe { gl::CreateShader(kind as u32) };
}

pub fn create_program() -> u32 {
    return unsafe { gl::CreateProgram() };
}

pub fn shader_source(id: u32, src: &str) {
    let c_str = CString::new(src).expect("CString::new failed");
    unsafe {
        gl::ShaderSource(
            id,
            1,
            // this is bananas
            [c_str.as_ptr()].as_ptr(),
            std::ptr::null(),
        );
    }
}

pub fn compile_shader(id: u32) -> Result<(), String> {
    unsafe { gl::CompileShader(id) }
    let mut success: i32 = 0;
    let mut info_log: Vec<u8> = Vec::with_capacity(512);
    let mut log_size: i32 = 0;
    unsafe { gl::GetShaderiv(id, gl::COMPILE_STATUS, &mut success) }

    if success == 0 {
        unsafe {
            gl::GetShaderInfoLog(id, 512, &mut log_size, info_log.as_mut_ptr() as *mut i8);
            info_log.set_len(log_size as usize);
        }

        let err = String::from_utf8(info_log).unwrap();
        return Err(err);
    }

    Ok(())
}

pub fn attach_shader(program_id: u32, shader_id: u32) {
    unsafe { gl::AttachShader(program_id, shader_id) }
}

pub fn link_program(id: u32) -> Result<(), String> {
    let mut success: i32 = 0;
    let mut info_log: Vec<u8> = Vec::with_capacity(512);
    let mut log_size: i32 = 0;

    unsafe {
        gl::LinkProgram(id);
        gl::GetProgramiv(id, gl::LINK_STATUS, &mut success);
        if success == 0 {
            gl::GetProgramInfoLog(id, 512, &mut log_size, info_log.as_mut_ptr() as *mut i8);

            info_log.set_len(log_size as usize);
            let err = String::from_utf8(info_log).unwrap();
            return Err(err);
        }
    }

    Ok(())
}

pub fn get_uniform_location(id: u32, uniform: &str) -> i32 {
    let c_str = CString::new(uniform).unwrap();
    let loc: i32 = unsafe { gl::GetUniformLocation(id, c_str.into_raw()) };
    loc
}

pub fn program_uniform2f(id: u32, location: i32, x: f32, y: f32) {
    unsafe { gl::ProgramUniform2f(id, location, x, y) }
}

pub fn program_uniform1i(id: u32, location: i32, val: i32) {
    unsafe { gl::ProgramUniform1i(id, location, val) }
}

pub fn program_uniform1u(id: u32, location: i32, val: u32) {
    unsafe { gl::ProgramUniform1ui(id, location, val) }
}

pub fn program_uniform_matrix4fv(id: u32, location: i32, val: [f32; 16]) {
    unsafe { gl::ProgramUniformMatrix4fv(id, location, 1, gl::FALSE, val.as_ptr()) }
}

pub fn delete_shader(id: u32) {
    unsafe { gl::DeleteShader(id) }
}

// drawing
pub fn clear_color(r: f32, g: f32, b: f32, a: f32) {
    unsafe { gl::ClearColor(r, g, b, a) }
}

pub fn clear(mask: u32) {
    unsafe { gl::Clear(mask) }
}

pub fn draw_arrays(mode: DrawMode, first: i32, count: i32) {
    unsafe { gl::DrawArrays(mode as u32, first, count) }
}

pub fn draw_elements(mode: DrawMode, indices: &Vec<u32>) {
    unsafe {
        gl::DrawElements(
            mode as u32,
            indices.len() as i32,
            gl::UNSIGNED_INT,
            0 as *const std::ffi::c_void,
        )
    }
}

// textures
pub fn gen_textures(count: u32) -> u32 {
    let mut id: u32 = 0;
    unsafe { gl::GenTextures(1, &mut id) };
    id
}

pub fn bind_texture(kind: TextureType, id: u32) {
    unsafe { gl::BindTexture(kind as u32, id) }
}

pub fn tex_parameteri(kind: TextureType, param: TextureParameter, val: TextureValue) {
    unsafe { gl::TexParameteri(kind as u32, param as u32, val as i32) }
}

pub fn tex_image(
    kind: TextureType,
    level: i32,
    internal_format: TextureFormat,
    width: u32,
    height: u32,
    format: TextureFormat,
    data: &[u8],
) {
    unsafe {
        gl::TexImage2D(
            kind as u32,
            level,
            internal_format as i32,
            width as i32,
            height as i32,
            0,
            format as u32,
            gl::UNSIGNED_BYTE,
            data.as_ptr() as *const std::ffi::c_void,
        )
    }
}

pub fn active_texture(id: u32) {
    // unsafe { gl::ActiveTexture(id) }
    unsafe { gl::ActiveTexture(gl::TEXTURE0) }
}

pub fn enable(cap: Capability) {
    unsafe { gl::Enable(cap as u32) };
}

pub fn blend_func(sfactor: BlendFactor, dfactor: BlendFactor) {
    unsafe { gl::BlendFunc(sfactor as u32, dfactor as u32) };
}
