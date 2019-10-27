// Copyright 2018 Your Name <your_email>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_
#include <any>
#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;
class Json {
  std::any data;
  unordered_map<string, any> Ob1;
  vector<any> Ob2;
 public:
  // ����������� �� ������, ���������� Json-������.
  Json(const std::string& s);

  void print();

  // ����� ���������� true, ���� ������ ��������� �������� � ���� JSON-������.
  // ����� false.
  bool is_array() const;

  // ����� ���������� true, ���� ������ ��������� �������� � ���� JSON-������.
  // ����� false.
  bool is_object() const;

  // ����� ���������� �������� �� ����� key, ���� ��������� ��������
  // JSON-��������. �������� ����� ����� ���� �� ��������� �����: Json,
  // std::string, double, bool ��� ���� ������. ���� ��������� ��������
  // JSON-��������, ������������ ����������.
  std::any& operator[](const std::string& key);

  // ����� ���������� �������� �� ������� index, ���� ��������� ��������
  // JSON-��������. �������� ����� ����� ���� �� ��������� �����: Json,
  // std::string, double, bool ��� ���� ������. ���� ��������� ��������
  // JSON-��������, ������������ ����������.
  std::any& operator[](int index);

  // ����� ���������� ������ ������ Json �� ������, ���������� Json-������.
  static Json parse(const std::string& s);

  // ����� ���������� ������� ������ Json �� �����, ����������� Json-������ �
  // ��������� �������.
  static Json parseFile(const std::string& path_to_file);
};
#endif // INCLUDE_HEADER_HPP_
