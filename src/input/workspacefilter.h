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
class TaskSwitchAnimation;
class WorkspaceModel;

class WorkspaceFilter : public IInputEventFilter
{
    Q_OBJECT

public:
    explicit WorkspaceFilter(Helper *helper, QObject *parent = nullptr);
    ~WorkspaceFilter() override;

    FilterResult beforeDisposeEvent(WSeat *seat, QWindow *watched, QInputEvent *event) override;

private:
    bool handleTaskSwitch(QKeyEvent *event);
    void trackMetaKeyState(QKeyEvent *event);

    Helper *m_helper;
    TaskSwitchAnimation *m_taskSwitch;
    bool m_singleMetaKeyPendingPressed;
};
