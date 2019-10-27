#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <any>
#include <fstream>
#include "json.hpp"
using namespace std;
Json::Json(const std::string& s) {
  bool flag, flag2;
  if (s[0] == '{') {
    flag = true;  // Объект не массив
  }
  if (s[0] == '[') {
    flag = false;  // Массив
  }
  if (!(s[0] == '{' || s[0] == '[')) throw exception("Неверный формат строки");
  void* pointer = nullptr;  // Для задания null
  string key = "";          // Ключ
  double x;
  int a;  // Для чтения вещественных и целых значений
  // Ищем ключ - строку

  int k1 = 0, k2;  // Индексы для перемещения по строке
  while (1)        // Цикл поиска ключа и значения
  {
    if (flag)  // Чиатем ключ для объекта
    {
      k1 = (int)s.find("\"", k1);  // начало строки "
      if (k1 < 0) throw exception("Неверный формат строки");
      k2 = (int)s.find("\"", k1 + 1);  // конец строки "
      if (k2 < 0) throw exception("Неверный формат строки");
      key = s.substr(k1 + 1, k2 - k1 - 1);  // Извлекаем ключ
      k1 = (int)s.find(":", k2 + 1);        // Ищем : после ключа
      if (k1 < 0) throw exception("Неверный формат строки");
    }
    k1++;  // Символ за : или за [
    string sOb;
    // Цикл чтения значения
    while (1)  // Перебираем символы разделители пропускаем
    {
      // k1 - текущий индекс строки с неготначинаем чтение
      if (k1 >= s.size()) throw exception("Неверный формат строки");
      if (s[k1] == ',')
        if (flag)
          break;
        else
          k1++;
      if (s[k1] == '}')
        if (flag)
          break;
        else
          throw exception("Неверный формат строки");
      if (s[k1] == ']')
        if (!flag)
          break;
        else
          throw exception("Неверный формат строки");
      if (s[k1] != ' ' && s[k1] != '\n' && s[k1] != '\r' &&
          s[k1] != '\t')  // Ищем не разделитель
      {
        switch (s[k1])  // Определяем тип значения
        {
          case '{':                         // Это объект
            k2 = (int)s.find("}", k1 + 1);  // конец объекта
            if (k2 < 0) throw exception("Неверный формат строки");
            sOb = s.substr(k1, k2 - k1 + 1);  // Строка задающая объект
            //	any_cast<unordered_map<string, any> *>(data)->insert({ key,
            //any_cast<any>(Json(sOb)) });
            if (flag)
              Ob1.insert({key, Json(sOb)});  // Рекурсивный вызов конструктора
                                             // для вложенного объекта
            else
              Ob2.push_back(Json(sOb));  // Рекурсивный вызов конструктора для
                                         // вложенного объекта
            k2++;  // Переход к след символу за }
            break;
          case '[':  // Значение массив
          {
            k2 = (int)s.find("]", k1 + 1);  // конец массива
            if (k2 < 0) throw exception("Неверный формат строки");
            sOb = s.substr(k1, k2 - k1 + 1);  // Строка задающая объект
            if (flag)
              Ob1.insert({key, Json(sOb)});  // Рекурсивный вызов конструктора
                                             // для вложенного объекта
            else
              Ob2.push_back(Json(sOb));  // Рекурсивный вызов конструктора для
                                         // вложенного объекта
            k2++;  // Переход к след символу за ]
            break;
          }
          case '\"':                         // Значение строка
            k2 = (int)s.find("\"", k1 + 1);  // конец строки
            if (k2 < 0) throw exception("Неверный формат строки");
            sOb = s.substr(k1 + 1, k2 - k1 - 1);  // Значение - простая строка
            if (flag)
              Ob1.insert({key, sOb});
            else
              Ob2.push_back(sOb);
            k2++;  // Переход к след символу за " (концом строки)
            break;
          case 'f':
          case 't':  // значение true или false
            if (s[k1] == 'f')
              flag2 = false;
            else
              flag2 = true;
            k2 = k1 + 1;
            while (isalpha(s[k2])) k2++;  // Пропускаем символы - буквы
            if (flag)
              Ob1.insert({key, flag2});
            else
              Ob2.push_back(flag2);
            break;
          case 'n':  // null
            k2 = k1 + 1;
            while (isalpha(s[k2])) k2++;  // Пропускаем символы - буквы
            if (flag)
              Ob1.insert({key, pointer});
            else
              Ob2.push_back(pointer);
            break;
          default:
            // Читаем значение как число
            stringstream ss(s.substr(k1));
            ss >> x;
            if (ss.fail())  // Ошибка чтения
              throw exception("Неверный формат строки");
            // Проверяем целое число или вещественное
            a = x;
            if (flag) {
              if (a == x)
                Ob1.insert({key, a});  // Целое
              else
                Ob1.insert({key, x});  // Вещественное
            } else if (a == x)
              Ob2.push_back(a);  // Целое
            else
              Ob2.push_back(x);  // Вещественное

            k2 = k1 + 1;
            while (s[k2] >= '0' && s[k2] <= '9' || s[k2] == '.')
              k2++;  // Пропускаем части числа цифры и .

        }  // switch (s[k])
        k1 = k2;

      }  // if (s[k] != ' ' && s[k] != '\n' && s[k] != '\r'); // Ищем не
         // разделитель
      else
        k1++;  // Символы разделители пропускаем
      if (s[k1] == ']') break;
    }  // while(1)

    if (s[k1] == ']' || s[k1] == '}') break;
  }  // while(1)

  if (flag)
    data = Ob1;
  else
    data = Ob2;
}

