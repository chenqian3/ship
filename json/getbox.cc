#include <json/json.h>
#include <fstream>
#include <iostream>
#include <cstdio>

using namespace std;

class ShipBox {
public:
  int id, x, y, w, h;
};

class ShipInfo {
public:
  static const int _NAME_LEN = 1024;
  char image[_NAME_LEN];
  vector<ShipBox> ships;
  ShipInfo() { image[0] = 0; }
};

int getvalue(Json::Value val, bool *fp = nullptr)
{
  int v;
  bool f = false;
  switch (val.type()) {
  case Json::intValue:  v = val.asLargestInt(); f = true; break;
  case Json::uintValue: v = val.asLargestUInt(); f = true; break;
  case Json::realValue: v = val.asDouble(); f = true; break;
  defaults: break;
  }
  if (fp != nullptr) *fp = f;
  return v;
}

void getinfo(Json::Value val, ShipInfo *info)
{
  switch (val.type()) {
  case Json::nullValue:    break;
  case Json::intValue:     break;
  case Json::uintValue:    break;
  case Json::realValue:    break;
  case Json::stringValue:  break;
  case Json::booleanValue: break;
  case Json::arrayValue:   break;
  case Json::objectValue:
    Json::Value::Members keys = val.getMemberNames();
    for (int i = 0; i < val.size(); ++i) {
      if (strcmp(keys[i].c_str(), "data") != 0) continue;
      Json::Value av = val["data"];
      if (av.type() == Json::arrayValue) {
	av = av[0];
	if (av.type() == Json::objectValue) {
	  Json::Value boxv = av["box"];
	  info->ships = vector<ShipBox>(boxv.size());
	  for (int j = 0; j < boxv.size(); ++j) {
	    Json::Value box = boxv[j];
	    info->ships[j].id = getvalue(box["ID"]);
	    info->ships[j].x  = getvalue(box["X"]);
	    info->ships[j].y  = getvalue(box["Y"]);
	    info->ships[j].w  = getvalue(box["W"]);
	    info->ships[j].h  = getvalue(box["H"]);
	  }
	  strncpy(info->image, av["file"].asCString(), ShipInfo::_NAME_LEN - 1);
	}
      }
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
  ShipInfo info;

  getinfo(obj, &info);
  cout << info.image << "\n";
  for (int i = 0; i < info.ships.size(); ++i) {
    printf("id=%d, (%d, %d, %d, %d)\n", info.ships[i].id,
	   info.ships[i].x, info.ships[i].y,
	   info.ships[i].w, info.ships[i].h);
  }
  return 0;
}
