import QtQuick 2.0
import "../components/"
import "../components/platform"

PageListPL  {
    id: page
    title: qsTr("Blocked numbers")

    pageMenu: PageMenuPL {
        PageMenuItemPL {
            text: qsTr("Add number")
            iconSource: styler.iconAdd
            onClicked: {
                app.pages.push(Qt.resolvedUrl("EditPage.qml"), {})
            }
        }

        PageMenuItemPL {
            text: qsTr("Settings")
            iconSource: styler.iconSettings
            onClicked: {
                app.pages.push(Qt.resolvedUrl("SettingsPage.qml"), {})
            }
        }
    }

    model: BlockModel
    placeholderEnabled: BlockModel.count === 0
    placeholderText: qsTr("No records, make phone call or add number manually")

    delegate: ListItemPL {
        contentHeight: details.height
        property bool isHiddenNumber: (model.number === "x-ofono-unknown")

        onClicked: {
            if (isHiddenNumber) {
                app.pages.push(Qt.resolvedUrl("SettingsPage.qml"), {})
                return;
            }

            var item = BlockModel.get(index)
            console.log(JSON.stringify(item));
            app.pages.push(Qt.resolvedUrl("EditPage.qml"), item)
        }

        Column {
            id: details
            Item { // spacer
                width: 1
                height: styler.themePaddingSmall
            }
            LabelPL {
                text: isHiddenNumber ? qsTr("Anonymous caller") : model.note
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
                visible: !isHiddenNumber && (text !== "") && (model.number !== model.name)
            }
            LabelPL {
                text: getFormattedTime(model.lastSeen) + " • " + model.count
            }
            LabelPL {
                visible: !isHiddenNumber
                text:  model.blocked ? qsTr("Blocked") : qsTr("Not blocked")
                color: model.blocked ? "#b50000" : "#009100"
                font.bold: true
                font.pixelSize: styler.themeFontSizeSmall
            }

            Item { // spacer
                width: 1
                height: styler.themePaddingSmall
            }

        }
    }

    function getFormattedTime(dateTime) {
        var input = new Date(dateTime)
        var diffInSeconds = (new Date() - input) / 1000;

        if (diffInSeconds > 86400) { // More than a day
            return input.toLocaleDateString(Qt.locale().name);
        }
        return Qt.formatDateTime(dateTime, "hh:mm");
    }


}

