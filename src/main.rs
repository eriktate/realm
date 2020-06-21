use glfw::{Action, Context, Key};

mod camera;
mod gl;
mod gm;
mod input;
mod scene;
mod shader;
mod sprite;
mod texture;

use gm::{Vec3, Vertex};
use input::Controller;
use sprite::Show;
use texture::Texture;

const WIDTH: u32 = 1280;
const HEIGHT: u32 = 960;

fn process_input(window: &mut glfw::Window, ctrl: &mut Controller) {
    // clear inputs before processing new state
    ctrl.clear();
    if window.get_key(glfw::Key::Escape) == glfw::Action::Press {
        window.set_should_close(true);
    }

    if window.get_key(glfw::Key::W) == glfw::Action::Press {
        ctrl.press(input::KeyInput::Up);
    }

    if window.get_key(glfw::Key::S) == glfw::Action::Press {
        ctrl.press(input::KeyInput::Down);
    }

    if window.get_key(glfw::Key::A) == glfw::Action::Press {
        ctrl.press(input::KeyInput::Left);
    }

    if window.get_key(glfw::Key::D) == glfw::Action::Press {
        ctrl.press(input::KeyInput::Right);
    }

    // lock in new inputs and determine key states
    ctrl.lock_in();
}

fn simple_level(sc: &mut scene::Scene, tex: &texture::Texture) {
    let floor_tex = tex.tex_quad(16, 64, 16, 16);
    let wall_tex = tex.tex_quad(32, 0, 16, 32);
    for i in 0..10 {
        // face walls
        let pos = Vec3::new(128.0 + (16 * i) as f32, 96.0, 0.0);
        sc.new_sprite(
            pos,
            16,
            32,
            gm::Rect::new(Vec3::new(0.0, 24.0, 0.0), 16.0, 8.0),
            true,
            Show::Tex(wall_tex),
        );
        for j in 0..10 {
            // flooring
            let pos = Vec3::new(128.0 + (16 * i) as f32, 128.0 + (16 * j) as f32, -0.5);
            sc.new_sprite(
                pos,
                16,
                16,
                gm::Rect::new(Vec3::new(0.0, 0.0, 0.0), 0.0, 0.0),
                false,
                Show::Tex(floor_tex),
            );
        }

        // bottom wall
        let pos = Vec3::new(128.0 + (16 * i) as f32, 256.0, 0.0);
        sc.new_sprite(
            pos,
            16,
            32,
            gm::Rect::new(Vec3::new(0.0, 24.0, 0.0), 16.0, 8.0),
            true,
            Show::Tex(wall_tex),
        );
    }
}

