#ifndef _SPEAR_UI_H
#define _SPEAR_UI_H

#include <corelibs/base.h>
#include <corelibs/math.h>

typedef enum
{
    UiBehaviour_Nothing   = 0,
    UiBehaviour_Visible   = 1 << 0,
    UiBehaviour_Hoverable = 1 << 1,
    UiBehaviour_Clickable = 1 << 2,
} ui_behaviour;

typedef struct
{
    entity_id parent;

    uint32 behaviour_flags;

    vector2 position;
    vector2 scale;
    float32 rotation;
    float32 width;
    float32 height;

    // UiBehaviour_Visible
    bool32 is_visible;
    // UiBehaviour_Hoverable or UiBehaviour_Clickable
    vector2 area_min, area_max;
} ui_element;

typedef struct
{
    entity_id root;

    entity_id hot; // The element has 'hoverable' behaviour AND mouse pointer above it
    entity_id active; // The element has 'clickable' behaviour AND mouse clicked on it
} ui;


void ui_element_init(ui_element *e);
void ui_element_hoverable_init(ui_element *e);
void ui_element_clickable_init(ui_element *e);


#endif // _SPEAR_UI_H
