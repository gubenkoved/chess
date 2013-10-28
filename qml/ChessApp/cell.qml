// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
    property int xPos;
    property int yPos;
    property string cellText: '?';
    Text {
        text: parent.cellText;
        anchors.centerIn: parent;
        font.pixelSize: Math.min(parent.height, parent.width);
    }
}
