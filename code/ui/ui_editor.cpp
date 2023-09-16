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

    if (e->hot)
    // for (uint32 i = 0; i < s->elements.size(); i++)
    {
        auto *element = s->elements.data() + e->hot.index;
        auto model =
            math::scaled(V3(50, 50, 1),
            math::to_matrix4(element->transform_to_root));
        transpose(model);

        render_command::command_draw_screen_frame draw_frame;
        draw_frame.model = model;
        draw_frame.view = math::matrix4::identity();
        draw_frame.projection = projection;

        draw_frame.color = V4(1,0,0,1);
        push_draw_screen_frame(context, draw_frame);
    }
}


} // namespace ui
