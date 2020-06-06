use gl::types::GLenum;
use glfw::Glfw;

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

fn bind_vertex_array(id: u32) {
    unsafe {
        gl::BindVertexArray(id);
    }
}

fn bind_buffer(kind: GLenum, id: u32) {
    unsafe {
        gl::BindBuffer(kind, id);
    }
}

pub fn create_vbo<T>(vao: u32, data: Vec<T>) -> u32 {
    let usage = gl::DYNAMIC_DRAW;
    let mut id: u32 = 0;

    bind_vertex_array(vao);
    unsafe { gl::GenBuffers(1, &mut id) };
    bind_buffer(gl::ARRAY_BUFFER, id);

    let size = data.capacity() * std::mem::size_of::<T>();
    let ptr = data.as_ptr();
    unsafe {
        let ptr = ptr as *const std::ffi::c_void;
        gl::BufferData(gl::ARRAY_BUFFER, size as isize, ptr, usage);
    };

    bind_vertex_array(0);
    bind_buffer(gl::ARRAY_BUFFER, 0);

    id
}
