#include <iostream>
#include <QObject>
#include <QGraphicsObject>
#include <QString>
#include <QArgument>

#include <QApplication>
#include <QtDeclarative/QDeclarativeContext>
#include <QMessageBox>

#include "qmlapplicationviewer.h"
#include "figureposition.h"
#include "tests.h"
#include "board.h"
#include "chessapp.h"
#include "exception.h"

//#define TESTS

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    try {

#ifdef TESTS
        Tests tests;
        tests.TestAll();

        return 0;
#endif

        QScopedPointer<QApplication> application(createApplication(argc, argv));

        ChessApp app;

        QmlApplicationViewer viewer;
        viewer.rootContext()->setContextProperty("app", &app);
        viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
        viewer.setMainQmlFile(QLatin1String("qml/ChessApp/main.qml"));
        viewer.showExpanded();

        QObject* root = dynamic_cast<QObject*>( viewer.rootObject());
        QObject* boardControl = root->findChild<QObject*>("board");
        QMetaObject::invokeMethod(boardControl, "createField");

        app.SetBoardControl(boardControl);
        app.Draw();

        return application->exec();
    } catch (Exception e) {
        //qDebug() << "Unhandled exception:" << e.GetMessage();
    }
    catch (...) {
        qDebug() << "Unhandled exception!";
    }
}
