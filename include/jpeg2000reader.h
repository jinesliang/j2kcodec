#ifndef JPEG2000READER_H
#define JPEG2000READER_H

#include "openjpeg.h"
#include <string>

struct ImageData {
  int32_t* data;
  uint32_t w;
  uint32_t h;
};

// class simplej2kcodec
class Jpeg2000Reader {
public:
  Jpeg2000Reader();
  ~Jpeg2000Reader();
  // Decode tile from current loaded file
  std::unique_ptr<ImageData> DecodeTile(const int& tileId);
  // Decode whole image from current loaded file
  std::unique_ptr<ImageData> Decode();
  // Encodes current loaded file
  void EncodeAsTiles(const char* outfile,
              const int32_t* data,
              const unsigned int imageWidth,
              const unsigned int imageHeight,
              const unsigned int tileWidth,
              const unsigned int tileHeight,
              const unsigned int numComps,
              const unsigned int compPrec);
  void CreateInfileStream(const std::string& filename);
  void SetResolutionFactor(const int resolution);

private:
  const int GetInfileFormat(const char* fname);
  void Destroy();
  void SetupDecoder();
  void SetupEncoder();

  bool _isFileLoaded;
  bool _isDecoderSetup;
  char _infileName[OPJ_PATH_LEN];

  // Decoding will happen frequently and we don't want to set up decoder every
  opj_stream_t* _infileStream;
  opj_codec_t* _decoder;
  opj_dparameters_t _decoderParams;
  opj_image_t* _image;
  opj_codestream_info_v2_t* _codestreamInfo;
};

#endif // JPEG2000READER_H
