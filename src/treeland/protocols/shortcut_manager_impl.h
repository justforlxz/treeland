// Copyright (C) 2023 Dingyuan Zhang <lxz@mkacg.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <wayland-server-core.h>

class ShortcutManager;
struct ztreeland_shortcut_manager_v1 {
    struct wl_global *global;

    struct {
        struct wl_signal destroy;
    } events;

    void *data;

    struct wl_list contexts;  // link to treeland_socket_context_v1.link

    struct wl_listener display_destroy;

    ShortcutManager *manager = nullptr;
};

struct ztreeland_shortcut_context_v1_state {
    int32_t key_code;
    int32_t modify;
};

struct ztreeland_shortcut_context_v1 {
    struct ztreeland_shortcut_manager_v1      *manager;
    struct ztreeland_shortcut_context_v1_state state;
    struct wl_list link;  // treeland_shell_manager_v1.contexts
};

void shortcut_manager_bind(struct wl_client *client,
                           void             *data,
                           uint32_t          version,
                           uint32_t          id);
void shortcut_manager_handle_display_destroy(struct wl_listener *listener,
                                             void               *data);