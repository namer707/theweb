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
#ifndef SINGLEINSTANCEMANAGER_H
#define SINGLEINSTANCEMANAGER_H

#include <QObject>
#include <QMap>
#include <QVariant>

struct SingleInstanceManagerPrivate;
class SingleInstanceManager : public QObject
{
        Q_OBJECT
    public:
        explicit SingleInstanceManager(QObject *parent = nullptr);
        ~SingleInstanceManager();

        bool shouldRun();
        void handOverControl(QVariantMap options);

    signals:
        void optionsRequested(QVariantMap options);

    public slots:

    private:
        SingleInstanceManagerPrivate* d;
};

#endif // SINGLEINSTANCEMANAGER_H
