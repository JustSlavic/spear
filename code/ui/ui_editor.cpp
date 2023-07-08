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
    handle hovered;
    handle selection;

    // @note: current_action is like a scratchpad where you
    // record your action and then commit to history later.
    editor_action current_action;
    editor_action history[32];
    isize action_end_index;
};


void update_editor(system *s, editor *e, input_devices *input)
{

}

void render_editor(execution_context *context, system *s, editor *e)
{

}


} // namespace ui
