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
            placeholderText: "number"
            inputMethodHints: Qt.ImhDialableCharactersOnly
        }
        TextFieldPL {
            id: addName
            placeholderText: "name"
        }
        TextFieldPL {
            id: addNote
            placeholderText: "note"
        }
        ButtonPL {
            text: "Add"
            onClicked: {
                BlockModel.addItem(addNumber.text, addName.text, addNote.text, true);
                app.pages.pop();
            }
        }
    }

}
