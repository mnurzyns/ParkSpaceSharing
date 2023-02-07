#include <QApplication>
#include <QQmlApplicationEngine>
#include <QTableView>
#include <QStandardItemModel>
#include <qstandarditemmodel.h>

int main(int argc, char *argv[])
{
    QApplication app{argc, argv};

    QTableView view{};
    QStandardItemModel model{};
    model.setHeaderData(0, Qt::Vertical, QObject::tr("ID"));
    model.setHeaderData(1, Qt::Vertical, QObject::tr("Location"));
    model.setHeaderData(2, Qt::Vertical, QObject::tr("User ID"));

    view.setModel(&model);
    view.show();

    return QApplication::exec();
}
