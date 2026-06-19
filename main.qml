import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 2.15
import QtQuick.Controls.Basic 2.15

import App.Enums 1.0

Window {
    minimumWidth: 1200
    minimumHeight: 800
    visible: true
    title: qsTr("Server")

    property bool addrPanelVisible: false
    property bool notifPanelVisible: false

    ColumnLayout {
        id: verSplit

        anchors.fill: parent
        spacing: 0

        AddressPanel {
            id: addrPanel

            visible: addrPanelVisible
            Layout.fillWidth: true
            Layout.preferredHeight: 50
        }

        SplitButton {
            id: addrSplitButton

            Layout.fillWidth: true
            Layout.preferredHeight: 5

            onClicked: addrPanelVisible = !addrPanelVisible
        }

        RowLayout {
            id: horSplit

            Layout.fillHeight: true
            Layout.fillWidth: true
            spacing: 0

            MainPanel {
                id: mainPanel

                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            SplitButton {
                id: notifSplitButton

                Layout.fillHeight: true
                Layout.preferredWidth: 5

                onClicked: notifPanelVisible = !notifPanelVisible
            }

            NotificationPanel {
                id: notifPanel

                visible: notifPanelVisible
                Layout.fillHeight: true
                Layout.preferredWidth: 300
            }
        }
    }
}
