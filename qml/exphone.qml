import QtQuick 2.9
import QtQuick.Controls 2.2


ApplicationWindow {
    id: appWindow
    width: 640
    height: 480
    visible: true


    ListView {
        anchors.fill: parent
        model: BlockModel
        delegate: Row {
            spacing: 10
            Button {
                text: model.blocked ? "unblock" : "block"
                onClicked: {
                    BlockModel.setBlocked(model.number, !model.blocked)
                }
            }
            Button {
                text: "remove"
                onClicked: {
                    BlockModel.removeItem(model.number)
                }
            }
            Rectangle {
                width: 10;
                height: 10
                color: model.blocked ? "red" : "green"
            }
            Label {
                text: model.number
            }
            Label {
                text: model.name
            }
            Label {
                text: model.lastSeen
            }
        }
    }


    Column {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        TextField {
            id: addNumber
            placeholderText: "number"
        }
        TextField {
            id: addName
            placeholderText: "name"
        }
        TextField {
            id: addNote
            placeholderText: "note"
        }
        Button {
            text: "Add"
            onClicked: {
                BlockModel.addItem(addNumber.text, addName.text, addNote.text, true);
            }
        }
    }
}
