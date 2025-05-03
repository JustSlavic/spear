#ifndef DEBUG_GRAPH_HPP
#define DEBUG_GRAPH_HPP

#include <base.h>
#include <math/vector4.hpp>
#include <common_graphics.hpp>


struct debug_graph
{
    memory_buffer memory;
    float32 *graph;
    uint64   count;
    uint64   index;
    float32  max_value;
};


void draw_debug_graph(debug_graph *graph, gpu_mesh *m, shader *s);
void debug_draw_line(float32 posx, float32 posy, float32 width, float32 height,
    vector4 color, gpu_mesh *m, shader *s);


#endif // DEBUG_GRAPH_HPP
