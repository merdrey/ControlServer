import QtQuick 2.15
import QtQuick.Controls.Basic

import App.Enums 1.0

Item {
    id: root

    width: parent.width
    height: 70

    required property string message
    required property int type

    property int duration: 5 * 1000

    signal finished()

    Rectangle {
        id: itemBack

        anchors.fill: parent
        opacity: 0.75
        color: "white"
        radius: 4
        border {
            width: 4
            color: "gray"
        }

        Column {
            anchors.fill: parent
            anchors.topMargin: 5
            spacing: 5
            Text {
                id: notifType

                anchors {
                    left: parent.left
                    right: parent.right
                    leftMargin: 10
                }

                text: getMessageType()
                color: "black"
                font.pixelSize: 12
                font.bold: true
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            Text {
                id: notifText

                anchors {
                    left: parent.left
                    right: parent.right
                    leftMargin: 5
                }
                text: root.message
                color: "black"
                font.pixelSize: 10
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
                wrapMode: Text.WordWrap
            }
        }

        SequentialAnimation {
                running: true

                PauseAnimation { duration: root.duration }

                ScriptAction {
                    script: root.finished()
                }
            }
    }

    function getMessageType() {
        switch(root.type) {
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

    onFinished: {
        destroy()
    }
}
