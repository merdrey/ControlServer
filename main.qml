import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 2.15
import QtQuick.Controls.Basic 2.15

import App.Enums 1.0

Window {
    minimumWidth: 640
    minimumHeight: 480
    visible: true
    title: qsTr("Server")

    NotificationManager {
        id: notifManager

        anchors.fill: parent
        anchors.margins: 10
        z: 1
    }

    ColumnLayout {
        id: mainLayout

        anchors.fill: parent

        Rectangle {
            Layout.minimumHeight: 50
            Layout.fillWidth: true
            Layout.topMargin: 10
            Layout.leftMargin: 15
            Layout.rightMargin: 15

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

        Rectangle {
            Layout.preferredHeight: controlsColumn.height * 1.2
            Layout.fillWidth: true
            Layout.topMargin: 10
            Layout.leftMargin: 15
            Layout.rightMargin: 15

            border {
                width: 3
                color: "gray"
            }

            RowLayout {
                id: colorPickerRow

                anchors {
                    fill: parent
                    margins: 10
                }
                spacing: 5
                Rectangle {
                    id: colorDemoRect

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: Qt.rgba(rSlider.value, gSlider.value, bSlider.value, 1)
                    border {
                        width: 3
                        color: "#000000"
                    }
                }
                Column {
                    id: controlsColumn

                    spacing: 5
                    Row {
                        spacing: 5
                        Text {
                            text: qsTr("Red Color:")
                            anchors.verticalCenter: parent.verticalCenter
                            width: 70
                        }
                        Slider {
                            id: rSlider

                            from: 0
                            to: 1
                        }
                    }

                    Row {
                        spacing: 5
                        Text {
                            text: qsTr("Green Color:")
                            anchors.verticalCenter: parent.verticalCenter
                            width: 70
                        }
                        Slider {
                            id: gSlider

                            from: 0
                            to: 1
                        }
                    }

                    Row {
                        spacing: 5
                        Text {
                            text: qsTr("Blue Color:")
                            anchors.verticalCenter: parent.verticalCenter
                            width: 70
                        }
                        Slider {
                            id: bSlider

                            from: 0
                            to: 1
                        }
                    }

                    Row {
                        spacing: 5
                        Text {
                            text: qsTr("Current Color:")
                            anchors.verticalCenter: parent.verticalCenter
                            width: 70
                        }

                        TextField {
                            id: currentColor

                            text: colorDemoRect.color
                            readOnly: true

                            background: Rectangle {
                                border.color: "#000000"
                            }
                        }
                    }

                    Button {
                        id: sendColorBtn

                        text: qsTr("Отправить цвет")
                        height: 50

                        onClicked: {
                            udpClient.sendCommand(colorDemoRect.color, Enums.ComFillScreen)
                        }
                    }
                }
            }
        }

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.bottomMargin: 10
            Layout.leftMargin: 15
            Layout.rightMargin: 15

            border {
                width: 3
                color: "gray"
            }

            ColumnLayout {
                id: messageColumn

                anchors {
                    fill: parent
                    margins: 10
                }
                spacing: 5

                ScrollView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    TextArea {
                        id: textInput

                        property int maxLength: 50
                        placeholderText: qsTr("Input Text to Display on Screen (max" + maxLength + " symbols)")
                        wrapMode: TextEdit.Wrap
                        background: Rectangle {
                            border.color: "#000000"
                        }

                        onTextChanged: {
                            if (textInput.length > maxLength)
                                text = text.slice(0, maxLength)
                        }
                    }

                }

                Button {
                    id: sendTextBtn

                    enabled: textInput.length > 0
                    text: qsTr("Отправить текст")
                    height: 50

                    onClicked: {
                        udpClient.sendCommand(textInput.text, Enums.ComSendText)
                    }
                }
            }
        }
    }

    Connections {
        target: udpClient

        function onSendMessage(message, type) {
            notifManager.addNotification(type, message)
        }
    }
}
