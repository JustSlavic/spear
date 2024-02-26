#ifndef RENDERER_OPENGL_HPP
#define RENDERER_OPENGL_HPP

#include <base.h>
#include <vector4.hpp>
#include <matrix4.hpp>
#include <rs/resource_system.hpp>


namespace gl
{

matrix4 make_projection_matrix(float32 w, float32 h, float32 n, float32 f);
matrix4 make_projection_matrix_fov(float32 fov, float32 aspect_ratio, float32 n, float32 f);
matrix4 make_orthographic_matrix(float32 w, float32 h, float32 n, float32 f);
matrix4 make_orthographic_matrix(float32 aspect_ratio, float32 n, float32 f);

void render_mesh_single_color(context *ctx, matrix4 model, matrix4 view, matrix4 proj, rs::token mesh, rs::token shader, vector4 color);
void render_mesh_texture(context *ctx, matrix4 model, matrix4 view, matrix4 proj, rs::token mesh, rs::token shader, rs::token texture);

} // namespace gl

#endif // RENDERER_OPENGL_HPP
