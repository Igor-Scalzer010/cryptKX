import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root

    readonly property int selectedLength: Math.round(lengthSlider.value)

    color: "#111820"
    radius: 8
    border.color: passwordController.hasError ? "#f06575" : "#243241"
    border.width: 1
    implicitHeight: content.implicitHeight + 48

    ColumnLayout {
        id: content

        anchors.fill: parent
        anchors.margins: 24
        spacing: 18

        RowLayout {
            Layout.fillWidth: true
            spacing: 12

            Text {
                text: "Password"
                color: "#f5f7fa"
                font.pixelSize: 18
                font.weight: Font.DemiBold
            }

            Item {
                Layout.fillWidth: true
            }

            Rectangle {
                id: copiedFeedback

                visible: false
                radius: 8
                color: "#11392f"
                border.color: "#63e6be"
                implicitWidth: copiedLabel.implicitWidth + 20
                implicitHeight: 30

                Text {
                    id: copiedLabel

                    anchors.centerIn: parent
                    text: "Copied"
                    color: "#c7fff0"
                    font.pixelSize: 13
                    font.weight: Font.DemiBold
                }
            }
        }

        TextField {
            id: passwordField

            Layout.fillWidth: true
            Layout.preferredHeight: 52
            readOnly: true
            selectByMouse: true
            text: passwordController.password
            placeholderText: "Your password appears here"
            color: "#f5f7fa"
            placeholderTextColor: "#6f7d8d"
            selectionColor: "#2f806d"
            selectedTextColor: "#ffffff"
            font.pixelSize: 17
            font.family: "monospace"
            leftPadding: 16
            rightPadding: 16

            background: Rectangle {
                radius: 8
                color: "#0b1117"
                border.color: passwordController.hasError ? "#f06575" : passwordField.activeFocus ? "#63e6be" : "#2b3848"
                border.width: 1
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 12

            Text {
                text: "Length"
                color: "#d8dee9"
                font.pixelSize: 14
                font.weight: Font.DemiBold
            }

            Item {
                Layout.fillWidth: true
            }

            Rectangle {
                radius: 8
                color: "#0b1117"
                border.color: "#2b3848"
                implicitWidth: 48
                implicitHeight: 32

                Text {
                    anchors.centerIn: parent
                    text: root.selectedLength
                    color: "#f5f7fa"
                    font.pixelSize: 14
                    font.weight: Font.DemiBold
                }
            }
        }

        Slider {
            id: lengthSlider

            Layout.fillWidth: true
            from: 8
            to: 64
            value: 20
            stepSize: 1
            snapMode: Slider.SnapAlways

            background: Rectangle {
                x: lengthSlider.leftPadding
                y: lengthSlider.topPadding + lengthSlider.availableHeight / 2 - height / 2
                width: lengthSlider.availableWidth
                height: 6
                radius: 3
                color: "#273544"

                Rectangle {
                    width: lengthSlider.visualPosition * parent.width
                    height: parent.height
                    radius: 3
                    color: "#63e6be"
                }
            }

            handle: Rectangle {
                x: lengthSlider.leftPadding + lengthSlider.visualPosition * (lengthSlider.availableWidth - width)
                y: lengthSlider.topPadding + lengthSlider.availableHeight / 2 - height / 2
                width: 22
                height: 22
                radius: 11
                color: lengthSlider.pressed ? "#c7fff0" : "#f5f7fa"
                border.color: "#63e6be"
                border.width: 2
            }
        }

        GridLayout {
            Layout.fillWidth: true
            columns: root.width < 460 ? 1 : 2
            columnSpacing: 12
            rowSpacing: 12

            OptionCheckBox {
                id: uppercaseOption

                Layout.fillWidth: true
                text: "Uppercase"
                checked: true
            }

            OptionCheckBox {
                id: lowercaseOption

                Layout.fillWidth: true
                text: "Lowercase"
                checked: true
            }

            OptionCheckBox {
                id: digitsOption

                Layout.fillWidth: true
                text: "Digits"
                checked: true
            }

            OptionCheckBox {
                id: symbolsOption

                Layout.fillWidth: true
                text: "Symbols"
                checked: true
            }
        }

        Rectangle {
            Layout.fillWidth: true
            visible: passwordController.hasError
            radius: 8
            color: "#351820"
            border.color: "#f06575"
            implicitHeight: errorText.implicitHeight + 22

            Text {
                id: errorText

                anchors.left: parent.left
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: 14
                anchors.rightMargin: 14
                text: passwordController.errorMessage
                color: "#ffd7dc"
                font.pixelSize: 13
                wrapMode: Text.WordWrap
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 12

            PrimaryButton {
                Layout.fillWidth: true
                text: "Generate"
                onClicked: root.generate()
            }

            PrimaryButton {
                Layout.preferredWidth: 132
                text: "Copy"
                secondary: true
                success: copiedFeedback.visible
                enabled: passwordController.password.length > 0
                onClicked: root.copyPassword()
            }
        }
    }

    Timer {
        id: copiedTimer

        interval: 1600
        repeat: false
        onTriggered: copiedFeedback.visible = false
    }

    function generate() {
        copiedFeedback.visible = false
        copiedTimer.stop()
        passwordController.generatePassword(
                    root.selectedLength,
                    uppercaseOption.checked,
                    lowercaseOption.checked,
                    digitsOption.checked,
                    symbolsOption.checked)
    }

    function copyPassword() {
        if (passwordController.copyToClipboard(passwordController.password)) {
            copiedFeedback.visible = true
            copiedTimer.restart()
        }
    }
}
