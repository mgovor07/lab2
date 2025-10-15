#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <limits>
#include <filesystem>

using namespace std;
namespace fs = filesystem;

struct Pipe {
    string name;
    double length;
    int diameter;
    bool underRepair;
};

struct CompressorStation {
    string name;
    int totalWorkshops;
    int activeWorkshops;
    int stationClass;
};

class PipelineSystem {
private:
    vector<Pipe> pipes;
    vector<CompressorStation> stations;

    // Функция для безопасного ввода целых чисел с проверкой диапазона
    int getIntInput(const string& prompt, int min = numeric_limits<int>::min(),
                   int max = numeric_limits<int>::max()) {
        int value;
        while (true) {
            cout << prompt;
            string input;
            getline(cin, input);
            
            if (input.empty()) {
                cout << "Ошибка: ввод не может быть пустым.\n";
                continue;
            }
            
            try {
                value = stoi(input);
                if (value < min || value > max) {
                    cout << "Ошибка: значение должно быть от " << min << " до " << max << ".\n";
                    continue;
                }
                break;
            } catch (const exception&) {
                cout << "Ошибка: пожалуйста, введите целое число.\n";
            }
        }
        return value;
    }

    // Функция для безопасного ввода чисел с плавающей точкой с проверкой диапазона
    double getDoubleInput(const string& prompt, double min = 0.0,
                         double max = numeric_limits<double>::max()) {
        double value;
        while (true) {
            cout << prompt;
            string input;
            getline(cin, input);
            
            if (input.empty()) {
                cout << "Ошибка: ввод не может быть пустым.\n";
                continue;
            }
            
            try {
                value = stod(input);
                if (value < min || value > max) {
                    cout << "Ошибка: значение должно быть от " << min << " до " << max << ".\n";
                    continue;
                }
                break;
            } catch (const exception&) {
                cout << "Ошибка: пожалуйста, введите число.\n";
            }
        }
        return value;
    }

    // Функция для безопасного ввода строки (не константная, так как изменяет состояние ввода)
    string getStringInput(const string& prompt) {
        string input;
        while (true) {
            cout << prompt;
            getline(cin, input);
            if (!input.empty()) {
                break;
            }
            cout << "Ошибка: ввод не может быть пустым.\n";
        }
        return input;
    }

    // Константная версия для использования в константных методах (просто запрашивает ввод)
    string getStringInputConst(const string& prompt) const {
        string input;
        while (true) {
            cout << prompt;
            getline(cin, input);
            if (!input.empty()) {
                break;
            }
            cout << "Ошибка: ввод не может быть пустым.\n";
        }
        return input;
    }

public:
    void addPipe() {
        Pipe newPipe;
        newPipe.name = getStringInput("Введите название трубы: ");
        newPipe.length = getDoubleInput("Введите длину трубы (км): ", 0.001);
        newPipe.diameter = getIntInput("Введите диаметр трубы (мм): ", 1);
        newPipe.underRepair = false;
        
        pipes.push_back(newPipe);
        cout << "Труба добавлена!\n";
    }

    void addStation() {
        CompressorStation newStation;
        newStation.name = getStringInput("Введите название КС: ");
        newStation.totalWorkshops = getIntInput("Введите количество цехов: ", 1);
        
        // Проверка, что работающих цехов не больше общего количества
        newStation.activeWorkshops = getIntInput("Введите количество работающих цехов: ",
                                               0, newStation.totalWorkshops);
        
        newStation.stationClass = getIntInput("Введите класс станции: ", 1);
        stations.push_back(newStation);
        cout << "КС добавлена!\n";
    }

    void viewAll() const {
        if (pipes.empty() && stations.empty()) {
            cout << "Нет объектов для отображения.\n";
            return;
        }
        
        if (!pipes.empty()) {
            cout << "\n=== Трубы ===\n";
            for (size_t i = 0; i < pipes.size(); ++i) {
                cout << i << ". Название: " << pipes[i].name
                     << ", Длина: " << pipes[i].length
                     << " км, Диаметр: " << pipes[i].diameter
                     << " мм, В ремонте: " << (pipes[i].underRepair ? "Да" : "Нет") << endl;
            }
        }

        if (!stations.empty()) {
            cout << "\n=== Компрессорные станции ===\n";
            for (size_t i = 0; i < stations.size(); ++i) {
                cout << i << ". Название: " << stations[i].name
                     << ", Цехов: " << stations[i].totalWorkshops
                     << ", Работает: " << stations[i].activeWorkshops
                     << ", Класс: " << stations[i].stationClass << endl;
            }
        }
    }

    void editPipe() {
        if (pipes.empty()) {
            cout << "Нет доступных труб!\n";
            return;
        }
        
        viewAll();
        int index = getIntInput("Введите номер трубы для редактирования: ", 0, pipes.size()-1);
        
        cout << "1. Изменить статус ремонта\n2. Редактировать параметры\n";
        int choice = getIntInput("Выберите действие: ", 1, 2);
        
        if (choice == 1) {
            pipes[index].underRepair = !pipes[index].underRepair;
            cout << "Статус ремонта изменен на: "
                 << (pipes[index].underRepair ? "В ремонте" : "Работает") << endl;
        } else {
            pipes[index].name = getStringInput("Введите новое название трубы: ");
            pipes[index].length = getDoubleInput("Введите новую длину трубы (км): ", 0.001);
            pipes[index].diameter = getIntInput("Введите новый диаметр трубы (мм): ", 1);
            cout << "Параметры трубы обновлены!\n";
        }
    }

