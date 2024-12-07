import QtQuick 2.0
import "./components/"
import "./components/platform"
import "./pages/"

ApplicationWindowPL {
    id: app

    initialPage: Component { ListPage { } }

    StylerPL {
        id: styler
    }

    Timer {
        id: tmrStartup
        running: true
        repeat: false
        interval:300
        onTriggered: {
            app.pages.processCurrentItem();
        }
    }

    TruncationModes { id: truncModes }

    function tr(message) {
        return qsTr(message);
    }

    function pushAttached(pagefile, options) {
        return app.pages.pushAttached(pagefile, options);
    }

    function createObject(page, options, parent) {
        var pc = Qt.createComponent(page);
        if (pc.status === Component.Error) {
            console.log('Error while creating component');
            console.log(pc.errorString());
            return null;
        }
        return pc.createObject(parent ? parent : app, options ? options : {})
    }

}
