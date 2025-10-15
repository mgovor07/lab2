#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <limits>
#include <filesystem>
#include <algorithm>
#include <sstream>

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
    
    vector<int> selectMultiplePipes() {
            if (pipes.empty()) {
                cout << "Нет доступных труб!\n";
                return {};
            }
            
            viewAll();
            cout << "\nВыберите номера труб через запятую (например: 0,2,3) или 'all' для выбора всех: ";
            string input;
            getline(cin, input);
            
            if (input == "all" || input == "ALL") {
                vector<int> allIndices;
                for (int i = 0; i < pipes.size(); ++i) {
                    allIndices.push_back(i);
                }
                return allIndices;
            }
            
            vector<int> indices;
            stringstream ss(input);
            string token;
            
            while (getline(ss, token, ',')) {
                try {
                    int index = stoi(token);
                    if (index >= 0 && index < pipes.size()) {
                        indices.push_back(index);
                    } else {
                        cout << "Предупреждение: номер " << index << " не существует и будет пропущен.\n";
                    }
                } catch (const exception&) {
                    cout << "Предупреждение: '" << token << "' не является числом и будет пропущено.\n";
                }
            }
            
            sort(indices.begin(), indices.end());
            indices.erase(unique(indices.begin(), indices.end()), indices.end());
            
            return indices;
        }

        vector<int> selectMultipleStations() {
            if (stations.empty()) {
                cout << "Нет доступных КС!\n";
                return {};
            }
            
            viewAll();
            cout << "\nВыберите номера КС через запятую (например: 0,2,3) или 'all' для выбора всех: ";
            string input;
            getline(cin, input);
            
            if (input == "all" || input == "ALL") {
                vector<int> allIndices;
                for (int i = 0; i < stations.size(); ++i) {
                    allIndices.push_back(i);
                }
                return allIndices;
            }
            
            vector<int> indices;
            stringstream ss(input);
            string token;
            
            while (getline(ss, token, ',')) {
                try {
                    int index = stoi(token);
                    if (index >= 0 && index < stations.size()) {
                        indices.push_back(index);
                    } else {
                        cout << "Предупреждение: номер " << index << " не существует и будет пропущен.\n";
                    }
                } catch (const exception&) {
                    cout << "Предупреждение: '" << token << "' не является числом и будет пропущено.\n";
                }
            }
            
            sort(indices.begin(), indices.end());
            indices.erase(unique(indices.begin(), indices.end()), indices.end());
            
            return indices;
        }

