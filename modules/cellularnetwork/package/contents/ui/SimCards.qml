/*
 *   Copyright 2020 Dimitris Kardarakos <dimkard@posteo.net>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2 or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Library General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

import org.kde.kcm 1.2 as KCM
import QtQuick.Layouts 1.2
import QtQuick 2.7
import QtQuick.Controls 2.2 as Controls
import org.kde.kirigami 2.10 as Kirigami
import MeeGo.QOfono 0.2

KCM.SimpleKCM {
    title: i18n("SIM Cards")

    OfonoManager {
        id: ofonoManager
    }

    Kirigami.CardsListView {
        width: parent.width
        model: ofonoManager.modems.length
        delegate: Kirigami.AbstractCard {
            property int modemId: model.index

            width: parent.width
            header: Kirigami.Heading {
                text: ofonoManager.modems.length == 1 ? i18n("SIM Card") : i18np("SIM Card 1", "SIM Card %1", modemId+1)
                level: 2
                horizontalAlignment: Text.AlignHCenter
            }

            contentItem: Kirigami.FormLayout {
                width: parent.width

                Controls.Label {
                    visible: ofonoSimManager.subscriberIdentity != ""
                    wrapMode: Text.WordWrap
                    text: ofonoSimManager.subscriberIdentity
                    Kirigami.FormData.label: i18n("IMSI:")
                    Layout.fillWidth: true
                }

                Controls.Label {
                    visible: ofonoSimManager.mobileCountryCode != ""
                    wrapMode: Text.WordWrap
                    text: ofonoSimManager.mobileCountryCode
                    Kirigami.FormData.label: i18n("MCC:")
                    Layout.fillWidth: true
                }

                Controls.Label {
                    visible: ofonoSimManager.mobileNetworkCode != ""
                    wrapMode: Text.WordWrap
                    text: ofonoSimManager.mobileNetworkCode
                    Kirigami.FormData.label: i18n("MNC:")
                    Layout.fillWidth: true
                }

                Controls.Label {
                    visible: ofonoSimManager.cardIdentifier != ""
                    wrapMode: Text.WordWrap
                    text: ofonoSimManager.cardIdentifier
                    Kirigami.FormData.label: i18n("ICCID:")
                    Layout.fillWidth: true
                }

                Controls.Label {
                    visible: ofonoSimManager.serviceProviderName != ""
                    wrapMode: Text.WordWrap
                    text: ofonoSimManager.serviceProviderName
                    Kirigami.FormData.label: i18n("Service Provider:")
                    Layout.fillWidth: true
                }

                Controls.Label {
                    visible: ofonoSimManager.subscriberNumbers.length > 0
                    wrapMode: Text.WordWrap
                    text: ofonoSimManager.subscriberNumbers.join(",")
                    Kirigami.FormData.label: i18np("Subscriber Number:", "Subscriber Numbers:", ofonoSimManager.subscriberNumbers.length)
                    Layout.fillWidth: true
                }

                Controls.Label {
                    visible: ofonoSimManager.preferredLanguages.length > 0
                    text: ofonoSimManager.preferredLanguages.join(", ")
                    wrapMode: Text.WordWrap
                    Kirigami.FormData.label: i18np("Preferred Language:", "Preferred Languages:", ofonoSimManager.preferredLanguages.length)
                    Layout.fillWidth: true
                }
            }

            OfonoSimManager {
                id: ofonoSimManager

                modemPath: ofonoManager.modems[modemId]
            }
        }
    }
}
