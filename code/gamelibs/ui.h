#ifndef _SPEAR_UI_H
#define _SPEAR_UI_H

#include <corelibs/base.h>
#include <corelibs/math.h>
#include <gamelibs/entity_manager.h>

enum
{
    UiBehaviour_Invalid   = 0,

    UiBehaviour_Visible   = 0x1,
    UiBehaviour_Hoverable = 0x2,
    UiBehaviour_Clickable = 0x4,
};

typedef struct
{
    entity_id parent;
    uint32 behaviour_flags;

    // UiElement
    vector2 position;
    float32 width;
    float32 height;
    vector2 scale;
    float32 rotation;

    // Cache
    transform tm;
    transform tm_to_root;
    bool32 tm_updated;

    // UiDrawable
    bool is_visible;

    // UiBehaviour_Hoverable
    vector2 hover_area_min, hover_area_max;
    bool hovered;
    // UiBehaviour_Clickable
} ui_element;

typedef struct
{
    entity_id hot; // The element has 'hoverable' behaviour AND mouse pointer above it
    entity_id active; // The element has 'clickable' behaviour AND mouse clicked on it
} ui;


void ui_element_flag_set(ui_element *e, uint32 flag);
void ui_element_flag_clear(ui_element *e, uint32 flag);
bool ui_element_flag_test(ui_element *e, uint32 flag);


#endif // _SPEAR_UI_H
