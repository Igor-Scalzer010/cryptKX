import QtQuick
import QtQuick.Controls

Button {
    id: control

    property bool secondary: false
    property bool success: false

    implicitHeight: 44
    padding: 0

    contentItem: Text {
        text: control.text
        color: control.enabled ? (control.secondary ? "#e5edf5" : "#071014") : "#6f7d8d"
        font.pixelSize: 14
        font.weight: Font.DemiBold
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        radius: 8
        color: {
            if (!control.enabled) {
                return "#151d26"
            }
            if (control.success) {
                return "#1d6f5d"
            }
            if (control.secondary) {
                return control.down ? "#23303d" : control.hovered ? "#202b37" : "#151e28"
            }
            return control.down ? "#50c9a7" : control.hovered ? "#76f3ce" : "#63e6be"
        }
        border.color: control.secondary ? "#2b3a4a" : "transparent"
        border.width: control.secondary ? 1 : 0
    }
}
