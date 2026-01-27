// Copyright (C) 2026 UnionTech Software Technology Co., Ltd.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "lockscreenfilter.h"

#include "helper.h"

LockScreenFilter::LockScreenFilter(Helper *helper, QObject *parent)
    : IInputEventFilter(parent)
    , m_helper(helper)
{
}

LockScreenFilter::~LockScreenFilter() = default;

IInputEventFilter::FilterResult LockScreenFilter::beforeDisposeEvent(WSeat *,
                                                                     QWindow *,
                                                                     QInputEvent *)
{
    if (m_helper->m_currentMode == Helper::CurrentMode::LockScreen) {
        return FilterAccepted;
    }
    return FilterContinue;
}
