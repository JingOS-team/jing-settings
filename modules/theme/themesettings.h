/***************************************************************************
 *                                                                         *
 *   Copyright 2015 Sebastian Kügler <sebas@kde.org>                       *
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

#ifndef THEMESETTINGS_H
#define THEMESETTINGS_H

#include <QObject>
#include <QVariant>

namespace Plasma {
    class Theme;
}

#include <KQuickAddons/ConfigModule>

class ThemeSettings : public KQuickAddons::ConfigModule
{
    Q_OBJECT

    Q_PROPERTY(QString themeName READ themeName WRITE setThemeName NOTIFY themeNameChanged)

    public:
        ThemeSettings(QObject* parent, const QVariantList& args);
        virtual ~ThemeSettings();

        QString themeName();
        void setThemeName(const  QString &theme);

    Q_SIGNALS:
        void themeNameChanged();

    private:
        Plasma::Theme *m_theme;
        QString m_themeName;
};

#endif // THEMESETTINGS_H
