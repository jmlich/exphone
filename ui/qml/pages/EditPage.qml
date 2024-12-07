import QtQuick 2.0
import "../components/"
import "../components/platform"

PagePL  {
    id: page

    title: (count < 0) ? qsTr("Add number") : qsTr("Update details")

    property alias number: addNumber.text
    property alias name: addName.text
    property alias note: addNote.text
    property string lastSeen
    property int count: -1

    Column {
        id: column
        width: parent.width
        anchors.top: parent.top
        anchors.margins: styler.themePaddingMedium
        spacing: styler.themePaddingLarge


        TextFieldPL {
            id: addNumber
            placeholderText: qsTr("Number")
            inputMethodHints: Qt.ImhDialableCharactersOnly
        }
        TextFieldPL {
            id: addName
            placeholderText: qsTr("Contact name")
        }
        TextFieldPL {
            id: addNote
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
            text: qsTr("Blocked")
        }

        ButtonPL {
            text: qsTr("Save")
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: {
                BlockModel.addItem(addNumber.text, addName.text, addNote.text, true);
                app.pages.pop();
            }
        }
    }

}
