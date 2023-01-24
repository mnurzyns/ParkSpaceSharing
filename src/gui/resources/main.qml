import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.1

Window {
    id: register
    width: 640
    height: 480
    visible: true
    color: "#ffffff"
    title: qsTr("ParkSpaceSharing")

    Button {
        id: button
        x: 474
        y: 0
        text: qsTr("Register")
        onClicked: {
            var component = Qt.createComponent("Register.qml")
            var window = component.createObject(register)
            window.show()
        }
    }

    Button {
        id: login
        x: 560
        y: 0
        text: qsTr("Login")
        onClicked: {
            var component1 = Qt.createComponent("Login.qml")
            var window1 = component1.createObject(login)
            window1.show()
        }
    }
}
