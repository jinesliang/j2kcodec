## j2kcodec

Cross platform simple codec to encode, decode JPEG2000 files and extract metadata from JPEG2000 files to JSON format.

#### Setup
* Clone repo and submodules `git clone --recurse-submodules https://github.com/novalain/j2kcodec.git`
* Run `mkdir build && cd build && cmake ..`
* Run makefile or open up VS
* On Windows: Copy over openjp2.dll to bin folder 😬 

### Samples
There are two sample apps for extracting metadata from .jp2 files: <br/>

`./extract_json_from_jp2 -m {filename} // Single file`<br/>
`./extract_data_from_jp2_rec -m {directory} --{json/xml} // Recursively extract from all files in directory`

Example
`./extract_data_from_jp2_rec -m ../relative_path/directory_with_jp2s --json`