fn main() {
    let mut glfw = glfw::init(glfw::FAIL_ON_ERRORS).unwrap();
    glfw.window_hint(glfw::WindowHint::ContextVersion(3, 3));

    let (mut window, events) = glfw
        .create_window(
            WIDTH,
            HEIGHT,
            "float - playground",
            glfw::WindowMode::Windowed,
        )
        .expect("failed to create GLFW window");

    window.make_current();
    window.set_key_polling(true);

    gl::init(&glfw);
    gl::viewport(0, 0, WIDTH, HEIGHT);

    // do texture things
    let tex = Texture::load("spritesheet.png");
    let atlas = texture::Atlas::new(tex, 128, 192, 16, 32, 0, 0, 1, 8);
    let stand_anim = sprite::Animation::new(&atlas, 8.0, vec![0, 1, 2, 3]);
    let walk_anim = sprite::Animation::new(&atlas, 8.0, vec![4, 5, 6, 7]);

    // make scene
    let mut sc = scene::Scene::new(100);

    // make sprite
    simple_level(&mut sc, &tex);
    let wall_tex = tex.tex_quad(288, 288, 16, 32);
    for i in 0..8 {
        sc.new_sprite(
            Vec3::new(144.0 + (16 * i) as f32, 160.0, 0.0),
            16,
            32,
            gm::Rect::new(Vec3::new(0.0, 16.0, 0.0), 16.0, 16.0),
            true,
            Show::Tex(wall_tex),
        );
    }

    // add player last so everything else renders below
    let player_id = sc.new_sprite(
        Vec3::new(144.0, 128.0, 0.0),
        16,
        32,
        gm::Rect::new(Vec3::new(0.0, 24.0, 0.0), 16.0, 8.0),
        false,
        Show::Anim(stand_anim.clone()),
    );

    // load shaders
    let vert_src = include_str!("../shaders/sprite_vs.glsl");
    let frag_src = include_str!("../shaders/sprite_fs.glsl");
    let shader_program = shader::Shader::new(vert_src, frag_src).unwrap();
    shader_program.set_u32("tex", gl::Textures::Tex0 as u32);

    let indices = gm::make_indices(&sc.quads());
    let vao = gl::create_vao();
    let vbo = gl::create_vbo(&sc.quads());
    let ebo = gl::create_ebo(&indices);

    gl::bind_vao(vao); // start recording vao
    gl::bind_vbo(vbo);
    gl::bind_ebo(ebo);
    gl::vertex_attrib_pointer(0, 3, false, std::mem::size_of::<Vertex>() as u32, 0);
    gl::vertex_attrib_pointer(
        1,
        2,
        false,
        std::mem::size_of::<Vertex>() as u32,
        std::mem::size_of::<Vec3>() as u32,
    );

    gl::enable_vertex_attrib_array(0);
    gl::enable_vertex_attrib_array(1);
    gl::bind_vao(0); // stop recording vao
    gl::bind_vbo(0);
    gl::bind_ebo(0);

    // alpha in texture won't work without setting the blend mode
    gl::enable(gl::Capability::Blend);
    gl::blend_func(gl::BlendFactor::SrcAlpha, gl::BlendFactor::OneMinusSrcAlpha);

    let mut cam = camera::Camera::new(0.0, 0.0, WIDTH / 2, HEIGHT / 2, WIDTH / 12, HEIGHT / 12);
    let mut time_last_frame = glfw.get_time() as f32;
    let mut ctrl = input::Controller::new();
    cam.center_on(sc.get_sprite(player_id).pos);

    let mut moving = false;

    while !window.should_close() {
        window.swap_buffers();
        glfw.poll_events();
        process_input(&mut window, &mut ctrl);
        let current_time = glfw.get_time() as f32;
        let delta = current_time - time_last_frame;
        time_last_frame = current_time;

        let speed = 75.0;
        let initial_move = ctrl.move_vec().unit() * speed * delta;
        let mut move_vec = initial_move;
        for spr in sc.sprites() {
            if spr.id == player_id || !spr.solid {
                continue;
            }
            let player = sc.get_sprite(player_id);
            let other = spr.hitbox();
            if player.will_overlap(move_vec, &other) {
                move_vec.x = 0.0;
                if player.will_overlap(move_vec, &other) {
                    move_vec = initial_move;
                    move_vec.y = 0.0;
                    if player.will_overlap(move_vec, &other) {
                        move_vec = Vec3::zero();
                    }
                }
            }
        }

        {
            let player = sc.get_mut_sprite(player_id);
            if move_vec.x > 0.0 {
                player.flip = false;
            }

            if move_vec.x < 0.0 {
                player.flip = true;
            }

            if !moving {
                if !gm::f32_eq(move_vec.mag(), 0.0) {
                    moving = true;
                    player.show(Show::Anim(walk_anim.clone()));
                }
            } else {
                if gm::f32_eq(move_vec.mag(), 0.0) {
                    moving = false;
                    player.show(Show::Anim(stand_anim.clone()));
                }
            }
        }

        sc.move_sprite(player_id, move_vec);

        sc.tick(delta);

        cam.look_at(sc.get_sprite(player_id).pos);
        shader_program.set_mat4("transform", cam.transform());
        gl::update_vbo(vbo, &sc.quads());

        gl::clear_color(0.5, 0.8, 0.5, 1.0);
        gl::clear(gl::BufferBit::Color as u32);
        shader_program.use_program();
        gl::active_texture(0);
        gl::bind_texture(gl::TextureType::Tex2D, tex.id);
        gl::bind_vao(vao);
        gl::draw_elements(gl::DrawMode::Triangles, &indices);
    }
}
