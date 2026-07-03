import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "components"

ApplicationWindow {
    id: root

    width: 900
    height: 640
    minimumWidth: 420
    minimumHeight: 560
    visible: true
    title: "cryptKX"
    color: "#090d12"

    Rectangle {
        anchors.fill: parent
        color: "#090d12"
    }

    ColumnLayout {
        anchors.centerIn: parent
        width: Math.min(root.width - 32, 600)
        spacing: 18

        Text {
            Layout.alignment: Qt.AlignHCenter
            text: "cryptKX"
            color: "#f5f7fa"
            font.pixelSize: 34
            font.weight: Font.DemiBold
        }

        PasswordCard {
            Layout.fillWidth: true
        }
    }
}