    void editStation() {
        if (stations.empty()) {
            cout << "Нет доступных КС!\n";
            return;
        }
        
        viewAll();
        int index = getIntInput("Введите номер КС для редактирования: ", 0, stations.size()-1);
        
        cout << "1. Запустить/остановить цех\n2. Редактировать параметры\n";
        int choice = getIntInput("Выберите действие: ", 1, 2);
        
        if (choice == 1) {
            cout << "1. Запустить цех\n2. Остановить цех\n";
            int action = getIntInput("Выберите действие: ", 1, 2);
            
            if (action == 1 && stations[index].activeWorkshops < stations[index].totalWorkshops) {
                stations[index].activeWorkshops++;
                cout << "Цех запущен! Работает цехов: " << stations[index].activeWorkshops << endl;
            } else if (action == 2 && stations[index].activeWorkshops > 0) {
                stations[index].activeWorkshops--;
                cout << "Цех остановлен! Работает цехов: " << stations[index].activeWorkshops << endl;
            } else {
                cout << "Невозможно выполнить операцию!\n";
            }
        } else {
            stations[index].name = getStringInput("Введите новое название КС: ");
            int newTotal = getIntInput("Введите новое количество цехов: ", 1);
            
            // Корректировка работающих цехов, если новое общее количество меньше текущих работающих
            if (newTotal < stations[index].activeWorkshops) {
                stations[index].activeWorkshops = newTotal;
            }
            stations[index].totalWorkshops = newTotal;
            
            stations[index].stationClass = getIntInput("Введите новый класс станции: ", 1);
            cout << "Параметры КС обновлены!\n";
        }
    }

    void saveData() const {
        string filename = getStringInputConst("Введите имя файла для сохранения: ");
        if (filename.find('.') == string::npos) {
            filename += ".txt";
        }
        
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Ошибка: невозможно создать файл " << filename << endl;
            return;
        }
        
        // Сохранение труб
        file << "PIPES " << pipes.size() << endl;
        for (const auto& pipe : pipes) {
            file << pipe.name << endl
                 << pipe.length << endl
                 << pipe.diameter << endl
                 << pipe.underRepair << endl;
        }
        
        // Сохранение станций
        file << "STATIONS " << stations.size() << endl;
        for (const auto& station : stations) {
            file << station.name << endl
                 << station.totalWorkshops << endl
                 << station.activeWorkshops << endl
                 << station.stationClass << endl;
        }
        
        file.close();
        cout << "Данные сохранены в файл: " << fs::absolute(filename) << endl;
    }

    void loadData() {
        string filename = getStringInput("Введите имя файла для загрузки: ");
        
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Ошибка: файл " << filename << " не найден.\n";
            return;
        }
        
        pipes.clear();
        stations.clear();
        
        string header;
        size_t count;
        
        // Загрузка труб
        file >> header >> count;
        if (header != "PIPES") {
            cout << "Ошибка: неверный формат файла.\n";
            return;
        }
        file.ignore();
        
        for (size_t i = 0; i < count; ++i) {
            Pipe pipe;
            getline(file, pipe.name);
            file >> pipe.length >> pipe.diameter >> pipe.underRepair;
            file.ignore();
            pipes.push_back(pipe);
        }
        
        // Загрузка станций
        file >> header >> count;
        if (header != "STATIONS") {
            cout << "Ошибка: неверный формат файла.\n";
            return;
        }
        file.ignore();
        
        for (size_t i = 0; i < count; ++i) {
            CompressorStation station;
            getline(file, station.name);
            file >> station.totalWorkshops >> station.activeWorkshops >> station.stationClass;
            file.ignore();
            
            // Проверка корректности данных
            if (station.activeWorkshops > station.totalWorkshops) {
                station.activeWorkshops = station.totalWorkshops;
            }
            
            stations.push_back(station);
        }
        
        file.close();
        cout << "Данные загружены из файла: " << fs::absolute(filename) << endl;
        cout << "Загружено труб: " << pipes.size() << ", КС: " << stations.size() << endl;
    }

    void run() {
        while (true) {
            cout << "\n=== Система управления трубопроводом ===\n"
                 << "1. Добавить трубу\n"
                 << "2. Добавить КС\n"
                 << "3. Просмотр всех объектов\n"
                 << "4. Редактировать трубу\n"
                 << "5. Редактировать КС\n"
                 << "6. Сохранить данные\n"
                 << "7. Загрузить данные\n"
                 << "0. Выход\n";
            
            int choice = getIntInput("Выберите действие: ", 0, 7);
            
            switch (choice) {
                case 1: addPipe(); break;
                case 2: addStation(); break;
                case 3: viewAll(); break;
                case 4: editPipe(); break;
                case 5: editStation(); break;
                case 6: saveData(); break;
                case 7: loadData(); break;
                case 0:
                    cout << "Выход из программы.\n";
                    return;
            }
        }
    }
};

int main() {
    PipelineSystem system;
    system.run();
    return 0;
}

