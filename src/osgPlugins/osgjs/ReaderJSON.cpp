//    copyright: 'Zach Pomerantz tavoricko@gmail.com'
#include "ReaderWriterJSON.h"

using ReadResult = ReaderWriterJSON::ReadResult;

ReadResult ReaderWriterJSON::readNode(const std::string& fileName,
                                      const Options* options) const
{
    std::string ext = osgDB::getLowerCaseFileExtension(fileName);
    if (!acceptsExtension(ext)) return ReadResult::FILE_NOT_HANDLED;

    std::string dataFile = osgDB::findDataFile(fileName, options);
    if (dataFile.empty()) return ReadResult::FILE_NOT_FOUND;

    osgDB::ifstream fin(dataFile.c_str());
    if (fin)
    {
      return readNode(json::parse(fin));
    }
    return 0L;
}

ReadResult ReaderWriterJSON::readNode(const json& data) const
{
    return 0L;
}
