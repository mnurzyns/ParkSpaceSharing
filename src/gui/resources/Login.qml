import QtQuick 2.12
import QtQuick.Controls 2.1
import QtQuick.Window 2.12

Window {
    id: login
    width: 470
    height: 320
    title: qsTr("Login")

    Text {
        id: text1
        x: 32
        y: 66
        text: qsTr("Login or e-mail:")
        font.pixelSize: 26
    }

    Text {
        id: text2
        x: 32
        y: 138
        text: qsTr("Password:")
        font.pixelSize: 26
    }

    Button {
        id: button
        x: 195
        y: 227
        text: qsTr("Login")
        scale: 2.339
    }

    TextInput {
        id: textInput
        x: 248
        y: 66
        width: 171
        height: 30
        text: qsTr("..")
        font.pixelSize: 20
    }

    TextInput {
        id: textInput1
        x: 248
        y: 138
        width: 171
        height: 30
        text: qsTr("..")
        font.pixelSize: 20
    }
}
