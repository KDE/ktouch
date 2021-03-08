/*
 *  SPDX-FileCopyrightText: 2017 Sebastian Gottfried <sebastian.gottfried@posteo.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "kcolorschemeproxy.h"

#include <QGuiApplication>

KColorSchemeProxy::KColorSchemeProxy(QObject* parent) :
    QObject(parent),
    m_colorGroup(QPalette::Active),
    m_colorSet(KColorScheme::View),
    m_colorScheme(m_colorGroup, m_colorSet)
{
    auto app = qobject_cast<QGuiApplication*>(QCoreApplication::instance());
    if (app)
    {
        connect(app, SIGNAL(paletteChanged(QPalette)), SIGNAL(paletteChanged()));
    }
}

KColorSchemeProxy::ColorGroup KColorSchemeProxy::colorGroup() const
{
    return static_cast<ColorGroup>(m_colorGroup);
}

void KColorSchemeProxy::setColorGroup(ColorGroup group)
{
    auto newColorGroup = static_cast<QPalette::ColorGroup>(group);
    if (newColorGroup != m_colorGroup)
    {
        m_colorGroup = newColorGroup;
        emit colorGroupChanged();
        m_colorScheme = KColorScheme(m_colorGroup, m_colorSet);
        emit paletteChanged();
    }
}

KColorSchemeProxy::ColorSet KColorSchemeProxy::colorSet() const
{
    return static_cast<ColorSet>(m_colorSet);
}

void KColorSchemeProxy::setColorSet(ColorSet colorSet)
{
    auto newColorSet = static_cast<KColorScheme::ColorSet>(colorSet);
    if (newColorSet != m_colorSet)
    {
        m_colorSet = newColorSet;
        emit colorSetChanged();
        m_colorScheme = KColorScheme(m_colorGroup, m_colorSet);
        emit paletteChanged();
    }
}

QColor KColorSchemeProxy::normalBackground() const
{
    return m_colorScheme.background(KColorScheme::NormalBackground).color();
}

QColor KColorSchemeProxy::alternateBackground() const
{
    return m_colorScheme.background(KColorScheme::AlternateBackground).color();
}

QColor KColorSchemeProxy::activeBackground() const
{
    return m_colorScheme.background(KColorScheme::ActiveBackground).color();
}

QColor KColorSchemeProxy::linkBackground() const
{
    return m_colorScheme.background(KColorScheme::LinkBackground).color();
}

QColor KColorSchemeProxy::visitedBackground() const
{
    return m_colorScheme.background(KColorScheme::VisitedBackground).color();
}

QColor KColorSchemeProxy::negativeBackground() const
{
    return m_colorScheme.background(KColorScheme::NegativeBackground).color();
}

QColor KColorSchemeProxy::neutralBackground() const
{
    return m_colorScheme.background(KColorScheme::NeutralBackground).color();
}

QColor KColorSchemeProxy::positiveBackground() const
{
    return m_colorScheme.background(KColorScheme::PositiveBackground).color();
}

QColor KColorSchemeProxy::focusDecoration() const
{
    return m_colorScheme.decoration(KColorScheme::FocusColor).color();
}

QColor KColorSchemeProxy::hoverDecoration() const
{
    return m_colorScheme.decoration(KColorScheme::HoverColor).color();
}

QColor KColorSchemeProxy::normalText() const
{
    return m_colorScheme.foreground(KColorScheme::NormalText).color();
}

QColor KColorSchemeProxy::activeText() const
{
    return m_colorScheme.foreground(KColorScheme::ActiveText).color();
}

QColor KColorSchemeProxy::linkText() const
{
    return m_colorScheme.foreground(KColorScheme::LinkText).color();
}

QColor KColorSchemeProxy::visitedText() const
{
    return m_colorScheme.foreground(KColorScheme::VisitedText).color();
}

QColor KColorSchemeProxy::negativeText() const
{
    return m_colorScheme.foreground(KColorScheme::NegativeText).color();
}

QColor KColorSchemeProxy::neutralText() const
{
    return m_colorScheme.foreground(KColorScheme::NeutralText).color();
}

QColor KColorSchemeProxy::positiveText() const
{
    return m_colorScheme.foreground(KColorScheme::PositiveText).color();
}

QColor KColorSchemeProxy::lightShade() const
{
    return m_colorScheme.shade(KColorScheme::LightShade);
}

QColor KColorSchemeProxy::midlightShade() const
{
    return m_colorScheme.shade(KColorScheme::MidlightShade);
}

QColor KColorSchemeProxy::midShade() const
{
    return m_colorScheme.shade(KColorScheme::MidShade);
}

QColor KColorSchemeProxy::darkShade() const
{
    return m_colorScheme.shade(KColorScheme::DarkShade);
}

QColor KColorSchemeProxy::shadowShade() const
{
    return m_colorScheme.shade(KColorScheme::ShadowShade);
}

qreal KColorSchemeProxy::contrast() const
{
    return KColorScheme::contrastF();
}

QColor KColorSchemeProxy::shade(const QColor& color, ShadeRole role) const
{
    return KColorScheme::shade(color, static_cast<KColorScheme::ShadeRole>(role));
}

QColor KColorSchemeProxy::shade(const QColor& color, ShadeRole role, qreal contrast, qreal chromaAdjust) const
{
    return KColorScheme::shade(color, static_cast<KColorScheme::ShadeRole>(role), contrast, chromaAdjust);
}
