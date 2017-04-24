#ifndef JPEG2000READER_H
#define JPEG2000READER_H

#include "openjpeg.h"
#include <string>

struct ImageData {
  int32_t* data;
  uint32_t w;
  uint32_t h;
};

class Jpeg2000Reader {
public:
  Jpeg2000Reader();
  ~Jpeg2000Reader();
  std::unique_ptr<ImageData> DecodeTile(const int& resolutionLevel);
  void Load(const std::string& filename);
  void SetResolutionFactor(const int resolution);

private:
  const int GetInfileFormat(const char* fname);
  void Destroy();
  void SetupDecoder();

  bool _isFileLoaded;
  opj_stream_t* _l_stream;
  opj_codec_t* _l_codec;
  opj_dparameters_t _parameters;
  opj_image_t* _image;
  opj_codestream_info_v2_t* _cstr_info;
};

#endif // JPEG2000READER_H
