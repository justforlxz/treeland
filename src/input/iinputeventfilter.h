// Copyright (C) 2026 UnionTech Software Technology Co., Ltd.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <QInputEvent>
#include <QObject>

#include <wglobal.h>

WAYLIB_SERVER_BEGIN_NAMESPACE
class WSeat;
class WSurface;
WAYLIB_SERVER_END_NAMESPACE

WAYLIB_SERVER_USE_NAMESPACE

/**
 * @brief 输入事件过滤器基类接口
 *
 * 提供可链式的事件过滤机制，允许在事件处理的不同阶段拦截和处理输入事件。
 * 每个过滤器返回一个 FilterResult 来指示事件的后续处理。
 */
class IInputEventFilter : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 过滤器结果枚举
     *
     * 定义过滤器如何影响事件的后续分发
     */
    enum FilterResult
    {
        FilterContinue, /// 继续传递给下一个过滤器
        FilterAccepted, /// 事件已处理，停止分发，返回 true 给调用者
        FilterRejected  /// 事件未处理，继续分发，返回 false 给调用者
    };
    Q_ENUM(FilterResult)

    explicit IInputEventFilter(QObject *parent = nullptr);
    virtual ~IInputEventFilter() = default;

    /**
     * @brief 在事件处理前调用
     *
     * @param seat 输入座位
     * @param watched 被观察的窗口
     * @param event 输入事件
     * @return FilterResult 过滤器结果
     *
     * 注意：
     * - 返回 FilterAccepted: 事件已处理，停止分发
     * - 返回 FilterContinue: 继续传递给下一个过滤器
     * - 返回 FilterRejected: 事件未处理，继续分发但标记为未接受
     */
    virtual FilterResult beforeDisposeEvent(WSeat *seat, QWindow *watched, QInputEvent *event) = 0;

    /**
     * @brief 在事件处理后调用
     *
     * @param seat 输入座位
     * @param watched 被观察的表面
     * @param surfaceItem 表面项对象
     * @param oldSurfaceItem 旧的表面项对象
     * @param event 输入事件
     * @return FilterResult 过滤器结果
     */
    virtual FilterResult afterHandleEvent(WSeat *seat,
                                          WSurface *watched,
                                          QObject *surfaceItem,
                                          QObject *oldSurfaceItem,
                                          QInputEvent *event);

    /**
     * @brief 处理未接受的事件
     *
     * @param seat 输入座位
     * @param watched 被观察的窗口
     * @param event 输入事件
     * @return FilterResult 过滤器结果
     */
    virtual FilterResult unacceptedEvent(WSeat *seat, QWindow *watched, QInputEvent *event);

protected:
    /**
     * @brief 检查过滤器是否处于激活状态
     *
     * @return bool 过滤器是否激活
     */
    bool isActive() const;

    /**
     * @brief 设置过滤器的激活状态
     *
     * @param active 是否激活
     */
    void setActive(bool active);

private:
    bool m_active = true;
};
