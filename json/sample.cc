#include <json/json.h>
#include <fstream>
#include <iostream>

using namespace std;

static void putnsp(int n)
{
  for (int i = 0; i < n; ++i) cout << " ";
}

static void print_val(Json::Value val, int id = 0)
{
  putnsp(id);

  switch (val.type()) {
  case Json::nullValue: cout << "null\n"; break;
  case Json::intValue: cout << "int " << val.asLargestInt() << "\n"; break;
  case Json::uintValue: cout << "uint " << val.asLargestUInt() << "\n"; break;
  case Json::realValue: cout << "real " << val.asDouble() << "\n"; break;
  case Json::stringValue: cout << "string " << val.asString() << "\n"; break;
  case Json::booleanValue: cout << "boolean " << val.asBool() << "\n"; break;
  case Json::arrayValue:
    cout << "array[" << val.size() << "]\n";
    for (int i = 0; i < val.size(); ++i) print_val(val[i], id + 2);
    break;
  case Json::objectValue:
    Json::Value::Members keys = val.getMemberNames();
    cout << "object[" << val.size() << "]\n";
    for (int i = 0; i < val.size(); ++i) {
      putnsp(id); cout << keys[i] << " : ";
      print_val(val[keys[i]], id + 2);
    }
    break;
  defaults: cout << "wrong type\n"; break;
  }
}

int main(int argc, char **argv)
{
  if (argc < 2) return 0;

  ifstream fs(argv[1]);

  Json::Reader reader;
  Json::Value obj;
  reader.parse(fs, obj);

  print_val(obj);
  
  return 0;
}
