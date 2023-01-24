import QtQuick 2.12
import QtQuick.Controls 2.1
import QtQuick.Window 2.12

Window {
    id: register
    width: 470
    height: 400
    title: qsTr("Register")

    Text {
        id: text1
        x: 40
        y: 56
        text: qsTr("Login:")
        font.pixelSize: 26
    }

    Text {
        id: text2
        x: 40
        y: 110
        text: qsTr("E-mail:")
        font.pixelSize: 26
    }

    Text {
        id: text3
        x: 40
        y: 168
        text: qsTr("Password:")
        font.pixelSize: 26
    }

    TextInput {
        id: textInput
        x: 184
        y: 56
        width: 208
        height: 30
        text: qsTr("..")
        font.pixelSize: 20
    }

    TextInput {
        id: textInput1
        x: 184
        y: 110
        width: 208
        height: 30
        text: qsTr("..")
        font.pixelSize: 20
    }

    TextInput {
        id: textInput2
        x: 184
        y: 168
        width: 208
        height: 30
        text: qsTr("..")
        font.pixelSize: 20
        selectedTextColor: "#ffffff"
    }

    Button {
        id: button
        x: 195
        y: 283
        text: qsTr("Register")
        scale: 2.339
    }
}
