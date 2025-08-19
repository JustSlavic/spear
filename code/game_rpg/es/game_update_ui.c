

void ui_update_transform_recursive(game_state *gs, entity *e)
{
    if (e->ui.tm_updated) return;

    e->ui.tm =
        tm_mul(
            tm_translate(e->ui.position.x, e->ui.position.y, 0),
            tm_mul(
                tm_rotate_z(degrees_to_radians(e->ui.rotation)),
                tm_scale(e->ui.scale.x, e->ui.scale.y, 1)));
    if (e->ui.parent)
    {
        entity *parent = get_entity(gs, e->ui.parent);
        ui_update_transform_recursive(gs, parent);

        e->ui.tm_to_root = tm_mul(parent->ui.tm_to_root, e->ui.tm);
    }
    else
    {
        e->ui.tm_to_root = e->ui.tm;
    }

    e->ui.tm_updated = true;
}

void game_update_ui_transforms(game_state *gs, entity_id_array *ui_elements)
{
    uint32 i;
    for (i = 0; i < ui_elements->count; i++)
    {
        entity *e = get_entity(gs, ui_elements->data[i]);
        e->ui.tm_updated = false;
    }
    for (i = 0; i < ui_elements->count; i++)
    {
        entity *e = get_entity(gs, ui_elements->data[i]);
        ui_update_transform_recursive(gs, e);
    }
}

void game_update_ui(context *ctx, game_state *gs, input *input)
{
    // update_animations(s, inp->dt);
    game_update_ui_transforms(gs, &gs->ui_elements);

    uint32 i;
    vector3 mouse_position = v3f(input->keyboard_and_mouse.viewport_mouse.x, input->keyboard_and_mouse.viewport_mouse.y, 0);

    entity_id hovered = INVALID_ENTITY_ID;
    UNUSED(hovered);
    for (i = 0; i < gs->ui_hoverables.count; i++)
    {
        entity_id eid = gs->ui_hoverables.data[i];
        entity *e = get_entity(gs, eid);

        transform inverse_transform = tm_inverse(e->ui.tm_to_root);
        vector3 mouse_position_local = tm_transform_point3f(inverse_transform, mouse_position);

        bool is_inside = test_aabb_point2f(
            e->ui.hover_area_min.x,
            e->ui.hover_area_min.y,
            e->ui.hover_area_max.x,
            e->ui.hover_area_max.y,
            mouse_position_local.x,
            mouse_position_local.y);

        e->ui.hovered = is_inside;
        if (is_inside)
        {
            hovered = eid;
        }
    }

    // if (hovered)
    // {
    //     if (!s->active)
    //     {
    //         // I found element under mouse, and no element is active! That's good, I can set myself as hot.
    //         if (s->hot)
    //         {
    //             // There's something hot already, check if it's what I found.
    //             if (s->hot == hovered)
    //             {
    //                 // The element I found under the mouse is exactly what I have hot. I will do nothing then.
    //             }
    //             else
    //             {
    //                 for (auto child : iterate_attaches(s, s->hot))
    //                 {
    //                     if (child.type == UI_HOVERABLE)
    //                     {
    //                         auto *p = s->hoverables.data() + child.index;
    //                         p->callbacks.on_leave_internal(s, s->hot);
    //                         p->callbacks.on_leave(s, s->hot);
    //                         break;
    //                     }
    //                 }
    //                 s->hot = hovered;
    //                 for (auto child : iterate_attaches(s, s->hot))
    //                 {
    //                     if (child.type == UI_HOVERABLE)
    //                     {
    //                         auto *p = s->hoverables.data() + child.index;
    //                         p->callbacks.on_enter(s, s->hot);
    //                         p->callbacks.on_enter_internal(s, s->hot);
    //                     }
    //                 }
    //             }
    //         }
    //         else
    //         {
    //             // There's nothing hot yet, let's make our element hot.
    //             s->hot = hovered;
    //             for (auto child : iterate_attaches(s, s->hot))
    //             {
    //                 if (child.type == UI_HOVERABLE)
    //                 {
    //                     auto *p = s->hoverables.data() + child.index;
    //                     p->callbacks.on_enter(s, s->hot);
    //                     p->callbacks.on_enter_internal(s, s->hot);
    //                 }
    //             }
    //         }
    //     }
    //     else
    //     {
    //         // If there's already something active
    //         if (s->hot == hovered)
    //         {
    //             // What I found is already hot, do nothing
    //         }
    //         else if (hovered == s->active)
    //         {
    //             // What I found is the thing I have active, but it's not hot, make it hot again
    //             s->hot = hovered;
    //             for (auto child : iterate_attaches(s, s->hot))
    //             {
    //                 if (child.type == UI_HOVERABLE)
    //                 {
    //                     auto *p = s->hoverables.data() + child.index;
    //                     p->callbacks.on_enter(s, s->hot);
    //                     p->callbacks.on_enter_internal(s, s->hot);
    //                 }
    //             }
    //         }
    //     }
    // }
    // else
    // {
    //     if (s->hot)
    //     {
    //         // I didn't find anything under the mouse, but I have a hot element? Make it cold again.
    //         for (auto child : iterate_attaches(s, s->hot))
    //         {
    //             if (child.type == UI_HOVERABLE)
    //             {
    //                 auto *p = s->hoverables.data() + child.index;
    //                 p->callbacks.on_leave_internal(s, s->hot);
    //                 p->callbacks.on_leave(s, s->hot);
    //                 break;
    //             }
    //         }
    //         s->hot = null_handle();
    //     }
    // }

    // if (get_press_count(inp->mouse[MOUSE_LEFT]))
    // {
    //     s->active = s->hot;
    //     if (s->active)
    //     {
    //         for (auto child : iterate_attaches(s, s->active))
    //         {
    //             if (child.type == UI_CLICKABLE)
    //             {
    //                 if (s->active == s->hot)
    //                 {
    //                     auto *p = s->clickables.data() + child.index;
    //                     p->callbacks.on_press_internal(s, s->active);
    //                     p->callbacks.on_press(s, s->active);
    //                 }

    //                 break;
    //             }
    //         }
    //     }
    // }

    // if (get_release_count(inp->mouse[MOUSE_LEFT]))
    // {
    //     if (s->active)
    //     {
    //         for (auto child : iterate_attaches(s, s->active))
    //         {
    //             if (child.type == UI_CLICKABLE)
    //             {
    //                 auto *p = s->clickables.data() + child.index;
    //                 if (s->active == s->hot)
    //                 {
    //                     p->callbacks.on_release(s, s->active);
    //                 }
    //                 p->callbacks.on_release_internal(s, s->active);
    //             }
    //         }
    //         s->active = null_handle();
    //     }
    // }

}
