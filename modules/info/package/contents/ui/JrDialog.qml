/*
 * Copyright 2021 Wang Rui <wangrui@jingos.com>
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

import QtQuick 2.7
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import org.kde.kirigami 2.15 as Kirigami
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0

Kirigami.JArrowPopup {
    id: root

    property string uid
    property var description: ""
    property int px: 0
    property int py: 0
    property int selectIndex

    signal menuSelectChanged(int value)

    property int screenWidth: 888 * appScale
    property int screenHeight: 648 * appScale

    property int statusbar_height : 22 * appScale
    property int statusbar_icon_size: 22 * appScale
    property int default_setting_item_height: 45 * appScale

    property int marginTitle2Top : 44  * appScale
    property int marginItem2Title : 18  * appScale
    property int marginLeftAndRight : 20  * appScale
    property int marginItem2Top : 24 * appScale
    property int radiusCommon: 10  * appScale
    property int fontNormal: 14 * appFontSize
    property string dialogTextColor: Kirigami.JTheme.majorForeground

    width: 300 * appScale
    height: contentItem.height +  root.topPadding + root.bottomPadding

    blurBackground.arrowX: width * 0.75
    blurBackground.arrowWidth: 16 * appScale
    blurBackground.arrowHeight: 11 * appScale
    blurBackground.arrowPos: Kirigami.JRoundRectangle.ARROW_TOP

    x: px
    y: py
    modal: true
    Overlay.modal: Rectangle {
                    color: "#00000000"
                }
    focus: true

    // background: Rectangle {
    //     id: background
    //     color: "transparent"
    // }

    contentItem: Rectangle {
        id: contentItem

        anchors.left: parent.left
        anchors.right: parent.right

        width: parent.width
        height: default_setting_item_height * 5 + 14 * appScale

        radius: radiusCommon
        // layer.enabled: true
        // layer.effect: DropShadow {
        //     horizontalOffset: 0
        //     radius:20
        //     samples:25
        //     color: "#1A000000"
        //     verticalOffset: 0
        //     spread: 0
        // }
        color: "transparent"//Kirigami.JTheme.floatBackground//"#ffffff"

        Rectangle {
            id: menu_content

            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
                bottom: parent.bottom
                topMargin: 14 * appScale
                bottomMargin: 14 * appScale
                leftMargin: 14 * appScale
                rightMargin: 14 * appScale
            }

            width: root.width - 14 * 2 * appScale
            color: "transparent"

            Column {
                id: menu_list

                spacing: 0

                Rectangle {
                    width: menu_content.width
                    height: default_setting_item_height
                    color: "transparent"

                    Text {
                        anchors {
                            left: parent.left
                            leftMargin: marginLeftAndRight
                            verticalCenter: parent.verticalCenter
                        }

                        font.pixelSize: fontNormal
                        text: i18n("Every day")
                        color: dialogTextColor
                    }

                    Image {
                        anchors {
                            right: parent.right
                            rightMargin: marginLeftAndRight
                            verticalCenter: parent.verticalCenter
                        }

                        width: 17 * appScale
                        height: 17 * appScale

                        source: "../image/menu_select.png"
                        visible: selectIndex == 0 ? true : false
                    }

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            selectIndex = 0
                            menuSelectChanged(selectIndex)
                            root.close()
                        }
                    }
                    Kirigami.Separator {
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.leftMargin: marginLeftAndRight
                        anchors.rightMargin: marginLeftAndRight
                        color: Kirigami.JTheme.dividerForeground//"#f0f0f0"
                    }
                }

                Rectangle {
                    width: menu_content.width
                    height: default_setting_item_height

                    color: "transparent"

                    Text {
                        anchors {
                            left: parent.left
                            leftMargin: marginLeftAndRight
                            verticalCenter: parent.verticalCenter
                        }

                        font.pixelSize: fontNormal
                        text: i18n("Every two days")
                        color: dialogTextColor
                    }

                    Image {
                        anchors {
                            right: parent.right
                            rightMargin: marginLeftAndRight
                            verticalCenter: parent.verticalCenter
                        }

                        width: 17 * appScale
                        height: 17 * appScale

                        visible: selectIndex == 1 ? true : false
                        source: "../image/menu_select.png"
                    }

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            selectIndex = 1
                            menuSelectChanged(selectIndex)
                            root.close()
                        }
                    }
                    Kirigami.Separator {
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.leftMargin: marginLeftAndRight
                        anchors.rightMargin: marginLeftAndRight
                        color: Kirigami.JTheme.dividerForeground//"#f0f0f0"
                    }
                }

                Rectangle {
                    width: menu_content.width
                    height: default_setting_item_height

                    color: "transparent"

                    Text {
                        anchors {
                            left: parent.left
                            leftMargin: marginLeftAndRight
                            verticalCenter: parent.verticalCenter
                        }

                        font.pixelSize: fontNormal
                        text: i18n("Weekly")
                        color: dialogTextColor
                    }

                    Image {
                        anchors {
                            right: parent.right
                            rightMargin: marginLeftAndRight
                            verticalCenter: parent.verticalCenter
                        }

                        width: 17 * appScale
                        height: 17 * appScale
                        source: "../image/menu_select.png"
                        visible: selectIndex == 2 ? true : false
                    }

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            selectIndex = 2
                            menuSelectChanged(selectIndex)
                            root.close()
                        }
                    }
                    Kirigami.Separator {
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.leftMargin: marginLeftAndRight
                        anchors.rightMargin: marginLeftAndRight
                        color: Kirigami.JTheme.dividerForeground//"#f0f0f0"
                    }
                }

                Rectangle {
                    width: menu_content.width
                    height: default_setting_item_height

                    color: "transparent"

                    Text {
                        anchors {
                            left: parent.left
                            leftMargin: marginLeftAndRight
                            verticalCenter: parent.verticalCenter
                        }

                        font.pixelSize: fontNormal
                        text: i18n("Every two weeks")
                        color: dialogTextColor
                    }

                    Image {
                        anchors {
                            right: parent.right
                            rightMargin: marginLeftAndRight
                            verticalCenter: parent.verticalCenter
                        }

                        width: 17 * appScale
                        height: 17 * appScale
                        source: "../image/menu_select.png"
                        visible: selectIndex == 3 ? true : false
                    }

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            selectIndex = 3
                            menuSelectChanged(selectIndex)
                            root.close()
                        }
                    }
                    Kirigami.Separator {
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.leftMargin: marginLeftAndRight
                        anchors.rightMargin: marginLeftAndRight
                        color: Kirigami.JTheme.dividerForeground//"#f0f0f0"
                    }
                }

                Rectangle {
                    width: menu_content.width
                    height: default_setting_item_height

                    color: "transparent"

                    Text {
                        anchors {
                            left: parent.left
                            leftMargin: marginLeftAndRight
                            verticalCenter: parent.verticalCenter
                        }

                        font.pixelSize: fontNormal
                        text: i18n("Never")
                        color: dialogTextColor
                    }
                    
                    Image {
                        anchors {
                            right: parent.right
                            rightMargin: marginLeftAndRight
                            verticalCenter: parent.verticalCenter
                        }

                        width: 17 * appScale
                        height: 17 * appScale
                        source: "../image/menu_select.png"
                        visible: selectIndex == 4 ? true : false
                    }

                    MouseArea {
                        anchors.fill: parent
                        
                        onClicked: {
                            selectIndex = 4
                            menuSelectChanged(selectIndex)
                            root.close()
                        }
                    }
                }
            }
        }
    }
}
