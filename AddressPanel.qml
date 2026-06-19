import QtQuick 2.15
import QtQuick.Layouts 2.15
import QtQuick.Controls.Basic 2.15

import App.Enums 1.0

Rectangle {
    color: "seagreen"
    opacity: 0.6

    RowLayout {
        id: ipRow

        anchors.centerIn: parent

        spacing: 5

        Label {
            text: qsTr("Client IP: ")
            color: "white"
            font.bold: true
            verticalAlignment: Text.AlignVCenter
        }
        TextField {
            id: firstOctet

            validator: IntValidator {
                bottom: 0
                top: 255
            }
            verticalAlignment: Text.AlignVCenter

            background: Rectangle {
                implicitWidth: 35
                color: "navajowhite"
                border.color: "linen"
                border.width: 2
                radius: 4
            }
        }
        Label {
            text: qsTr(".")
            font.bold: true
            font.pixelSize: 14
            verticalAlignment: Text.AlignVCenter
        }
        TextField {
            id: secondOctet

            validator: IntValidator {
                bottom: 0
                top: 255
            }
            verticalAlignment: Text.AlignVCenter

            background: Rectangle {
                implicitWidth: 35
                color: "navajowhite"
                border.color: "linen"
                border.width: 2
                radius: 4
            }
        }
        Label {
            text: qsTr(".")
            font.bold: true
            font.pixelSize: 14
            verticalAlignment: Text.AlignVCenter
        }
        TextField {
            id: thirdOctet

            validator: IntValidator {
                bottom: 0
                top: 255
            }
            verticalAlignment: Text.AlignVCenter

            background: Rectangle {
                implicitWidth: 35
                color: "navajowhite"
                border.color: "linen"
                border.width: 2
                radius: 4
            }
        }
        Label {
            text: qsTr(".")
            font.bold: true
            font.pixelSize: 14
            verticalAlignment: Text.AlignVCenter
        }
        TextField {
            id: fourthOctet

            validator: IntValidator {
                bottom: 0
                top: 255
            }
            verticalAlignment: Text.AlignVCenter

            background: Rectangle {
                implicitWidth: 35
                color: "navajowhite"
                border.color: "linen"
                border.width: 2
                radius: 4
            }
        }
        Button {
            id: ipButton

            enabled: firstOctet.acceptableInput
                     && secondOctet.acceptableInput
                     && thirdOctet.acceptableInput
                     && fourthOctet.acceptableInput

            text: qsTr("Enter")

            onClicked: {
                udpClient.updateClientAddr(firstOctet.text + "." + secondOctet.text + "." + thirdOctet.text + "." + fourthOctet)
            }
        }
    }
}
