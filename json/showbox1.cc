#include <json/json.h>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <opencv2/opencv.hpp>

using namespace cv;
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

void correct_path(char *d, const char *s)
{
  int n = strlen(s);
  int i0 = 2;
  for (int i = i0; i < n; ++i) {
    int c = s[i];
    if (c == '\\') c = '/';
    d[i - i0] = c;
  }
  d[n - i0] = 0;
}

int main(int argc, char **argv)
{
  const char *imgpre  = "/home/chen/NAS/ship_d/images";
  const char *jsonpre = "/home/chen/NAS/ship_d/ship_20190308_1_chen";
  if (argc < 2) return 0;

  Json::Reader reader;
  Json::Value obj;
  ShipInfo info;

  char json_path[1024];
  char image_path[1024];

  namedWindow(argv[0], WINDOW_AUTOSIZE);
  for (int i = 1; i < argc; ++i) {
    sprintf(json_path, "%s/%s.json", jsonpre, argv[i]);
    sprintf(image_path, "%s/%s.jpg", imgpre, argv[i]);
    ifstream fs(json_path);
    reader.parse(fs, obj);
    getinfo(obj, &info);

    Mat image = imread(image_path, IMREAD_COLOR);
 
    if (image.data == 0) {
      printf("Failed to read image file %s\n", image_path);
      return 2;
    }
    for (int i = 0; i < info.ships.size(); ++i) {
      int x = info.ships[i].x, y = info.ships[i].y;
      int w = info.ships[i].w, h = info.ships[i].h;
      rectangle(image, Point(x, y), Point(x+w-1, y+h-1), Scalar(Vec3b(0, 255, 255)), 2);
    }
    imshow(argv[0], image);
    waitKey(0);
  }
  return 0;
}
