#include "openjpegwrapper.h"
#include "format_defs.h"
#include <iostream>
#include <memory>

#define JP2_RFC3745_MAGIC "\x00\x00\x00\x0c\x6a\x50\x20\x20\x0d\x0a\x87\x0a"
#define JP2_MAGIC "\x0d\x0a\x87\x0a"
#define J2K_CODESTREAM_MAGIC "\xff\x4f\xff\x51"

OpenjpegWrapper::OpenjpegWrapper()
    : _isFileLoaded(false),
      _isDecoderSetup(false),
      _l_stream(nullptr),
      _l_codec(nullptr),
      _image(nullptr),
      _cstr_info(nullptr) {}

OpenjpegWrapper::~OpenjpegWrapper() {
  Destroy();
}

std::unique_ptr<ImageData> OpenjpegWrapper::DecodeTile(const int& tileId) {
  if (!_isDecoderSetup) {
    std::cerr << "Decoder needs to be set up before reading tiles\n";
    return nullptr;
  }

  if (!opj_get_decoded_tile(_l_codec, _l_stream, _image, tileId)) {
    std::cerr << "Could not decode tile\n";
    Destroy();
    return nullptr;
  }
  // Assume greyscale for now
  ImageData im = {_image->comps[0].data, _image->comps[0].w, _image->comps[0].h};
  return std::make_unique<ImageData>(im);
}

void OpenjpegWrapper::Destroy() {
  opj_stream_destroy(_l_stream);
  opj_destroy_cstr_info(&_cstr_info);
  opj_destroy_codec(_l_codec);
  opj_image_destroy(_image);
}

void OpenjpegWrapper::Load(const std::string& filename) {
  opj_set_default_decoder_parameters(&_parameters);
  //strncpy(_parameters.infile, filename, OPJ_PATH_LEN - 1);
  strcpy(_parameters.infile, filename.c_str());
  _parameters.decod_format = GetInfileFormat(_parameters.infile);

  switch(_parameters.decod_format) {
    case J2K_CFMT: { // JPEG-2000 codestream
      _l_codec = opj_create_decompress(OPJ_CODEC_J2K);
      break;
    }
    case JP2_CFMT: { // JPEG 2000 compressed image data
      _l_codec = opj_create_decompress(OPJ_CODEC_JP2);
      break;
    }
    case JPT_CFMT: { // JPEG 2000, JPIP
      _l_codec = opj_create_decompress(OPJ_CODEC_JPT);
      break;
    }
    default:
      std::cerr << "Unrecognized format for input " << _parameters.infile
                << " - Accept only .j2k, .jp2, .jpc or .jpt]";
      return;
  }

  _l_stream = opj_stream_create_default_file_stream(_parameters.infile, 1);
  if (!_l_stream){
    std::cerr << "Failed to create stream from file " << _parameters.infile;
    return;
  }

  _isFileLoaded = true;
}

void OpenjpegWrapper::SetupDecoder(const int resolution) {
  if (!_isFileLoaded) {
    std::cerr << "No file loaded\n";
    return;
  }

  if (!opj_setup_decoder(_l_codec, &_parameters)) {
    std::cerr << "Failed to set up the decoder\n";
    Destroy();
    return;
  }

  // Read the main header of the codestream and if necessary the JP2 boxes
  if (!opj_read_header(_l_stream, _l_codec, &_image)) {
    std::cerr << "Failed to read the header\n";
    Destroy();
    return;
  }

  // Extract some info from the code stream
  _cstr_info = opj_get_cstr_info(_l_codec);
  fprintf(stdout, "The file contains %dx%d tiles\n", _cstr_info->tw, _cstr_info->th);

  // Catch events using our callbacks and give a local context
  opj_set_info_handler(_l_codec, [](const char* msg, void* client_data) {
                        (void)client_data;
                        std::clog << "[INFO]" << msg;
                      }, 00);
  opj_set_warning_handler(_l_codec, [](const char* msg, void* client_data) {
                        (void)client_data;
                        std::cerr << "[WARNING]" << msg;
                      }, 00);
  opj_set_error_handler(_l_codec, [](const char* msg, void* client_data) {
                        (void)client_data;
                        std::cerr << "[ERROR]" << msg;
                      }, 00);

  _isDecoderSetup = true;
}

const int OpenjpegWrapper::GetInfileFormat(const char *fname) {

  const auto get_file_format = [](const char* filename) {
    unsigned int i;
    static const char *extension[] = {"pgx", "pnm", "pgm", "ppm", "bmp","tif", "raw", "tga", "png", "j2k", "jp2", "jpt", "j2c", "jpc" };
    static const int format[] = { PGX_DFMT, PXM_DFMT, PXM_DFMT, PXM_DFMT, BMP_DFMT, TIF_DFMT, RAW_DFMT, TGA_DFMT, PNG_DFMT, J2K_CFMT, JP2_CFMT, JPT_CFMT, J2K_CFMT, J2K_CFMT };
    char * ext = strrchr(filename, '.');
    if (ext == NULL)
      return -1;
    ext++;
    if(ext) {
      for(i = 0; i < sizeof(format)/sizeof(*format); i++) {
        if(strncmp(ext, extension[i], 3) == 0) {
          return format[i];
        }
      }
    }

    return -1;
  };

  FILE *reader;
  const char *s, *magic_s;
  int ext_format, magic_format;
  unsigned char buf[12];
  OPJ_SIZE_T l_nb_read;

  reader = fopen(fname, "rb");

  if (reader == NULL) {
    return -1;
  }

  memset(buf, 0, 12);
  l_nb_read = fread(buf, 1, 12, reader);
  fclose(reader);
  if (l_nb_read != 12) {
    return -1;
  }

  ext_format = get_file_format(fname);

  if (ext_format == JPT_CFMT) {
    return JPT_CFMT;
  }

  if (memcmp(buf, JP2_RFC3745_MAGIC, 12) == 0 || memcmp(buf, JP2_MAGIC, 4) == 0) {
    magic_format = JP2_CFMT;
    magic_s = ".jp2";
  } else if (memcmp(buf, J2K_CODESTREAM_MAGIC, 4) == 0) {
    magic_format = J2K_CFMT;
    magic_s = ".j2k or .jpc or .j2c";
  } else {
    return -1;
  }

  if (magic_format == ext_format) {
    return ext_format;
  }

  s = fname + strlen(fname) - 4;
  std::cerr << "Extension of file is incorrect! Found " << s << " should be " << magic_s;
  return magic_format;
}
