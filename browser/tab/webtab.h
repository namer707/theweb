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
#ifndef WEBTAB_H
#define WEBTAB_H

#include <QWidget>
#include <QUrl>

namespace Ui {
    class WebTab;
}

class WebPage;
struct WebTabPrivate;
class WebTab : public QWidget
{
        Q_OBJECT

    public:
        explicit WebTab(WebPage* page = nullptr, QWidget* parent = nullptr);
        ~WebTab();

        QWidget* getTabButton();
        QUrl currentUrl();

        bool isLoading();
        int loadProgress();

    public slots:
        void activated();
        void close();

        void goBack();
        void goForward();
        void navigate(QUrl url);
        void reload();

        void leaveFullScreen();

    signals:
        void spawnTab(WebTab* tab);
        void urlChanged(QUrl url);
        void requestActivate();
        void closeTab();
        void loadProgressChanged();

        void goFullScreen(bool fullscreen);

    private:
        Ui::WebTab *ui;
        WebTabPrivate* d;

        void resizeEvent(QResizeEvent *event);
};

#endif // WEBTAB_H
