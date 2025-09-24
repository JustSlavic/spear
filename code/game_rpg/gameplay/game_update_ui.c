

void ui_update_transform_recursive(game_state *gs, entity *e)
{
    if (e->ui.tm_updated) return;

    e->ui.tm =
        transform_mul(
            transform_translate((float) e->ui.position.x, (float) e->ui.position.y, 0),
            transform_mul(
                transform_rotate_z(degrees_to_radians(e->ui.rotation)),
                transform_scale(e->ui.scale.x, e->ui.scale.y, 1)));
    if (e->ui.parent)
    {
        entity *parent = get_entity(gs, e->ui.parent);
        ui_update_transform_recursive(gs, parent);

        e->ui.tm_to_root = transform_mul(parent->ui.tm_to_root, e->ui.tm);
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
    vector3 mouse_position = vector3_create((float) input->keyboard_and_mouse.viewport_mouse.x, (float) input->keyboard_and_mouse.viewport_mouse.y, 0);

    entity_id hovered = INVALID_ENTITY_ID;
    UNUSED(hovered);
    for (i = 0; i < gs->ui_hoverables.count; i++)
    {
        entity_id eid = gs->ui_hoverables.data[i];
        entity *e = get_entity(gs, eid);

        transform inverse_transform = transform_inverse(e->ui.tm_to_root);
        vector3 mouse_position_local = transform_transform_point3f(inverse_transform, mouse_position);

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

    if (hovered)
    {
        if (!gs->ui.active)
        {
            // I found element under mouse, and no element is active!
            // That's good, maybe I can set myself as hot.
            if (gs->ui.hot)
            {
                // There's something hot already, check if it's what I found.
                if (gs->ui.hot == hovered)
                {
                    // The element I found under the mouse is exactly what I have hot.
                    // I will do nothing then.
                }
                else
                {
                    // @todo: call the 'on_hoverable_leave' event on hot element
                    // printf("UI: leave ui element (eid=%d)\n", gs->ui.hot);
                    push_event(gs, (event){ .tag = Event_UiLeave, .eid = gs->ui.hot, });
                    gs->ui.hot = hovered;
                    // @todo: call the 'on_hoverable_enter' event on hot element
                    // printf("UI: enter ui element (eid=%d)\n", gs->ui.hot);
                    push_event(gs, (event){ .tag = Event_UiEnter, .eid = gs->ui.hot, });
                }
            }
            else
            {
                // There's nothing hot yet, let's make our element hot.
                gs->ui.hot = hovered;
                // @todo: call the 'on_hoverable_enter' event on hot element
                // printf("UI: enter ui element (eid=%d)\n", gs->ui.hot);
                push_event(gs, (event){ .tag = Event_UiEnter, .eid = gs->ui.hot, });
            }
        }
        else
        {
            // If there's already something active
            if (gs->ui.hot == hovered)
            {
                // What I found is already hot, do nothing
            }
            else if (hovered == gs->ui.active)
            {
                // What I found is the thing I have active, but it's not hot,
                // make it hot again
                gs->ui.hot = hovered;
                // @todo: call the 'on_hoverable_enter' event on hot element
                // printf("UI: enter ui element (eid=%d)\n", gs->ui.hot);
                push_event(gs, (event){ .tag = Event_UiEnter, .eid = gs->ui.hot, });
            }
        }
    }
    else
    {
        if (gs->ui.hot)
        {
            // I didn't find anything under the mouse,
            // but I have a hot element? Make it cold again.

            // @todo: call the 'on_hoverable_leave' event on hot element
            // printf("UI: leave ui element (eid=%d)\n", gs->ui.hot);
            push_event(gs, (event){ .tag = Event_UiLeave, .eid = gs->ui.hot, });
            gs->ui.hot = INVALID_ENTITY_ID;
        }
    }

    if (input_button_get_press_count(input->keyboard_and_mouse.buttons[Mouse_Left]))
    {
        gs->ui.active = gs->ui.hot;
        if (gs->ui.active)
        {
            entity *e = get_entity(gs, gs->ui.active);
            if (ui_element_flag_test(&e->ui, UiBehaviour_Clickable))
            {
                push_event(gs, (event){ .tag = Event_UiPress, .eid = gs->ui.hot, });
            }
        }
    }

    if (input_button_get_release_count(input->keyboard_and_mouse.buttons[Mouse_Left]))
    {
        if (gs->ui.active)
        {
            entity *e = get_entity(gs, gs->ui.active);
            if (ui_element_flag_test(&e->ui, UiBehaviour_Clickable))
            {
                push_event(gs, (event){ .tag = Event_UiRelease, .eid = gs->ui.hot, });
            }
            gs->ui.active = INVALID_ENTITY_ID;
        }
    }
}
