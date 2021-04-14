/***************************************************************************
 *                                                                         *
 *   Copyright 2019 Nicolas Fella <nicolas.fella@gmx.de>                   *
 *             2021 Wang Rui <wangrui@jingos.com> 
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

#include "modulesmodel.h"

#include <QQuickItem>
#include <QSet>

#include <KPackage/PackageLoader>
#include <KPluginFactory>
#include <KPluginLoader>

#include <KDeclarative/KDeclarative>

#include <QDebug>

ModulesModel::ModulesModel(QObject *parent)
    : QAbstractListModel(parent)
{
    for (const KPluginMetaData &pluginMetaData : KPackage::PackageLoader::self()->listPackages(QString(), "kpackage/kcms/")) {
        bool isCurrentPlatform = false;
        if (KDeclarative::KDeclarative::runtimePlatform().isEmpty()) {
            isCurrentPlatform = true;
        } else {
            for (const QString &platform : KDeclarative::KDeclarative::runtimePlatform()) {
                if (pluginMetaData.formFactors().contains(platform)) {
                    isCurrentPlatform = true;
                }
            }
        }
        if (isCurrentPlatform) {
            Data d;
            d.plugin = pluginMetaData;
            m_plugins.append(d);
        }
    }
}

QVariant ModulesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= rowCount()) {
        return {};
    }

    // NOTE: as the kcm is lazy loading, this needs to not be const
    // a cleaner alternative, would be a ConfigModule *loadKcm(pluginId) method, which also wouldn't risk erroneous kcm instantiation when it shouldn't
    Data &d = const_cast<ModulesModel *>(this)->m_plugins[index.row()];

    switch (role) {
        case NameRole:
            return d.plugin.name();
        case DescriptionRole:
            return d.plugin.description();
        case IconNameRole:
            return d.plugin.iconName();
        case KcmRole: {
            if (!d.kcm) {
                d.kcm = instantiateKcm(d.plugin.pluginId());
            }

            return QVariant::fromValue(d.kcm.data());
        }
    }

    return {};
}

int ModulesModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : m_plugins.size();
}

QHash<int, QByteArray> ModulesModel::roleNames() const
{
    return {{NameRole, "name"}, {DescriptionRole, "description"}, {IconNameRole, "iconName"}, {KcmRole, "kcm"}};
}

KQuickAddons::ConfigModule *ModulesModel::instantiateKcm(const QString &name) const
{
    const QString pluginPath = KPluginLoader::findPlugin(QLatin1String("kcms/") + name);

    KPluginLoader loader(pluginPath);
    KPluginFactory *factory = loader.factory();

    KQuickAddons::ConfigModule *kcm = nullptr;

    if (!factory) {
        qWarning() << "Error loading KCM plugin:" << loader.errorString();
    } else {
        kcm = factory->create<KQuickAddons::ConfigModule>(const_cast<ModulesModel *>(this));
        if (!kcm) {
            qWarning() << "Error creating object from plugin" << loader.fileName();
        }
    }
    return kcm;
}
