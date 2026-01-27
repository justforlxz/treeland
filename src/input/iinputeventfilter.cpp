// Copyright (C) 2026 UnionTech Software Technology Co., Ltd.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "iinputeventfilter.h"

#include <WSeat>

IInputEventFilter::IInputEventFilter(QObject *parent)
    : QObject(parent)
    , m_active(true)
{
}

IInputEventFilter::FilterResult IInputEventFilter::afterHandleEvent(
    [[maybe_unused]] WSeat *seat,
    [[maybe_unused]] WSurface *watched,
    [[maybe_unused]] QObject *surfaceItem,
    [[maybe_unused]] QObject *oldSurfaceItem,
    [[maybe_unused]] QInputEvent *event)
{
    return FilterContinue;
}

IInputEventFilter::FilterResult IInputEventFilter::unacceptedEvent(
    [[maybe_unused]] WSeat *seat,
    [[maybe_unused]] QWindow *watched,
    [[maybe_unused]] QInputEvent *event)
{
    return FilterContinue;
}

bool IInputEventFilter::isActive() const
{
    return m_active;
}

void IInputEventFilter::setActive(bool active)
{
    m_active = active;
}
