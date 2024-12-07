import QtQuick 2.0
import "../components/"
import "../components/platform"

PageListPL  {
    id: page
    title: qsTr("Blocked numbers")


    model: BlockModel
    placeholderEnabled: BlockModel.count === 0
    placeholderText: qsTr("No records, make phone call or add number manually")
    delegate: ListItemPL {
        contentHeight: details.height
        menu: ContextMenuPL {
            ContextMenuItemPL {
                text: "delete"
                onClicked: {
                    BlockModel.removeItem(model.number)
                }
            }
        }

        onClicked: {
            BlockModel.setBlocked(model.number, !model.blocked)
        }

        Column {
            id: details
            LabelPL {
                text: model.note
                font.pixelSize: styler.themeFontSizeLarge
                visible: text !== ""
            }
            LabelPL {
                text: model.number
                font.pixelSize: styler.themeFontSizeMedium
            }
            LabelPL {
                text: model.name
                font.pixelSize: styler.themeFontSizeMedium
                visible: text !== ""
            }
            LabelPL {
                text: model.lastSeen + " (" + model.count + ")"
            }
            LabelPL {
                text: model.blocked ? qsTr("Blocked") : qsTr("Not blocked")
                color: model.blocked ? "#b50000" : "#009100"
                font.bold: true
                font.pixelSize: styler.themeFontSizeSmall
            }

            //         Label {
            //             text: model.lastSeen
            //         }
        }
    }

    pageMenu: PageMenuPL {
        PageMenuItemPL {
            text: qsTr("Add number")
            iconSource: styler.iconEdit
            onClicked: {
                app.pages.push(Qt.resolvedUrl("EditPage.qml"))
            }
        }
    }

}

