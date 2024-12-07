import QtQuick 2.0
import "../components/"
import "../components/platform"

PageListPL  {
    id: page
    title: qsTr("Blocked numbers")

    model: BlockModel
    placeholderEnabled: BlockModel.rowCount() === 0
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
            ContextMenuItemPL {
                text: "block/unblock"
                onClicked: {
                    BlockModel.setBlocked(model.number, !model.blocked)
                }
            }
        }

        Column {
            id: details
            LabelPL {
                text: model.note
                font.pixelSize: styler.themeFontSizeLarge
            }
            LabelPL {
                text: model.number
                font.pixelSize: styler.themeFontSizeMedium
            }
            LabelPL {
                text: model.name
                font.pixelSize: styler.themeFontSizeMedium
            }
            LabelPL {
                text: model.lastSeen
            }
            LabelPL {
                text: model.blocked ? qsTr("Blocked") : qsTr("Not blocked")
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

    // ListView {
    //     anchors.fill: parent
    //     delegate: Row {
    //         spacing: 10
    //         Button {
    //             text: model.blocked ? "unblock" : "block"
    //             onClicked: {
    //                 BlockModel.setBlocked(model.number, !model.blocked)
    //             }
    //         }
    //         Button {
    //             text: "remove"
    //             onClicked: {
    //                 BlockModel.removeItem(model.number)
    //             }
    //         }
    //         Rectangle {
    //             width: 10;
    //             height: 10
    //             color: model.blocked ? "red" : "green"
    //         }
    //         Label {
    //             text: model.number
    //         }
    //         Label {
    //             text: model.name
    //         }
    //         Label {
    //             text: model.lastSeen
    //         }
    //     }
    // }

}

