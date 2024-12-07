import QtQuick 2.0
import "../components/"
import "../components/platform"

PagePL  {
    id: page

    title: (count < 0) ? qsTr("Add number") : qsTr("Update details")

    property string number: ""
    property alias name: nameTextField.text
    property alias note: noteTextField.text
    property string lastSeen
    property int count: -1
    property alias blocked: blockedSwitch.checked

    Column {
        id: column
        width: parent.width
        anchors.top: parent.top
        anchors.margins: styler.themePaddingMedium
        spacing: styler.themePaddingLarge


        TextFieldPL {
            id: newNumberTextField
            placeholderText: qsTr("Phone number")
            inputMethodHints: Qt.ImhDialableCharactersOnly
            text: page.number
        }
        TextFieldPL {
            id: nameTextField
            placeholderText: qsTr("Name")
        }
        TextFieldPL {
            id: noteTextField
            placeholderText: qsTr("Note")
        }

        LabelPL {
            id: countLabel
            text: qsTr("Count: %1").arg(page.count)
            visible: page.count >= 0
        }
        LabelPL {
            id: lastSeenLabel
            text: qsTr("Last seen: %1").arg(page.lastSeen)
            visible: page.count >= 0
        }

        TextSwitchPL {
            id: blockedSwitch
            text: qsTr("Blocked")
            checked: true
        }

        Row {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: styler.themePaddingMedium
            spacing: styler.themePaddingLarge
            ButtonPL {
                text: qsTr("Save")
                onClicked: {
                    if (page.number === "") {
                        BlockModel.addItem(newNumberTextField.text, nameTextField.text, noteTextField.text, blockedSwitch.checked);
                    } else {
                        BlockModel.updateItem(page.number, newNumberTextField.text, nameTextField.text, noteTextField.text, blockedSwitch.checked);
                    }
                    app.pages.pop();
                }
            }

            ButtonPL {
                text: qsTr("Delete")
                visible: page.number !== ""
                onClicked: {
                    BlockModel.removeItem(page.number)
                    app.pages.pop();
                }
            }
        }
    }

}
