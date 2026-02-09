import QtQuick 2.15

Item {
    id: manager

    property int maxNotif: 4
    property int notifCount: 0

    function addNotification(mType, mText) {
        notifCount++
        var component = Qt.createComponent("Notification.qml")
        if (component.status === Component.Ready) {
            var notification = component.createObject(notificationArea, {
                message: mText,
                type : mType
            })

            if (notification) {
                arrangeNotifications()
                notification.finished.connect(function() {
                    notification.destroy()
                    notifCount--
                })
            }
        }
    }

    function arrangeNotifications() {
        var spacing = 10
        var yPos = spacing

        if (notifCount > maxNotif) {
            notificationArea.children[0].destroy()
            notifCount--
        }

        for (var i = notificationArea.children.length - 1; i >= 0; i--) {
            var child = notificationArea.children[i]
            if (child && child.hasOwnProperty("y")) {
                child.y = yPos
                yPos += child.height + spacing
            }
        }
    }

    Item {
        id: notificationArea

        width: 200
        height: 250
        anchors {
            top: parent.top
            right: parent.right
        }
    }
}
