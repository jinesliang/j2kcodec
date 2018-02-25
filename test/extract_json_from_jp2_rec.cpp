#include <iostream>
#include <string>
#include "j2kcodec.h"
#include "xml2json.hpp"
#include <filesystem>

using namespace j2c;

void extractJSON(const std::string& path, J2kCodec& j) {
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
}

int main(int argc, char* argv[]) {
	J2kCodec j(/*verbose=*/false);
	if (argc != 3 || std::string(argv[1]) != "-m") {
		std::cerr << "[ERROR] input format. Example: \n ./extract_json_from_jp2_rec -m directorypath\n";
		return 0;
	}

	const std::string dirpath = std::string(argv[2]);
	using std::experimental::filesystem::recursive_directory_iterator;
	for (auto& dirEntry : recursive_directory_iterator(dirpath)) {
		if (is_regular_file(dirEntry.path())) {
			const std::string path = dirEntry.path().string();
			if (path.substr(path.find_last_of(".") + 1) == "jp2") {
				extractJSON(path, j);
			}
		}
	}
		   
	return 0;
}
