import QtQuick 2.15
import QtQuick.Controls.Basic 2.15
import QtQuick.Layouts 2.15

import App.Enums 1.0

Rectangle {
    id: notifBack

    required property string message
    required property int type

    signal finished()

    width: parent.width
    height: notifCol.height
    opacity: 0.75
    color: "white"
    radius: 4
    border {
        width: 4
        color: "gray"
    }

    Column {
        id: notifCol

        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        spacing: 5

        RowLayout {
            id: notifHeaderLayout

            width: parent.width

            Text {
                id: notifType

                Layout.alignment: Qt.AlignVCenter

                topPadding: 5
                text: getMessageType()
                color: "black"
                font.pixelSize: 12
                font.bold: true
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            Text {
                id: notifTime

                Layout.fillWidth: true
                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                topPadding: 5
                text: Qt.formatDateTime(new Date(), "hh:mm:ss d.MM.yyyy")
                color: "black"
                font.pixelSize: 12
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            Text {
                id: cross

                Layout.alignment: Qt.AlignVCenter

                topPadding: 5
                text: "X"
                color: "black"
                font.pixelSize: 12
                font.bold: true

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true

                    onEntered: cross.color = "red"
                    onExited: cross.color = "black"
                    onPressed: cross.color = Qt.lighter("red", 1.2)
                    onReleased: cross.color = "red"
                    onClicked: finished()
                }
            }
        }

        Text {
            id: notifText

            width: parent.width
            height: implicitHeight
            bottomPadding: 5
            text: notifBack.message
            color: "black"
            font.pixelSize: 10
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
            wrapMode: Text.WordWrap
        }
    }

    function getMessageType() {
        switch(notifBack.type) {
        case Enums.Info: {
            notifType.color = "dimgray"
            return "Инфо"
        }
        case Enums.Send: {
            notifType.color = "seagreen"
            return "Отправка"
        }
        case Enums.Recieve: {
            notifType.color = "royalblue"
            return "Приём"
        }
        case Enums.Error: {
            notifType.color = "orangered"
            return "Ошибка"
        }
        default: {
            break
        }
        }
    }
}
