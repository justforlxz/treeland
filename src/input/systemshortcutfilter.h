// Copyright (C) 2026 UnionTech Software Technology Co., Ltd.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include "iinputeventfilter.h"

#include <QPointer>

#include <optional>

WAYLIB_SERVER_BEGIN_NAMESPACE
class WSeat;
WAYLIB_SERVER_END_NAMESPACE

WAYLIB_SERVER_USE_NAMESPACE

class Helper;
class RootSurfaceContainer;
class CaptureSourceSelector;

class SystemShortcutFilter : public IInputEventFilter
{
    Q_OBJECT

public:
    explicit SystemShortcutFilter(Helper *helper, QObject *parent = nullptr);
    ~SystemShortcutFilter() override;

    FilterResult beforeDisposeEvent(WSeat *seat, QWindow *watched, QInputEvent *event) override;

private:
    bool handleDebugShortcut(QKeyEvent *event);
    bool handleTtySwitch(QKeyEvent *event);
    bool handleWindowMoveResize(WSeat *seat, QInputEvent *event);
    void handleMouseButtons(QInputEvent *event);
    void handleWheel(QInputEvent *event);
    bool handleCaptureSelector(QKeyEvent *event);
    void handleGesture(QInputEvent *event);

    Helper *m_helper;
    std::optional<QPointF> m_fakelastPressedPosition;
};
