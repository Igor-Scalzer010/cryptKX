import QtQuick
import QtQuick.Controls

CheckBox {
    id: control

    implicitHeight: 42
    spacing: 10

    indicator: Rectangle {
        x: 12
        y: control.topPadding + (control.availableHeight - height) / 2
        width: 20
        height: 20
        radius: 5
        color: control.checked ? "#63e6be" : "transparent"
        border.color: control.checked ? "#63e6be" : "#526173"
        border.width: 1

        Rectangle {
            width: 8
            height: 8
            radius: 2
            anchors.centerIn: parent
            color: "#081014"
            visible: control.checked
        }
    }

    contentItem: Text {
        leftPadding: 42
        rightPadding: 12
        text: control.text
        color: control.enabled ? "#e5edf5" : "#718092"
        font.pixelSize: 14
        font.weight: Font.DemiBold
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        radius: 8
        color: control.hovered ? "#182330" : "#0d141c"
        border.color: control.checked ? "#3d8a78" : "#263544"
        border.width: 1
    }
}
