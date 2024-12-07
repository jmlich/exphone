import QtQuick 2.0
import "../components/"
import "../components/platform"

PagePL  {

    title: qsTr("Add number")

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
        ButtonPL {
            text: qsTr("Add to blacklist")
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: {
                BlockModel.addItem(addNumber.text, addName.text, addNote.text, true);
                app.pages.pop();
            }
        }
    }

}
