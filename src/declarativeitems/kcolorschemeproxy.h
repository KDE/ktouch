/*
 *  SPDX-FileCopyrightText: 2017 Sebastian Gottfried <sebastian.gottfried@posteo.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef KCOLORSCHEMEPROXY_H
#define KCOLORSCHEMEPROXY_H

#include <QObject>
#include <KColorScheme>
#include <QPalette>

class KColorSchemeProxy : public QObject
{
    Q_OBJECT
    Q_ENUMS(ColorGroup)
    Q_ENUMS(ColorSet)
    Q_ENUMS(ShadeRole)

    Q_PROPERTY(ColorGroup colorGroup READ colorGroup WRITE setColorGroup NOTIFY colorGroupChanged)
    Q_PROPERTY(ColorSet colorSet READ colorSet WRITE setColorSet NOTIFY colorSetChanged)

    Q_PROPERTY(QColor normalBackground READ normalBackground NOTIFY paletteChanged)
    Q_PROPERTY(QColor alternateBackground READ alternateBackground NOTIFY paletteChanged)
    Q_PROPERTY(QColor activeBackground READ activeBackground NOTIFY paletteChanged)
    Q_PROPERTY(QColor linkBackground READ linkBackground NOTIFY paletteChanged)
    Q_PROPERTY(QColor visitedBackground READ visitedBackground NOTIFY paletteChanged)
    Q_PROPERTY(QColor negativeBackground READ negativeBackground NOTIFY paletteChanged)
    Q_PROPERTY(QColor neutralBackground READ neutralBackground NOTIFY paletteChanged)
    Q_PROPERTY(QColor positiveBackground READ positiveBackground NOTIFY paletteChanged)

    Q_PROPERTY(QColor focusDecoration READ focusDecoration NOTIFY paletteChanged)
    Q_PROPERTY(QColor hoverDecoration READ hoverDecoration NOTIFY paletteChanged)

    Q_PROPERTY(QColor normalText READ normalText NOTIFY paletteChanged)
    Q_PROPERTY(QColor activeText READ activeText NOTIFY paletteChanged)
    Q_PROPERTY(QColor linkText READ linkText NOTIFY paletteChanged)
    Q_PROPERTY(QColor visitedText READ visitedText NOTIFY paletteChanged)
    Q_PROPERTY(QColor negativeText READ negativeText NOTIFY paletteChanged)
    Q_PROPERTY(QColor neutralText READ neutralText NOTIFY paletteChanged)
    Q_PROPERTY(QColor positiveText READ positiveText NOTIFY paletteChanged)

    Q_PROPERTY(QColor lightShade READ lightShade NOTIFY paletteChanged)
    Q_PROPERTY(QColor midlightShade READ midlightShade NOTIFY paletteChanged)
    Q_PROPERTY(QColor midShade READ midShade NOTIFY paletteChanged)
    Q_PROPERTY(QColor darkShade READ darkShade NOTIFY paletteChanged)
    Q_PROPERTY(QColor shadowShade READ shadowShade NOTIFY paletteChanged)

    Q_PROPERTY(qreal contrast READ contrast NOTIFY paletteChanged)

public:
    enum ColorGroup { Active, Disabled, Inactive, NColorGroups, Current, All, Normal = Active };

    enum ColorSet {
        View,
        Window,
        Button,
        Selection,
        Tooltip,
        Complementary
    };

    enum ShadeRole {
        LightShade,
        MidlightShade,
        MidShade,
        DarkShade,
        ShadowShade
    };

    explicit KColorSchemeProxy(QObject* parent = 0);

    ColorGroup colorGroup() const;
    void setColorGroup(ColorGroup group);


    ColorSet colorSet() const;
    void setColorSet(ColorSet colorSet);

    QColor normalBackground() const;
    QColor alternateBackground() const;
    QColor activeBackground() const;
    QColor linkBackground() const;
    QColor visitedBackground() const;
    QColor negativeBackground() const;
    QColor neutralBackground() const;
    QColor positiveBackground() const;

    QColor focusDecoration() const;
    QColor hoverDecoration() const;

    QColor normalText() const;
    QColor activeText() const;
    QColor linkText() const;
    QColor visitedText() const;
    QColor negativeText() const;
    QColor neutralText() const;
    QColor positiveText() const;

    QColor lightShade() const;
    QColor midlightShade() const;
    QColor midShade() const;
    QColor darkShade() const;
    QColor shadowShade() const;

    qreal contrast() const;

    Q_INVOKABLE QColor shade(const QColor &color, ShadeRole role) const;
    Q_INVOKABLE QColor shade(const QColor &color, ShadeRole role, qreal contrast, qreal chromaAdjust = 0.0) const;

signals:
    void colorGroupChanged();
    void colorSetChanged();
    void paletteChanged();

private:
    QPalette::ColorGroup m_colorGroup;
    KColorScheme::ColorSet m_colorSet;
    KColorScheme m_colorScheme;
};

#endif // KCOLORSCHEMEPROXY_H
