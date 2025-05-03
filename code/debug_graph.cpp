#include "debug_graph.hpp"


void debug_draw_line(float32 posx, float32 posy, float32 width, float32 height,
    vector4 color,
    gpu_mesh *m,
    shader *s)
{
    transform tm = transform::translate(-1.f, -1.f, 0.f)
                 * transform::scale(2.f / current_client_width,
                                    2.f / current_client_height,
                                    1.f)
                 * transform::translate_x(0.5f * width + posx)
                 * transform::translate_y(0.5f * height + posy)
                 * transform::scale(0.5f * width,
                                    0.5f * height,
                                    1.f);

    glUseProgram(s->id);
    s->uniform("u_model", to_matrix4(tm));
    s->uniform("u_view", matrix4::identity());
    s->uniform("u_projection", matrix4::identity());
    s->uniform("u_color", color);

    glBindVertexArray(m->vao);
    glDrawElements(GL_TRIANGLES, m->count, GL_UNSIGNED_INT, NULL);
}


void draw_debug_graph(debug_graph *graph, gpu_mesh *m, shader *s)
{
    transform tm_ui = transform::translate(-1.f, -1.f, 0.f)
                    * transform::scale(2.f / current_client_width,
                                       2.f / current_client_height,
                                       1.f);
    float32 frame_posy = 100;
    float32 frame_height = 200;
    float32 frame_width = (float32) current_client_width;
    // Draw frame for debug graph
    {
        float32 width[4] = { 1, 1, frame_width, frame_width };
        float32 height[4] = { frame_height, frame_height, 1, 1 };
        float32 posx[4] = { 0, frame_width - 1, 0, 0 };
        float32 posy[4] = { frame_posy, frame_posy, frame_posy, frame_posy + frame_height };

        for (int i = 0; i < 4; i++)
        {
            debug_draw_line(posx[i], posy[i], width[i], height[i], V4(1.f), m, s);
        }
    }
    // Draw usage graph
    for (int i = 0; i < graph->count; i++)
    {
        float32 col_width = ((float32) current_client_width - 2) / (graph->count);
        float32 col_height = (float32) graph->graph[(i + (graph->index)) % (graph->count)] / graph->max_value;

        float32 col_posx = (float32) col_width * i;
        float32 col_posy = (float32) frame_posy + 1;

        auto color = col_height > 1.0f ? V4(1.f, 0.f, 0.f, 1.f)
                                       : V4(0.f, 1.f, 0.f, 1.f);
        debug_draw_line(col_posx, col_posy, col_width, frame_height * col_height,
            color, m, s);
    }
}
