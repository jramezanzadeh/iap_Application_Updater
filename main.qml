import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.0
import Serial 1.0

Window {
    id: window
    width: 800
    height: 480
    visible: true
    title: qsTr("AIS Application Updater")

    UartAppProgrammer{
        id:appProgrammer
        onStatusChanged: {
            switch(status){
            case UartAppProgrammer.StartedProgramming:
                console.debug("programming started")
                statusText.color = "black"
                statusText.text = "start progaraming"
                progressBar.visible = true
                progressBar.value = 0
                programButton.enabled = false
                break
            case UartAppProgrammer.Programming:
                console.debug("programming %" +
                              appProgrammer.getProgrammedBlocksPercentage())
                statusText.color = "black"
                statusText.text = "progaraming"
                progressBar.value = appProgrammer.getProgrammedBlocksPercentage()
                break
            case UartAppProgrammer.DoneWithError:
                console.log("done with error:" + appProgrammer.errorMessage)
                programButton.enabled = true
                progressBar.visible = false
                statusText.color = "red"
                statusText.text = "Error:" + appProgrammer.errorMessage
                break
            case UartAppProgrammer.DoneSuccessfully:
                console.log("done successfully");
                programButton.enabled = true
                progressBar.visible = false
                statusText.color = "green"
                statusText.text = "Programming is successfully done"
            }
        }
    }

    GroupBox {
        id: portSettinggroupBox
        x: 28
        y: 73
        width: 299
        height: 183
        wheelEnabled: false
        layer.textureSize.width: 1
        clip: false
        layer.enabled: false
        layer.smooth: false
        font.bold: true
        font.pointSize: 16
        title: qsTr("Port Setting:")

        Label {
            id: portNameLabel
            y: 51
            text: qsTr("Uart Port:")
            anchors.left: parent.left
            anchors.bottom: baudRateLabel.top
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.bottomMargin: 40
            anchors.leftMargin: 0
            font.bold: true
            font.pointSize: 15

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    appProgrammer.updateAvailablePortsList()
                    portNameComboBox.currentIndex = 0
                }
            }
        }

        ComboBox {
            id: portNameComboBox
            y: 15
            height: 40
            anchors.verticalCenter: portNameLabel.verticalCenter
            anchors.left: portNameLabel.right
            anchors.right: parent.right
            anchors.leftMargin: 10
            anchors.verticalCenterOffset: 0
            anchors.rightMargin: 0
            model: appProgrammer
            indicator: Canvas {
                id: canvasPortName
                x: portNameComboBox.width - width - portNameComboBox.rightPadding
                y: portNameComboBox.topPadding + (portNameComboBox.availableHeight - height) / 2
                width: 12
                height: 8
                contextType: "2d"

                Connections {
                    target: portNameComboBox
                    function onPressedChanged() { canvasPortName.requestPaint(); }
                }

                onPaint: {
                    context.reset();
                    context.moveTo(0, 0);
                    context.lineTo(width, 0);
                    context.lineTo(width / 2, height);
                    context.closePath();
                    context.fillStyle = portNameComboBox.pressed ? "#17a81a" : "#21be2b";
                    context.fill();
                }
            }
        }

        Label {
            id: baudRateLabel
            y: 114
            text: qsTr("BaudRate:")
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.bottomMargin: 22
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: portNameLabel.horizontalCenter
            anchors.leftMargin: 0
            font.pointSize: 15
            font.bold: true
        }

        ComboBox {
            id: baudRateComboBox
            y: 85
            height: 40
            anchors.verticalCenter: baudRateLabel.verticalCenter
            anchors.left: baudRateLabel.right
            anchors.right: parent.right
            anchors.leftMargin: 10
            anchors.verticalCenterOffset: 0
            anchors.rightMargin: 0
            model: ["4800", "9600", "19200", "34800", "115200"]
            indicator: Canvas {
                id: canvasBaudRate
                x: baudRateComboBox.width - width - baudRateComboBox.rightPadding
                y: baudRateComboBox.topPadding + (baudRateComboBox.availableHeight - height) / 2
                width: 12
                height: 8
                contextType: "2d"

                Connections {
                    target: baudRateComboBox
                    function onPressedChanged() { canvasBaudRate.requestPaint(); }
                }

                onPaint: {
                    context.reset();
                    context.moveTo(0, 0);
                    context.lineTo(width, 0);
                    context.lineTo(width / 2, height);
                    context.closePath();
                    context.fillStyle = baudRateComboBox.pressed ? "#17a81a" : "#21be2b";
                    context.fill();
                }
            }
        }
    }

    FileDialog {
        id: fileDialog
        width: window.width
        height: window.width
        title: "Please choose a file"

        folder: "file:///home/eagle/workspace/signApp"
        nameFilters: ["Binary files(*.bin)"]
        onAccepted: {
            console.log("You chose: " + fileDialog.fileUrl)
            console.log("fileName: " + appProgrammer.urlToFileName(fileDialog.fileUrl.toString()))
            var index;
            for (index = 0; index < fileNameComboBoxModel.count; index++) {
                if(fileNameComboBoxModel.get(index).modelData === fileDialog.fileUrl.toString())
                    break;
            }
            if(index === fileNameComboBoxModel.count)
                fileNameComboBoxModel.append({modelData:fileDialog.fileUrl.toString()})

            fileNameComboBox.currentIndex = index
            console.log("selectedFilecomboBox.currentIndex = " + fileNameComboBox.currentIndex)
            console.log("selectedFilecomboBox.count = " + fileNameComboBox.count)
        }
        onRejected: {
            console.log("Canceled")
        }
    }

    GroupBox {
        id: fileSettingGroupBox
        x: 349
        y: 73
        width: 404
        height: portSettinggroupBox.height
        font.bold: true
        font.pointSize: 16
        title: qsTr("File Selection:")

        Label {
            id: fileNameLabel
            y: 51
            text: qsTr("App File :")
            anchors.left: parent.left
            anchors.bottom: selectFileButton.top
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.bottomMargin: 31
            anchors.leftMargin: 0
            font.pointSize: 15
            font.bold: true
        }

        ComboBox {
            id: fileNameComboBox
            y: 15
            height: 40
            anchors.verticalCenter: fileNameLabel.verticalCenter
            anchors.left: fileNameLabel.right
            anchors.right: parent.right
            anchors.leftMargin: 10
            anchors.verticalCenterOffset: 0
            anchors.rightMargin: 0

            model:ListModel{
                id: fileNameComboBoxModel
            }

            delegate:ItemDelegate {
                width: fileNameComboBox.width
                contentItem: Text {
                    text: appProgrammer.urlToFileName(modelData)
                    font: fileNameComboBox.font
                    elide: Text.ElideRight
                    verticalAlignment: Text.AlignVCenter
                }
                highlighted: fileNameComboBox.highlightedIndex === index
            }

            contentItem: Text {
                leftPadding: 0
                rightPadding: fileNameComboBox.indicator.width + fileNameComboBox.spacing

                text: appProgrammer.urlToFileName(fileNameComboBox.currentText)
                font: fileNameComboBox.font
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            indicator: Canvas {
                id: canvasFileName
                x: fileNameComboBox.width - width - fileNameComboBox.rightPadding
                y: fileNameComboBox.topPadding + (fileNameComboBox.availableHeight - height) / 2
                width: 12
                height: 8
                contextType: "2d"

                Connections {
                    target: fileNameComboBox
                    function onPressedChanged() { canvasFileName.requestPaint(); }
                }

                onPaint: {
                    context.reset();
                    context.moveTo(0, 0);
                    context.lineTo(width, 0);
                    context.lineTo(width / 2, height);
                    context.closePath();
                    context.fillStyle = fileNameComboBox.pressed ? "#17a81a" : "#21be2b";
                    context.fill();
                }
            }
        }

        RoundButton {
            id: selectFileButton
            x: 150
            y: 105
            radius: 8
            text: qsTr("Browse File")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 14
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: fileNameComboBox.horizontalCenter
            onClicked: fileDialog.visible = true;
        }
    }

    ProgressBar {
        id: progressBar
        x: 222
        //        width: 309
        //        height: 6
        // padding: 2
        value: 0
        visible: false

        anchors.top: statusText.bottom
        anchors.horizontalCenterOffset: 105
        anchors.topMargin: 20
        font.pointSize: 12
        anchors.horizontalCenter: programButton.horizontalCenter

        background: Rectangle {
            implicitWidth: 309
            implicitHeight: 10
            color: "#e6e6e6"
            radius: 3
        }
        contentItem: Item {
            implicitWidth: 309
            implicitHeight: 8

            Rectangle {
                width: progressBar.visualPosition * parent.width
                height: progressBar.height
                radius: 2
                color: "#17a81a"
            }
            Text {
                id: percentageText
                anchors.centerIn: parent
                text: qsTr("%"+ Math.round(progressBar.value*100))
            }
        }
    }

    RoundButton {
        id: programButton
        x: 222
        y: 285
        radius: 8
        text: qsTr("Program")
        anchors.verticalCenter: statusText.verticalCenter
        anchors.right: statusText.left
        font.pointSize: 16
        font.bold: true
        anchors.rightMargin: 30
        onClicked: appProgrammer.startProgramming(fileNameComboBox.currentText,
                                                  portNameComboBox.currentText,
                                                  parseInt(baudRateComboBox.currentText))
    }

    Text {
        id: statusText
        x: 365
        width: 381
        height: 35
        text: qsTr("")
        anchors.right: fileSettingGroupBox.right
        anchors.top: fileSettingGroupBox.bottom
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 14
        font.bold: false
        leftPadding: 10
        anchors.topMargin: 34
        anchors.rightMargin: 0
    }
}
