// Copyright (C) 2026 UnionTech Software Technology Co., Ltd.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "globalshortcutfilter.h"

#include "helper.h"
#include "input/globalshortcutfilter.h"
#include "input/iinputeventfilter.h"
#include "input/lockscreenfilter.h"
#include "input/systemshortcutfilter.h"
#include "input/workspacefilter.h"
#include "modules/shortcut/shortcutcontroller.h"
#include "modules/shortcut/shortcutmanager.h"
#include "utils/cmdline.h"

#include <QKeyEvent>

GlobalShortcutFilter::GlobalShortcutFilter(ShortcutManagerV2 *shortcutManager, QObject *parent)
    : IInputEventFilter(parent)
    , m_shortcutManager(shortcutManager)
    , m_helper(Helper::instance())
{
}

GlobalShortcutFilter::~GlobalShortcutFilter() = default;

IInputEventFilter::FilterResult GlobalShortcutFilter::beforeDisposeEvent(WSeat *,
                                                                         QWindow *,
                                                                         QInputEvent *event)
{
    if (!m_shortcutManager)
        return FilterContinue;

    if (!m_helper->m_captureSelector && m_helper->m_currentMode != Helper::CurrentMode::LockScreen
        && (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease)) {
        auto *keyEvent = static_cast<QKeyEvent *>(event);

        // SKIP Meta+Meta
        if (keyEvent->key() == Qt::Key_Meta && keyEvent->modifiers() == Qt::NoModifier
            && !m_helper->m_singleMetaKeyPendingPressed) {
            return FilterContinue;
        }

        if (m_shortcutManager->controller()->dispatchKeyEvent(keyEvent)) {
            return FilterAccepted;
        }
    }

    return FilterContinue;
}
