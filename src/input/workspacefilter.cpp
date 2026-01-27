// Copyright (C) 2026 UnionTech Software Technology Co., Ltd.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "workspacefilter.h"

#include "helper.h"
#include "workspace/workspace.h"

#include <QKeyEvent>
#include <QMetaObject>

WorkspaceFilter::WorkspaceFilter(Helper *helper, QObject *parent)
    : IInputEventFilter(parent)
    , m_helper(helper)
    , m_singleMetaKeyPendingPressed(false)
{
}

WorkspaceFilter::~WorkspaceFilter() = default;

IInputEventFilter::FilterResult WorkspaceFilter::beforeDisposeEvent(WSeat *,
                                                                    QWindow *,
                                                                    QInputEvent *event)
{
    if (!event)
        return FilterContinue;

    // Meta键状态跟踪
    if (event->type() == QEvent::KeyPress) {
        auto *keyEvent = static_cast<QKeyEvent *>(event);
        switch (keyEvent->key()) {
        case Qt::Key_Meta:
        case Qt::Key_Super_L:
            m_singleMetaKeyPendingPressed = true;
            m_helper->m_singleMetaKeyPendingPressed = true;
            break;
        default:
            m_singleMetaKeyPendingPressed = false;
            m_helper->m_singleMetaKeyPendingPressed = false;
            break;
        }
    }

    // 任务切换退出 (Ctrl/Meta 释放)
    if (event->type() == QEvent::KeyRelease && !m_helper->m_captureSelector) {
        auto *keyEvent = static_cast<QKeyEvent *>(event);
        if (m_helper->m_taskSwitch && m_helper->m_taskSwitch->property("switchOn").toBool()) {
            if (keyEvent->key() == Qt::Key_Alt || keyEvent->key() == Qt::Key_Meta) {
                auto *helper = Helper::instance();
                auto filter = helper->workspace()->currentFilter();
                filter->setFilterAppId("");
                m_helper->setCurrentMode(Helper::CurrentMode::Normal);
                QMetaObject::invokeMethod(m_helper->m_taskSwitch, "exit");
                return FilterAccepted;
            }
        }
    }

    return FilterContinue;
}
