#include <iostream>
#include <string>
#include "simplej2kcodec.h"
#include "xml2json.hpp"

using namespace j2c;

int main(int argc, char* argv[]) {
  SimpleJ2kCodec j(/*verbose=*/false);
  if (argc != 3 || std::string(argv[1]) != "-m") {
      std::cerr << "[ERROR] input format. Example: \n ./extract_json_from_jp2 -m image.jp2\n";
      return 0;
  } 
  const std::string fname = std::string(argv[2]);
  if (fname.substr(fname.find_last_of(".") + 1) != "jp2") {
    std::cerr << "[ERROR] wrong file name, must be .jp2\n";
    return 0;
  }

  const std::string path = argv[2];
  const XmlData xmlData = j.fetchXMLData(path);
  const std::string jsonData = xml2json(reinterpret_cast<char*>(xmlData.data));

  size_t lastindex = path.find_last_of(".");
  std::string basename = path.substr(0, lastindex);
  std::ofstream myfile(basename + ".json");
  if (myfile.is_open()) {
    for (int i = 0; i < jsonData.size(); ++i) {
      myfile << jsonData[i];
    }
    myfile.close();
  }

  return 0;
}