void Json::print() {
  if (is_object())  // Печать объекта
  {
    cout << endl << "{";
    int i = 0;
    for (auto pos : any_cast<unordered_map<string, any>>(data)) {
      if (i > 0) cout << ", ";
      cout << endl << pos.first << ": ";  // Ключ
                                          // Печать значений
      if (pos.second.type() == typeid(string))
        cout << any_cast<string>(pos.second);
      if (pos.second.type() == typeid(int)) cout << any_cast<int>(pos.second);
      if (pos.second.type() == typeid(double))
        cout << any_cast<double>(pos.second);
      if (pos.second.type() == typeid(bool)) cout << any_cast<bool>(pos.second);
      if (pos.second.type() == typeid(void*)) cout << "null";
      if (pos.second.type() == typeid(Json)) any_cast<Json>(pos.second).print();
      i++;
    }
    cout << endl << "}";
  } else  // Печать массива
  {
    cout << endl << "[";
    int i = 0;
    for (auto pos : any_cast<vector<any>>(data)) {
      if (i > 0) cout << ", ";
      // Печать значений массива
      if (pos.type() == typeid(string)) cout << any_cast<string>(pos);
      if (pos.type() == typeid(int)) cout << any_cast<int>(pos);
      if (pos.type() == typeid(double)) cout << any_cast<double>(pos);
      if (pos.type() == typeid(bool)) cout << any_cast<bool>(pos);
      if (pos.type() == typeid(void*)) cout << "null";
      if (pos.type() == typeid(Json)) any_cast<Json>(pos).print();
      i++;
    }
    cout << "]";
  }
}

// Метод возвращает true, если данный экземпляр содержит в себе JSON-массив.
// Иначе false.
bool Json::is_array() const {
  if (data.type() == typeid(vector<any>))
    return true;
  else
    return false;
}
// Метод возвращает true, если данный экземпляр содержит в себе JSON-объект.
// Иначе false.
bool Json::is_object() const {
  if (data.type() != typeid(vector<any>))
    return true;
  else
    return false;
}

// Метод возвращает значение по ключу key, если экземпляр является
// JSON-объектом. Значение может иметь один из следующих типов: Json,
// std::string, double, bool или быть пустым. Если экземпляр является
// JSON-массивом, генерируется исключение.
std::any& Json::operator[](const std::string& key) {
  if (data.type() == typeid(vector<any>)) {
    throw exception("Неверный тип");
  }
  return Ob1[key];
}

// Метод возвращает значение по индексу index, если экземпляр является
// JSON-массивом. Значение может иметь один из следующих типов: Json,
// std::string, double, bool или быть пустым. Если экземпляр является
// JSON-объектом, генерируется исключение.
std::any& Json::operator[](int index) {
  if (data.type() != typeid(vector<any>)) throw exception("Неверный тип");
  return Ob2[index];
}

// Метод возвращает объект класса Json из строки, содержащей Json-данные.
Json Json::parse(const std::string& s) {
  Json JS(s);
  return JS;
}

// Метод возвращает объекта класса Json из файла, содержащего Json-данные в
// текстовом формате.
Json Json::parseFile(const std::string& path_to_file) {
  ifstream F(path_to_file);
  string Str = "";
  while (!F.eof()) {
    string S1;
    F >> S1;
    Str += S1;
  }
  // cout << endl << Str << endl;
  F.close();
  return parse(Str);
}
