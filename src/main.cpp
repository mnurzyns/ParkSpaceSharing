#include <QApplication>
#include <QHostAddress>
#include <QQmlApplicationEngine>
#include <QStandardItemModel>
#include <QTableView>
#include <QTcpSocket>

#include <nlohmann/json.hpp>

#include <iostream>

static
constexpr
::std::uint16_t PORT{55555};

static
constexpr
qint64 MAX_BUFFER_SIZE{1024};

int main(int argc, char *argv[])
{
    QApplication app{argc, argv};

    QTcpSocket socket{};
    socket.connectToHost(QHostAddress("127.0.0.1"), PORT);
    if(!socket.waitForConnected()) {
        std::cerr << "not conn\n";
        return 1;
    }
    if(socket.write("parking_space/get") == -1) {
        std::cerr << "err\n";
        return 1;
    }
    if(!socket.waitForReadyRead()) {
        std::cerr << "cannot read";
        return 1;
    }

    std::array<char, MAX_BUFFER_SIZE> data{};
    socket.read(data.data(), MAX_BUFFER_SIZE);
    std::cout << data.data() << '\n';
    auto json = nlohmann::json::parse(data.data());
    std::cout << json.dump(4) << '\n';

    QTableView table_view{};
    QStandardItemModel model{3, 3};
    for(std::size_t i=0;i<json["id"].size();++i) {
        model.setItem(static_cast<int>(i), 0, new QStandardItem{QString::number(json["id"][i].get<int>())});
        model.setItem(static_cast<int>(i), 1, new QStandardItem{QString::fromStdString(json["location"][i].get<std::string>())});
        model.setItem(static_cast<int>(i), 2, new QStandardItem{QString::number(json["user_id"][i].get<int>())});
    }

    table_view.setModel(&model);
    table_view.show();

    return QApplication::exec();
}
