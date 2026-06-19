import QtQuick 2.15
import QtQuick.Controls.Basic 2.15

Button {
    id: splitButton

    hoverEnabled: true

    background: Rectangle {
        color: splitButton.pressed ? "lightsteelblue" :
                                     splitButton.hovered ? "lightslategray" : "slategray"
    }
}
