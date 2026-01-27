// Copyright (C) 2026 UnionTech Software Technology Co., Ltd.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include "iinputeventfilter.h"

#include <QPointer>

WAYLIB_SERVER_BEGIN_NAMESPACE
class WSeat;
class WSurface;
WAYLIB_SERVER_END_NAMESPACE

WAYLIB_SERVER_USE_NAMESPACE

class Helper;
class LockScreen;

class LockScreenFilter : public IInputEventFilter
{
    Q_OBJECT

public:
    explicit LockScreenFilter(Helper *helper, QObject *parent = nullptr);
    ~LockScreenFilter() override;

    FilterResult beforeDisposeEvent(WSeat *seat, QWindow *watched, QInputEvent *event) override;

private:
    Helper *m_helper;
    LockScreen *m_lockScreen;
};
