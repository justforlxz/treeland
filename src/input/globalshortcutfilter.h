// Copyright (C) 2026 UnionTech Software Technology Co., Ltd.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include "iinputeventfilter.h"

#include <QPointer>

WAYLIB_SERVER_BEGIN_NAMESPACE
class WSeat;
WAYLIB_SERVER_END_NAMESPACE

WAYLIB_SERVER_USE_NAMESPACE

class Helper;
class ShortcutManagerV2;

class GlobalShortcutFilter : public IInputEventFilter
{
    Q_OBJECT

public:
    explicit GlobalShortcutFilter(ShortcutManagerV2 *shortcutManager, QObject *parent = nullptr);
    ~GlobalShortcutFilter() override;

    FilterResult beforeDisposeEvent(WSeat *seat, QWindow *watched, QInputEvent *event) override;

private:
    ShortcutManagerV2 *m_shortcutManager;
    Helper *m_helper;
};
