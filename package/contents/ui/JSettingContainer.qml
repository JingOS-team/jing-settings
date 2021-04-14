/*
 * Copyright 2021 Rui Wang <wangrui@jingos.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
import QtQuick 2.6
import QtQuick.Controls 2.2 as Controls
import org.kde.kirigami 2.5 as Kirigami

import org.kde.plasma.settings 0.1

Kirigami.Page {
    id: container

    property QtObject kcm
    property Item internalPage

    leftPadding: 0
    rightPadding: 0
    topPadding: 0
    bottomPadding: 0
    globalToolBarStyle : Kirigami.ApplicationHeaderStyle.None
    flickable: internalPage.flickable

    background: Rectangle{
        color: "transparent"
    }

    onInternalPageChanged: {
        internalPage.parent = contentItem;
        internalPage.anchors.fill = contentItem;
    }
    
    onActiveFocusChanged: {
        if (activeFocus) {
            internalPage.forceActiveFocus();
        }
    }

    Component.onCompleted: {
        kcm.load()
    }

    data: [
        Connections {
            target: kcm
            onPagePushed: {
                pageStack.push(kcmContainer.createObject(pageStack, {"internalPage": page}));
            }
            onPageRemoved: pageStack.pop();
        },
        Connections {
            target: pageStack
            onPageRemoved: {
                if (kcm.needsSave) {
                    kcm.save()
                }
                if (page == container) {
                    page.destroy();
                }
            }
        }
    ]
}
