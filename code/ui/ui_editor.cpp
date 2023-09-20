#include "ui_editor.hpp"


namespace ui {


enum editor_action_type
{
    UI_EDITOR_ACTION_NONE,
    UI_EDITOR_ACTION_MOVE,
    UI_EDITOR_ACTION_SELECTION,
};

struct editor_action_move
{

};

struct editor_action_selection
{

};

struct editor_action
{
    editor_action_type type;
    union
    {
        editor_action_move move;
        editor_action_selection selection;
    };
};

struct editor
{
    handle hot; // The element above which is the mouse
    handle active; // The element which player is interacting with
    handle selection;

    // @note: current_action is like a scratchpad where you
    // record your action and then commit to history later.
    editor_action current_action;
    editor_action history[32];
    isize action_end_index;
};


editor *initialize_editor(memory_allocator a)
{
    editor *result = ALLOCATE(a, editor);
    return result;
}


void update_editor(system *s, editor *editor, input_state *input)
{
    auto mouse_position = V3(input->mouse.x, input->mouse.y, 0);

    handle hovered = null_handle();
    for (uint32 i = 0; i < s->drawables.size(); i++)
    {
        drawable *d = s->drawables.data() + i;

        ASSERT(d->owner.type == UI_ELEMENT);
        element *e = s->elements.data() + d->owner.index;

        if (e->is_visible)
        {
            auto inverse_transform = inverse(e->transform_to_root);
            auto mouse_position_local = inverse_transform * mouse_position;

            math::rectangle2 rect = math::rectangle2::from_center_size(e->position, (float32) d->width, (float32) d->height);
            if (math::is_inside(rect, mouse_position_local.xy))
            {
                if (!hovered)
                {
                    hovered = make_handle(UI_ELEMENT, d->owner.index);
                    break;
                }
            }
        }
    }

    if (hovered)
    {
        // I found something under the mouse
        editor->hot = hovered;
    }
    else
    {
        // I didn't find anything under the mouse
        editor->hot = null_handle();
    }
}


void render_editor(execution_context *context, system *s, editor *e)
{
    auto projection =
        math::translated(V3(-1, -1, 0),
        math::scaled(V3(2.0/context->letterbox_width, 2.0/context->letterbox_height, 1),
        math::matrix4::identity()));

    // if (e->hot)
    for (uint32 element_index = 0; element_index < s->elements.size(); element_index++)
    {
        auto *element = &s->elements[element_index];
        if (element->is_visible == false) continue;

        auto h = make_handle(UI_ELEMENT, element_index);
        bool32 have_graphics = false;
        for (auto a : iterate_attaches(s, h))
        {
            have_graphics = (a.type == UI_SHAPE || a.type == UI_IMAGE);
            if (have_graphics) break;
        }
        if (!have_graphics) continue;

        auto model = math::to_matrix4(element->transform_to_root);

        auto rect = math::rectangle2::from_center_size(V2(0), 100, 100);
        auto tl = model * V4(math::top_left(rect), 0, 1);
        auto bl = model * V4(math::bottom_left(rect), 0, 1);
        auto br = model * V4(math::bottom_right(rect), 0, 1);
        auto tr = model * V4(math::top_right(rect), 0, 1);

        auto min_x = tl.x;
        if (bl.x < min_x) min_x = bl.x;
        if (br.x < min_x) min_x = br.x;
        if (tr.x < min_x) min_x = tr.x;

        auto max_x = tl.x;
        if (bl.x > max_x) max_x = bl.x;
        if (br.x > max_x) max_x = br.x;
        if (tr.x > max_x) max_x = tr.x;

        auto min_y = tl.y;
        if (bl.y < min_y) min_y = bl.y;
        if (br.y < min_y) min_y = br.y;
        if (tr.y < min_y) min_y = tr.y;

        auto max_y = tl.y;
        if (bl.y > max_y) max_y = bl.y;
        if (br.y > max_y) max_y = br.y;
        if (tr.y > max_y) max_y = tr.y;

        auto aabb_width = max_x - min_x;
        auto aabb_height = max_y - min_y;
        auto aabb_center = V2(0.5f * (min_x + max_x), 0.5f * (min_y + max_y));

        render_command::command_draw_screen_frame draw_frame;
        draw_frame.model =
            math::translated(V3(aabb_center, 0),
            math::scaled(V3(0.5f * aabb_width, 0.5f * aabb_height, 1),
            math::matrix4::identity()));
        draw_frame.view = math::matrix4::identity();
        draw_frame.projection = projection;
        if (h == e->hot)
        {
            draw_frame.color = V4(1.0,0.2,0.2,1);
        }
        else
        {
            draw_frame.color = V4(0.2,0.5,0.9,1);
        }
        push_draw_screen_frame(context, draw_frame);
    }
}


} // namespace ui
