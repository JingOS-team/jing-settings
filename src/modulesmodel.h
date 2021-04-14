/***************************************************************************
 *                                                                         *
 *   Copyright 2019 Nicolas Fella <nicolas.fella@gmx.de>                   *
 *             2021 Wang Rui <wangrui@jingos.com>                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#pragma once

#include <QAbstractListModel>
#include <QVector>

#include <KPackage/Package>
#include <KPluginMetaData>
#include <KQuickAddons/ConfigModule>

struct Data {
    KPluginMetaData plugin;
    QPointer<KQuickAddons::ConfigModule> kcm;
};

class ModulesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum ModelRoles { NameRole = Qt::DisplayRole, IconNameRole = Qt::DecorationRole, DescriptionRole = Qt::UserRole + 1, KcmRole };

public:
    ModulesModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QHash<int, QByteArray> roleNames() const override;

private:
    KQuickAddons::ConfigModule *instantiateKcm(const QString &name) const;

    QVector<Data> m_plugins;
};
