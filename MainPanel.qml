import QtQuick 2.15
import QtQuick.Layouts 2.15
import QtQuick.Controls.Basic 2.15

import App.Enums 1.0

ColumnLayout {
    id: mainLayout

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
            id: imageColumn

            anchors {
                top: parent.top
                bottom: parent.bottom
                left: parent.left
                margins: 10
            }
            spacing: 5

            Image {
                id: qrImage

                Layout.fillHeight: true
                Layout.alignment: Qt.AlignHCenter
                source: "qrc:/qr-code.gif"
                fillMode: Image.PreserveAspectFit
            }

            Button {
                id: sendImgBtn

                Layout.alignment: Qt.AlignHCenter
                height: 50
                text: qsTr("Отправить изображение")
                onClicked: {
                    udpClient.sendCommand(qrImage.source, Enums.ComSendImg)
                }
            }
        }

        ColumnLayout {
            id: messageColumn

            anchors {
                top: parent.top
                bottom: parent.bottom
                left: imageColumn.right
                right: parent.right
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
