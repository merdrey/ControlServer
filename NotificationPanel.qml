import QtQuick 2.15
import QtQuick.Layouts 2.15
import QtQuick.Controls.Basic 2.15

import App.Enums 1.0

Rectangle {
    color: "steelblue"

    Rectangle {
        anchors.fill: parent
        anchors.margins: 20
        radius: 5
        color: "floralwhite"
        border {
            color: "goldenrod"
            width: 3
        }

        NotificationManager {
            id: notifManager

            anchors.fill: parent
            anchors.margins: 10
        }
    }

    Connections {
        target: udpClient

        function onSendMessage(message, type) {
            notifManager.addNotification(type, message)
        }
    }
}
