// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
//import QtQuick.Controls 1.0

Rectangle {
    id: rootRect
    width: 800
    height: 671
    //id: root
    //objectName: 'root'

    Rectangle
    {
        id: board
        objectName: 'board'

        property real cellWidth: width / 8.0
        property real cellHeight: height / 8.0

        property bool _figureSelected: false;
        property int _selectedX;
        property int _selectedY;
        width: 600
        height: 600
        border.width: 2
        border.color: "#000000"
        anchors.top: menu.bottom
        anchors.topMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 5

        function showNotification(message)
        {
            notificationMessage.text = message;
            notification.visible = true;
        }

        function updateField()
        {
            for (var i = 0; i < cellsContainer.children.length; i++)
            {
                var child = cellsContainer.children[i];

                child.width = cellWidth;
                child.height = cellHeight;
                child.x = (child.xPos - 1) * board.cellWidth;
                child.y = (8 - child.yPos) * board.cellHeight;
                child.color = (((child.xPos + child.yPos) % 2) === 0) ? "#BBBBBB" : "white";
                //child.cellText = '2';
                //child.;
                //console.log(child);

                fenText.text = app.GetFEN();
                moveList.text = app.BoardToString();
            }
        }

        function setCellColor(x, y, color)
        {
            for (var i = 0; i < cellsContainer.children.length; i++)
            {
                var child = cellsContainer.children[i];

                if (child.xPos === x && child.yPos === y)
                {
                    child.color = color;
                }
            }
        }

        function setCellText(x, y, txt)
        {
            for (var i = 0; i < cellsContainer.children.length; i++)
            {
                var child = cellsContainer.children[i];

                if (child.xPos === x && child.yPos === y)
                {
                    child.cellText = txt;
                }
            }
        }

        function createField()
        {
            for (var x = 1; x <= 8; x++)
            {
                for (var y = 1; y <= 8; ++y)
                {
                    var isBlack = (((x + y) % 2) === 0);

                    var newCell = createCell();

                    newCell.xPos = x;
                    newCell.yPos = y;
                }
            }

            updateField();
        }

        function createCell()
        {
            var cellComponent = Qt.createComponent("cell.qml");
            return cellComponent.createObject(cellsContainer);
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                var child = cellsContainer.childAt(mouse.x, mouse.y);
                var x = child.xPos;
                var y = child.yPos;

                board.updateField();

                if (board._figureSelected && app.IsPossibleStep(board._selectedX, board._selectedY, x, y))
                {
                    board._figureSelected = false;

                    app.Step(board._selectedX, board._selectedY, x, y);
                    app.Draw();
                } else
                {
                    if (app.IsFigureCell(x, y))
                    {
                        board._figureSelected = true;
                        board._selectedX = x;
                        board._selectedY = y;

                        board.setCellColor(x, y, '#5D8FB6');

                        var possible = app.GetPossible(board._selectedX, board._selectedY);

                        for (var i = 0; i < possible.length / 2; i++)
                        {
                            var tX = parseInt(possible[2 * i])
                            var tY = parseInt(possible[2 * i + 1]);

                            if ((tX + tY) % 2 === 0)
                                board.setCellColor(tX, tY, '#417643');
                            else
                                board.setCellColor(tX, tY, '#5DB65F');
                        }
                    } else
                    {
                        board._figureSelected = false;
                    }
                }
            }
        }

        Rectangle
        {
            id: notification
            x: 200
            y: 300
            width: 200
            height: 50
            color: "#136ab5"
            radius: 0
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            z: 1
            opacity: 0.6
            border.width: 1
            border.color: "#000000"
            visible: false;

            Text
            {
                id: notificationMessage
                color: "#ffffff"
                text: "Notification";
                font.bold: false
                font.family: "Segoe UI"
                font.pointSize: 14
                anchors.verticalCenterOffset: 0
                anchors.horizontalCenterOffset: 0
                anchors.centerIn: parent;
            }

            Text
            {
                color: "#ffffff"
                text: "click to close";
                font.family: "Segoe UI"
                anchors.right: parent.right
                anchors.rightMargin: 2
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 2
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: parent.opacity = 1
                onExited: parent.opacity = 0.6
                onClicked: parent.visible = false;
            }
        }

        Rectangle
        {
            id: cellsContainer
            anchors.fill: parent
            objectName: 'cellsContainer'
        }

        onWidthChanged: updateField();
        onHeightChanged: updateField();


    }

    Rectangle {
        id: menu
        height: 40
        color: "#00000000"
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0

        Flow {
            id: flow
            anchors.rightMargin: 5
            anchors.leftMargin: 5
            anchors.bottomMargin: 5
            anchors.topMargin: 5
            anchors.fill: parent

            Rectangle {
                id: backButton
                width: 100
                color: "white"
                radius: 0
                anchors.top: parent.top
                anchors.topMargin: 0
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                border.width: 2
                border.color: "#959595"

                Text {
                    x: 52
                    y: 36
                    text: qsTr("Turn back")
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 12
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: parent.color = "orange"
                    onExited: parent.color = "white"
                    onClicked: {
                        app.TurnBack();
                        app.Draw();
                    }
                }
            }

            Rectangle {
                id: startComputerMoveButton
                width: 140
                color: "white"
                radius: 0
                anchors.top: parent.top
                anchors.topMargin: 0
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                border.width: 2
                border.color: "#959595"

                Text {
                    x: 52
                    y: 36
                    text: qsTr("Step by computer")
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 12
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: parent.color = "orange"
                    onExited: parent.color = "white"
                    onClicked: {
                        app.StartStepByComputer();
                    }
                }
            }
        }
    }

    TextInput {
        id: fenText
        text: "fen"
        font.family: "Segoe UI"
        cursorVisible: true
        font.pointSize: 11
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
    }

    TextEdit {
        id: moveList
        text: "move list move list move list move list move list move list move list move list move list move list move list move list move list move list move list"
        font.pointSize: 10
        font.family: "Segoe UI"
        textFormat: TextEdit.PlainText
        wrapMode: TextEdit.WordWrap
        verticalAlignment: Text.AlignTop
        anchors.left: board.right
        anchors.leftMargin: 5
        anchors.bottom: loadButton.top
        anchors.bottomMargin: 5
        anchors.top: menu.bottom
        anchors.topMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 5
    }

    Rectangle {
        id: loadButton
        height: 30
        color: "white"
        radius: 0
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.left: board.right
        anchors.leftMargin: 5
        anchors.bottom: fenText.top
        anchors.bottomMargin: 5
        border.width: 2
        border.color: "#959595"

        Text {
            x: 52
            y: 36
            text: qsTr("Load")
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 12
        }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onEntered: parent.color = "orange"
            onExited: parent.color = "white"
            onClicked: {
                //app.TurnBack();
                app.BoardFromString(moveList.text);
                app.Draw();
            }
        }
    }
}
