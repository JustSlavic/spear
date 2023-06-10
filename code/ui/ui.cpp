#include "ui.hpp"


namespace ui {


bool32 button(system *s, uint64 id)
{
    bool32 result = false;
    handle h = get_handle_from_hash_table(s, id);
    if (which_array(h) == array_of::GROUPS)
    {
        result = (h == s->pressed);
    }
    return result;
}


} // namespace ui
