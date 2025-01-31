#pragma once

#include "view/view-impl.hpp"
#include "wayfire/signal-definitions.hpp"
#include "wayfire/signal-provider.hpp"
#include <wayfire/view.hpp>
#include <wayfire/scene-input.hpp>
#include <wayfire/compositor-view.hpp>
#include <wayfire/core.hpp>
#include <wayfire/seat.hpp>

#include <wayfire/nonstd/wlroots-full.hpp>
/**
 * An interface for scene nodes which interact with the keyboard.
 */
class view_keyboard_interaction_t : public wf::keyboard_interaction_t
{
    std::weak_ptr<wf::view_interface_t> view;

  public:
    view_keyboard_interaction_t(wayfire_view _view)
    {
        this->view = _view->weak_from_this();
    }

    void handle_keyboard_enter(wf::seat_t *seat) override
    {
        if (auto ptr = view.lock())
        {
            if (ptr->get_wlr_surface())
            {
                auto pressed_keys = seat->get_pressed_keys();

                auto kbd = wlr_seat_get_keyboard(seat->seat);
                wlr_seat_keyboard_notify_enter(seat->seat, ptr->get_wlr_surface(),
                    pressed_keys.data(), pressed_keys.size(), kbd ? &kbd->modifiers : NULL);
            }
        }
    }

    void handle_keyboard_leave(wf::seat_t *seat) override
    {
        if (auto ptr = view.lock())
        {
            wlr_seat_keyboard_notify_clear_focus(seat->seat);
        }
    }

    void handle_keyboard_key(wf::seat_t *seat, wlr_keyboard_key_event event) override
    {
        wlr_seat_keyboard_notify_key(seat->seat, event.time_msec, event.keycode, event.state);
    }
};
