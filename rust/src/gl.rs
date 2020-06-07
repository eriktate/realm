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

pub fn init(glfw: &Glfw) {
    gl::load_with(|s| glfw.get_proc_address_raw(s));
}

pub fn viewport(x: u32, y: u32, width: u32, height: u32) {
    unsafe { gl::Viewport(x as i32, y as i32, width as i32, height as i32) }
}

pub fn create_vao() -> u32 {
    let mut id: u32 = 0;
    unsafe { gl::GenVertexArrays(1, &mut id) };
    id
}

pub fn bind_vertex_array(id: u32) {
    unsafe {
        gl::BindVertexArray(id);
    }
}

pub fn bind_array_buffer(id: u32) {
    unsafe {
        gl::BindBuffer(gl::ARRAY_BUFFER, id);
    }
}

pub fn create_vbo<T>(data: &Vec<T>) -> u32 {
    let usage = gl::DYNAMIC_DRAW;
    let mut id: u32 = 0;

    unsafe { gl::GenBuffers(1, &mut id) };
    bind_array_buffer(id);

    let size = data.len() * std::mem::size_of::<T>();
    let ptr = data.as_ptr() as *const std::ffi::c_void;

    // let size = 9 * 4;
    // let data: [f32; 9] = [0.0, -0.5, 0.0, 0.5, -0.5, 0.0, -0.5, 0.5, 0.0];
    unsafe {
        // let ptr = data.as_ptr() as *const std::ffi::c_void;
        gl::BufferData(gl::ARRAY_BUFFER, size as isize, ptr, usage);
    };

    bind_array_buffer(0);

    id
}

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

pub fn delete_shader(id: u32) {
    unsafe { gl::DeleteShader(id) }
}

pub fn clear_color(r: f32, g: f32, b: f32, a: f32) {
    unsafe { gl::ClearColor(r, g, b, a) }
}

pub fn clear(mask: u32) {
    unsafe { gl::Clear(mask) }
}

pub fn draw_arrays(mode: DrawMode, first: i32, count: i32) {
    unsafe { gl::DrawArrays(mode as u32, first, count) }
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
