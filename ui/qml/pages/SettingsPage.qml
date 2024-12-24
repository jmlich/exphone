import QtQuick 2.0
import "../components/"
import "../components/platform"
import com.github.jmlich.exphone 1.0

PagePL  {
    id: page
    title: qsTr("Settings")

    Column {
        id: column
        width: parent.width
        anchors.top: parent.top
        anchors.margins: styler.themePaddingMedium
        spacing: styler.themePaddingLarge

        TextSwitchPL {
            id: unknownSwitch
            text: qsTr("Block unknown numbers")
        }

        TextSwitchPL {
            id: anonymousSwitch
            text: qsTr("Block anonymous numbers")
        }


        ButtonPL {
            text: qsTr("Save")
            visible: page.number !== ""
            onClicked: {
                saveSettings();
                app.pages.pop();
            }
        }

    }

    function saveSettings() {
        ExphoneConfig.blockingUnknown = unknownSwitch.checked;
        ExphoneConfig.blockingCLIR = anonymousSwitch.checked;
    }

    Component.onCompleted: {
        unknownSwitch.checked = ExphoneConfig.blockingUnknown;
        anonymousSwitch.checked = ExphoneConfig.blockingCLIR;
    }

}
