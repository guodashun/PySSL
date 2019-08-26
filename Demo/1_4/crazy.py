from PyQt5.QtCore import QUrl, QObject
from PyQt5.QtGui import QGuiApplication
from PyQt5.QtQml import qmlRegisterType, QQmlComponent, QQmlApplicationEngine
from interaction import Interaction

if __name__ == '__main__':
    app = QGuiApplication([])
    qmlRegisterType(Interaction, "ZSS", 1, 0, "Interaction")
    engine = QQmlApplicationEngine()
    engine.load(QUrl("./qml/main.qml"));
    app.exec_()
