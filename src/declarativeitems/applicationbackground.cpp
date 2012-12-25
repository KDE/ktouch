/*
 * Copyright 2012  Sebastian Gottfried <sebastiangottfried@web.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "applicationbackground.h"

#include <QApplication>
#include <QPainter>

#include <plasma/theme.h>

ApplicationBackground::ApplicationBackground(QDeclarativeItem *parent) :
    QDeclarativeItem(parent)
{
    setFlag(QDeclarativeItem::ItemHasNoContents, false);
}

void ApplicationBackground::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    const QColor appBgColor = QApplication::palette().color(QPalette::Window);
    QColor color = Plasma::Theme::defaultTheme()->color(Plasma::Theme::BackgroundColor);
    const int similarity = qAbs(appBgColor.lightness() - color.lightness());

    color.setAlpha(qMax(0, similarity - 50));

    painter->fillRect(boundingRect(), color);
}
