/****************************************
 *
 *   INSERT-PROJECT-NAME-HERE - INSERT-GENERIC-NAME-HERE
 *   Copyright (C) 2019 Victor Tran
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * *************************************/
#include "tabbutton.h"
#include <QPainter>
#include <the-libs_global.h>

struct TabButtonPrivate {
    QColor backgroundCol;
    QColor foregroundCol;

    int loadProgress = 0;
    bool showLoadProgress = false;
};

TabButton::TabButton(QWidget *parent) : QPushButton(parent)
{
    d = new TabButtonPrivate();

    d->backgroundCol = this->palette().color(QPalette::Window);
    d->foregroundCol = this->palette().color(QPalette::WindowText);
}

TabButton::~TabButton() {
    delete d;
}

QColor TabButton::color()
{
    return d->backgroundCol.darker();
}

QColor TabButton::foregroundColor()
{
    return d->foregroundCol;
}

void TabButton::setLoadProgress(int progress, bool show)
{
    d->loadProgress = progress;
    d->showLoadProgress = show;
    this->update();
}

void TabButton::setIcon(QIcon icon)
{
    QPushButton::setIcon(icon);

    QPixmap pm = icon.pixmap(this->iconSize());

    qulonglong red = 0, green = 0, blue = 0;

    int totalPixels = 0;
    QImage im = pm.toImage();
    for (int i = 0; i < pm.width(); i++) {
        for (int j = 0; j < pm.height(); j++) {
            QColor c = im.pixelColor(i, j);
            if (c.alpha() != 0) {
                //i = pm.width();
                //j = pm.height();

                //c.setAlpha(255);
                //QPalette pal = coverWidget->palette();
                //pal.setColor(QPalette::Window, c.darker(150));
                //coverWidget->setPalette(pal);
                red += c.red();
                green += c.green();
                blue += c.blue();
                totalPixels++;
            }
        }
    }

    QColor background, foreground;
    QPalette pal = this->palette();
    int averageCol = (background.red() + background.green() + background.blue()) / 3;

    if (totalPixels == 0) {
        background = QPalette().color(QPalette::Window);
        foreground = QPalette().color(QPalette::WindowText);
    } else {
        background = QColor(red / totalPixels, green / totalPixels, blue / totalPixels);

        if (averageCol < 127) {
            background = background.darker(150);
            foreground = Qt::white;
        } else {
            background = background.lighter(150);
            foreground = Qt::black;
        }
    }
    pal.setColor(QPalette::Button, background);
    pal.setColor(QPalette::ButtonText, foreground);
    this->setPalette(pal);
    d->backgroundCol = background;
    d->foregroundCol = foreground;
}

void TabButton::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QPalette pal = this->palette();
    QRect rect(0, 0, this->width(), this->height());

    painter.setPen(Qt::transparent);

    QBrush brush;
    QPen textPen;

#if defined(Q_OS_WIN) || defined(Q_OS_MACOS) && 0
    brush = QBrush(pal.color(QPalette::Highlight));
#else
    brush = QBrush(pal.color(QPalette::Button));
#endif

    /*if (button->state & QStyle::State_HasFocus) {
        brush = QBrush(pal.color(QPalette::Button).lighter(125));
    }

    if (button->state & QStyle::State_MouseOver) {
        brush = QBrush(pal.color(QPalette::Button).lighter());
    }*/

    if (this->isChecked()) {
        brush = QBrush(pal.color(QPalette::Button).darker());
    }

    textPen = pal.color(QPalette::ButtonText);

    painter.setBrush(brush);
    painter.drawRect(rect);

    QString text = this->text();

    QRect textRect;
    textRect.setLeft(rect.left() + (rect.width() / 2) - (this->fontMetrics().width(text) / 2));
    textRect.setWidth(this->fontMetrics().width(text));
    textRect.setTop(rect.top() + (rect.height() / 2) - (this->fontMetrics().height() / 2));
    textRect.setHeight(this->fontMetrics().height());

    if (!this->icon().isNull()) {
        QRect iconRect;
        int fullWidth = textRect.width() + this->iconSize().width();
        int iconLeft = rect.left() + (rect.width() / 2) - (fullWidth / 2);

        iconRect.setLeft(iconLeft);
        iconRect.setTop(rect.top() + (rect.height() / 2) - (this->iconSize().height() / 2));
        iconRect.setSize(this->iconSize());

        textRect.moveLeft(iconRect.right() + 4);

        QIcon icon = this->icon();
        QImage image = icon.pixmap(this->iconSize()).toImage();
        image = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);

        painter.drawImage(iconRect, image);
    }

    //Draw text
    painter.setPen(textPen);
    painter.drawText(textRect, Qt::AlignCenter, text.remove("&"));

    if (d->showLoadProgress) {
        //Draw progress indication
        painter.setPen(Qt::transparent);
        painter.setBrush(QColor(0, 150, 0));
        painter.drawRect(0, this->height() - SC_DPI(3), static_cast<int>(this->width() * static_cast<float>(d->loadProgress / 100)), this->height() - SC_DPI(3));
    }
}
