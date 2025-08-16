#include "ui.h"


void ui_init(ui *ui, entity_id root)
{
    ui->root = root;
}

void ui_element_flag_set(ui_element *e, uint32 flag)
{
    e->behaviour_flags = (e->behaviour_flags | flag);
}

void ui_element_flag_clear(ui_element *e, uint32 flag)
{
    e->behaviour_flags = (e->behaviour_flags & (~flag));
}

bool ui_element_flag_test(ui_element *e, uint32 flag)
{
    bool result = (e->behaviour_flags & flag) > 0;
    return result;
}
