// Copyright (C) 2026 UnionTech Software Technology Co., Ltd.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "systemshortcutfilter.h"

#include "core/rootsurfacecontainer.h"
#include "helper.h"
#include "input/globalshortcutfilter.h"
#include "input/iinputeventfilter.h"
#include "input/lockscreenfilter.h"
#include "input/systemshortcutfilter.h"
#include "input/workspacefilter.h"
#include "modules/capture/capture.h"
#include "modules/ddm/ddminterfacev1.h"
#include "qwsession.h"
#include "surface/surfacecontainer.h"
#include "utils/cmdline.h"
#include "wbackend.h"
#include "wcursor.h"

#include <QKeyEvent>
#include <QLoggingCategory>
#include <QMouseEvent>

Q_DECLARE_LOGGING_CATEGORY(treelandCore)

SystemShortcutFilter::SystemShortcutFilter(Helper *helper, QObject *parent)
    : IInputEventFilter(parent)
    , m_helper(helper)
{
}

SystemShortcutFilter::~SystemShortcutFilter() = default;

IInputEventFilter::FilterResult SystemShortcutFilter::beforeDisposeEvent(WSeat *seat,
                                                                         QWindow *,
                                                                         QInputEvent *event)
{
    if (!event)
        return FilterContinue;

    // 1. 处理按键事件
    if (event->type() == QEvent::KeyPress) {
        auto *keyEvent = static_cast<QKeyEvent *>(event);

        // 调试快捷键 (Ctrl+Shift+Meta+F11)
        if (QKeySequence(keyEvent->keyCombination())
            == QKeySequence(Qt::ControlModifier | Qt::ShiftModifier | Qt::MetaModifier
                            | Qt::Key_F11)) {
            if (m_helper->toggleDebugMenuBar())
                return FilterAccepted;
        }

        // TTY切换 (Ctrl+Alt+F1-F12)
        if (keyEvent->modifiers() == (Qt::ControlModifier | Qt::AltModifier)) {
            auto key = keyEvent->key();
            if (key >= Qt::Key_F1 && key <= Qt::Key_F12 && m_helper->m_backend->isSessionActive()) {
                const int vtnr = key - Qt::Key_F1 + 1;
                if (m_helper->m_ddmInterfaceV1 && m_helper->m_ddmInterfaceV1->isConnected()) {
                    m_helper->m_ddmInterfaceV1->switchToVt(vtnr);
                } else {
                    qCDebug(treelandCore) << "DDM is not connected";
                    m_helper->showLockScreen(false);
                    m_helper->m_backend->session()->change_vt(vtnr);
                }
                return FilterAccepted;
            }
        }

        // 捕获选择器 - ESC取消
        if (m_helper->m_captureSelector) {
            if (keyEvent->modifiers() == Qt::NoModifier && keyEvent->key() == Qt::Key_Escape) {
                m_helper->m_captureSelector->cancelSelection();
            }
        }
    }

    // 2. 鼠标/触摸 光标可见性
    if (seat && (event->type() == QEvent::MouseMove || event->type() == QEvent::MouseButtonPress)) {
        seat->cursor()->setVisible(true);
    } else if (seat && event->type() == QEvent::TouchBegin) {
        seat->cursor()->setVisible(false);
    }

    // 3. 手势调用
    m_helper->doGesture(event);

    // 4. 窗口移动/调整大小
    if (m_helper->m_rootSurfaceContainer) {
        auto surface = m_helper->m_rootSurfaceContainer->moveResizeSurface();
        if (surface) {
            if (Q_LIKELY(event->type() == QEvent::MouseMove
                         || event->type() == QEvent::TouchUpdate)) {
                if (!seat)
                    return FilterContinue;

                auto cursor = seat->cursor();
                QMouseEvent *ev = static_cast<QMouseEvent *>(event);

                auto ownsOutput = surface->ownsOutput();
                if (!ownsOutput) {
                    m_helper->m_rootSurfaceContainer->endMoveResize();
                    return FilterContinue;
                }

                auto lastPosition = m_helper->m_fakelastPressedPosition.value_or(
                    cursor->lastPressedOrTouchDownPosition());
                auto increment_pos = ev->globalPosition() - lastPosition;
                m_helper->m_rootSurfaceContainer->doMoveResize(increment_pos);

                return FilterAccepted;
            } else if (event->type() == QEvent::MouseButtonRelease
                       || event->type() == QEvent::TouchEnd) {
                m_helper->m_rootSurfaceContainer->endMoveResize();
                m_helper->m_fakelastPressedPosition.reset();
            }
        }
    }

    // 5. 鼠标按钮和滚轮
    if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonRelease) {
        m_helper->handleLeftButtonStateChanged(event);
    }

    if (event->type() == QEvent::Wheel) {
        m_helper->handleWhellValueChanged(event);
    }

    return FilterContinue;
}
