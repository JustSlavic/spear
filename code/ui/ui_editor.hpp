#ifndef UI_EDITOR_HPP
#define UI_EDITOR_HPP

#include <base.hpp>
#include <execution_context.hpp>
#include "ui.hpp"


namespace ui {



struct editor;


void update_editor(system *s, editor *e, input *input);
void render_editor(execution_context *context, system *s, editor *e);


} // namespace ui



#endif // UI_UI_EDITOR_HPP