public:
    void addMultiplePipes() {
            int count = getIntInput("Сколько труб добавить? ", 1, 100);
            for (int i = 0; i < count; ++i) {
                cout << "\n--- Добавление трубы " << (i + 1) << " из " << count << " ---\n";
                addPipe();
            }
            cout << "Добавлено " << count << " труб. Всего труб: " << pipes.size() << "\n";
        }

        void addMultipleStations() {
            int count = getIntInput("Сколько КС добавить? ", 1, 100);
            for (int i = 0; i < count; ++i) {
                cout << "\n--- Добавление КС " << (i + 1) << " из " << count << " ---\n";
                addStation();
            }
            cout << "Добавлено " << count << " КС. Всего КС: " << stations.size() << "\n";
        }
    
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
    
    void deleteMultiplePipes() {
            vector<int> indices = selectMultiplePipes();
            if (indices.empty()) return;
            
            sort(indices.rbegin(), indices.rend());
            int count = 0;
            
            for (int index : indices) {
                cout << "Удалена труба: " << pipes[index].name << "\n";
                pipes.erase(pipes.begin() + index);
                count++;
            }
            
            cout << "Удалено " << count << " труб. Осталось труб: " << pipes.size() << "\n";
        }

        void deleteMultipleStations() {
            vector<int> indices = selectMultipleStations();
            if (indices.empty()) return;
            
            sort(indices.rbegin(), indices.rend());
            int count = 0;
            
            for (int index : indices) {
                cout << "Удалена КС: " << stations[index].name << "\n";
                stations.erase(stations.begin() + index);
                count++;
            }
            
            cout << "Удалено " << count << " КС. Осталось КС: " << stations.size() << "\n";
        }

        void editMultiplePipes() {
            vector<int> indices = selectMultiplePipes();
            if (indices.empty()) return;
            
            cout << "\nВыбрано " << indices.size() << " труб для редактирования.\n";
            cout << "1. Изменить статус ремонта для всех\n2. Изменить диаметр для всех\n";
            int choice = getIntInput("Выберите действие: ", 1, 2);
            
            if (choice == 1) {
                cout << "1. Поставить все на ремонт\n2. Снять все с ремонта\n3. Инвертировать статус\n";
                int action = getIntInput("Выберите действие: ", 1, 3);
                
                for (int index : indices) {
                    if (action == 1) pipes[index].underRepair = true;
                    else if (action == 2) pipes[index].underRepair = false;
                    else pipes[index].underRepair = !pipes[index].underRepair;
                }
                cout << "Статус ремонта изменен для " << indices.size() << " труб.\n";
            } else {
                int newDiameter = getIntInput("Введите новый диаметр для всех выбранных труб (мм): ", 1);
                for (int index : indices) {
                    pipes[index].diameter = newDiameter;
                }
                cout << "Диаметр изменен для " << indices.size() << " труб.\n";
            }
        }

        void editMultipleStations() {
            vector<int> indices = selectMultipleStations();
            if (indices.empty()) return;
            
            cout << "\nВыбрано " << indices.size() << " КС для редактирования.\n";
            cout << "1. Запустить цехи\n2. Остановить цехи\n3. Изменить класс\n";
            int choice = getIntInput("Выберите действие: ", 1, 3);
            
            if (choice == 1) {
                for (int index : indices) {
                    if (stations[index].activeWorkshops < stations[index].totalWorkshops) {
                        stations[index].activeWorkshops++;
                    }
                }
                cout << "Запущены цехи для " << indices.size() << " КС.\n";
            } else if (choice == 2) {
                for (int index : indices) {
                    if (stations[index].activeWorkshops > 0) {
                        stations[index].activeWorkshops--;
                    }
                }
                cout << "Остановлены цехи для " << indices.size() << " КС.\n";
            } else {
                int newClass = getIntInput("Введите новый класс для всех выбранных КС: ", 1);
                for (int index : indices) {
                    stations[index].stationClass = newClass;
                }
                cout << "Класс изменен для " << indices.size() << " КС.\n";
            }
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
                             << "3. Добавить несколько труб\n"
                             << "4. Добавить несколько КС\n"
                             << "5. Просмотр всех объектов\n"
                             << "6. Редактировать трубу\n"
                             << "7. Редактировать КС\n"
                             << "8. Редактировать несколько труб\n"
                             << "9. Редактировать несколько КС\n"
                             << "10. Удалить трубу\n"
                             << "11. Удалить КС\n"
                             << "12. Удалить несколько труб\n"
                             << "13. Удалить несколько КС\n"
                             << "14. Сохранить данные\n"
                             << "15. Загрузить данные\n"
                             << "0. Выход\n";
            
            int choice = getIntInput("Выберите действие: ", 0, 15);
            
            switch (choice) {
                case 1: addPipe(); break;
                case 2: addStation(); break;
                case 3: addMultiplePipes(); break;
                case 4: addMultipleStations(); break;
                case 5: viewAll(); break;
                case 6: editPipe(); break;
                case 7: editStation(); break;
                case 8: editMultiplePipes(); break;
                case 9: editMultipleStations(); break;
                case 10:
                    if (!pipes.empty()) {
                        viewAll();
                        int index = getIntInput("Введите номер трубы для удаления: ", 0, pipes.size()-1);
                        cout << "Удалена труба: " << pipes[index].name << "\n";
                        pipes.erase(pipes.begin() + index);
                    } else {
                        cout << "Нет доступных труб!\n";
                    }
                    break;
                case 11:
                    if (!stations.empty()) {
                        viewAll();
                        int index = getIntInput("Введите номер КС для удаления: ", 0, stations.size()-1);
                        cout << "Удалена КС: " << stations[index].name << "\n";
                        stations.erase(stations.begin() + index);
                    } else {
                        cout << "Нет доступных КС!\n";
                    }
                    break;
                case 12: deleteMultiplePipes(); break;
                case 13: deleteMultipleStations(); break;
                case 14: saveData(); break;
                case 15: loadData(); break;
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

