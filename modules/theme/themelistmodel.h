/*
 * ThemeListModel
 * Copyright (C) 2002 Karol Szwed <gallium@kde.org>
 * Copyright (C) 2002 Daniel Molkentin <molkentin@kde.org>
 * Copyright (C) 2007 Urs Wolfer <uwolfer @ kde.org>
 * Copyright (C) 2009 by Davide Bettio <davide.bettio@kdemail.net>
 * Copyright (C) 2015 Marco Martin <mart@kde.org>

 * Portions Copyright (C) 2007 Paolo Capriotti <p.capriotti@gmail.com>
 * Portions Copyright (C) 2007 Ivan Cukic <ivan.cukic+kde@gmail.com>
 * Portions Copyright (C) 2008 by Petri Damsten <damu@iki.fi>
 * Portions Copyright (C) 2000 TrollTech AS.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef THEMELISTMODEL_H
#define THEMELISTMODEL_H

#include <QAbstractListModel>

namespace Plasma
{
class FrameSvg;
}

// Theme selector code by Andre Duffeck (modified to add package description)
class ThemeInfo
{
public:
    QString package;
    QString description;
    QString author;
    QString version;
    QString themeRoot;
};

class ThemeListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum { PackageNameRole = Qt::UserRole, PackageDescriptionRole = Qt::UserRole + 1, PackageAuthorRole = Qt::UserRole + 2, PackageVersionRole = Qt::UserRole + 3 };

    ThemeListModel(QObject *parent = nullptr);
    ~ThemeListModel() override;

    QHash<int, QByteArray> roleNames() const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QModelIndex indexOf(const QString &path) const;
    void reload();
    void clearThemeList();

    Q_INVOKABLE QVariantMap get(int index) const;

private:
    QHash<int, QByteArray> m_roleNames;

    QMap<QString, ThemeInfo> m_themes;
};

#endif
