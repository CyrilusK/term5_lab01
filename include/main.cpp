#include <iostream>
#include "jsonFor3.hpp"
using json = nlohmann::json;
using namespace std;
int main() {
  json j_in = json::array(
      {{"Si-9.15", "RTS-9.15", "GAZP-9.15"},
       {100024, 100027, 100050},
       {"Futures contract for USD/RUB", "Futures contract for index RTS",
        "Futures contract for Gazprom shares"}});
  json j_out;
  for (json::iterator it = j_in[0].begin(); it != j_in[0].end(); ++it) {
    j_out[it - j_in[0].begin()]["ticket"] = *it;
  }
  for (json::iterator it = j_in[1].begin(); it != j_in[1].end(); ++it) {
    j_out[it - j_in[1].begin()]["id"] = *it;
  }
  for (json::iterator it = j_in[2].begin(); it != j_in[2].end(); ++it) {
    j_out[it - j_in[2].begin()]["description"] = *it;
  }
  cout << j_out << endl;
  return 0;
}