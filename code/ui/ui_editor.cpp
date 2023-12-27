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
    handle h;
    vector2 old_position;
    vector2 new_position;
};

struct editor_action_selection
{
    handle old_selection;
    handle new_selection;
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
    editor_action history[64];
    uint64 action_end_index;
};


editor *initialize_editor(memory_allocator a)
{
    editor *result = ALLOCATE(a, editor);
    return result;
}


editor_action ui_editor__null_action()
{
    editor_action result;
    result.type = UI_EDITOR_ACTION_NONE;
    return result;
}

void ui_editor__commit_action(editor *e, editor_action action)
{
    if (action.type != UI_EDITOR_ACTION_NONE)
    {
        e->history[e->action_end_index % ARRAY_COUNT(e->history)] = action;
        e->action_end_index += 1;
        e->history[e->action_end_index % ARRAY_COUNT(e->history)] = ui_editor__null_action();
    }
}

void ui_editor__undo_action(editor *e)
{
    if (e->action_end_index > 0)
    {
        editor_action action = e->history[(e->action_end_index - 1) % ARRAY_COUNT(e->history)];
        if (action.type == UI_EDITOR_ACTION_SELECTION)
        {
            e->selection = action.selection.old_selection;
        }

        if (action.type != UI_EDITOR_ACTION_NONE)
        {
            e->action_end_index -= 1;
        }
    }
}

void ui_editor__redo_action(editor *e)
{
    editor_action action = e->history[e->action_end_index % ARRAY_COUNT(e->history)];
    if (action.type == UI_EDITOR_ACTION_SELECTION)
    {
        e->selection = action.selection.new_selection;
    }

    if (action.type != UI_EDITOR_ACTION_NONE)
    {
        e->action_end_index += 1;
    }
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
            auto inverse_transform = inverse(e->tm_to_root);
            auto mouse_position_local = transform_point(inverse_transform, mouse_position);

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
        if (!editor->active)
        {
            // I found element under mouse, and no element is active! That's good, I can set myself as hot.
            if (editor->hot)
            {
                // There's something hot already, check if it's what I found.
                if (editor->hot == hovered)
                {
                    // The element I found under the mouse is exactly what I have hot. I will do nothing then.
                }
                else
                {
                    // There's nothing hot yet, let's make our element hot.
                    editor->hot = hovered;
                }
            }
            else
            {
                // There's nothing hot yet, let's make our element hot.
                editor->hot = hovered;
            }
        }
        else
        {
            // If there's already something active
            if (editor->hot == hovered)
            {
                // What I found is already hot, do nothing
            }
            else if (hovered == editor->active)
            {
                // What I found is the thing I have active, but it's not hot, make it hot again
                editor->hot = hovered;
            }
        }
    }
    else
    {
        if (editor->hot)
        {
            // I didn't find anything under the mouse, but I have a hot element? Make it cold again.
            editor->hot = null_handle();
        }
    }

    if (get_hold_count(input->mouse[MOUSE_LEFT]))
    {

    }

    if (get_press_count(input->mouse[MOUSE_LEFT]))
    {
        if (editor->hot != editor->selection)
        {
            editor_action action;
            action.type = UI_EDITOR_ACTION_SELECTION;
            action.selection.old_selection = editor->selection;
            action.selection.new_selection = editor->hot;
            ui_editor__commit_action(editor, action);

            editor->active = editor->hot;
            editor->selection = editor->hot;
        }
    }

    if (get_release_count(input->mouse[MOUSE_LEFT]))
    {
        editor->active = null_handle();
    }

    auto ctrl = get_hold_count(input->keyboard[KB_CTRL]);
    auto shift = get_hold_count(input->keyboard[KB_SHIFT]);
    auto z = get_press_count(input->keyboard[KB_Z]);
    
    if (ctrl && !shift && z)
    {
        ui_editor__undo_action(editor);
    }

    if (ctrl && shift && z)
    {
        ui_editor__redo_action(editor);
    }
}


void render_editor(execution_context *context, system *s, editor *e)
{
    auto projection =
        matrix4::translate(-1, -1, 0) *
        matrix4::scale(2.f/context->letterbox_width, 2.f/context->letterbox_height, 1.f);

    for (uint32 element_index = 0; element_index < s->elements.size(); element_index++)
    {
        auto *element = &s->elements[element_index];
        if (element->is_visible == false) continue;

        auto h = make_handle(UI_ELEMENT, element_index);
        bool32 have_graphics = false;
        uint32 width = 0, height = 0;
        for (auto a : iterate_attaches(s, h))
        {
            have_graphics = (a.type == UI_SHAPE || a.type == UI_IMAGE);
            if (have_graphics) {
                auto *d = &s->drawables[a.index];
                width = d->width;
                height = d->height;
                break;
            }
        }
        if (!have_graphics) continue;

        auto model = to_matrix4(element->tm_to_root);

        auto rect = math::rectangle2::from_center_size(V2(0), (float32)width, (float32)height);
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
            matrix4::translate(V3(aabb_center, 0)) *
            matrix4::scale(0.5f * aabb_width, 0.5f * aabb_height, 1);
        draw_frame.view = matrix4::identity();
        draw_frame.projection = projection;
        if (h == e->selection)
        {
            draw_frame.color = V4(1.0,0.2,1,1);
        }
        else if (h == e->hot)
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
