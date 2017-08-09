//    copyright: 'Zach Pomerantz tavoricko@gmail.com'
#include "ReaderWriterJSON.h"

using ReadResult = ReaderWriterJSON::ReadResult;

namespace osgjs {
osg::Node* read(const json::object_t& data);
};

ReadResult ReaderWriterJSON::readNode(const std::string& fileName,
                                      const Options* options) const {
    std::string ext = osgDB::getLowerCaseFileExtension(fileName);
    if (!acceptsExtension(ext)) return ReadResult::FILE_NOT_HANDLED;

    std::string dataFile = osgDB::findDataFile(fileName, options);
    if (dataFile.empty()) return ReadResult::FILE_NOT_FOUND;

    osgDB::ifstream fin(dataFile.c_str());
    if (fin) {
      json data = json::parse(fin);
      return osgjs::read(data);
    }
    return 0L;
}

#define KEYS(m) \
  m(Generator) \
  m(Version) \
  m(Node)

#define ENUM_VAL(val) val##,
#define ENUM_DEF(name, vals) enum name { vals(KEYS_ENUM_VAL) };
#define ENUM_HASH(vals) const std::string keys[] = { vals(KEYS_ENUM_VAL) };
#define ENUM_LOOKUP(name) \
  typeName parse##typeName##(const std::string& str) { \
    for (int i = 0; i <
  }

KEYS_ENUM_DEF(KEYS);
KEYS_ENUM_LOOKUP(KEYS);

osg::Node* readNode(const json::object_t& data);

osg::Node* read(const json::object_t& data) {
    osg::Node* node = new osg::Node;

    for (const auto& it : data) {
        switch (keys[it.first]) {
        case Generator:
        case Version:
            std::cout << it.first << std::endl;
            break;
        case Node:
            group->addChild(readNode(it.second));
            break;
        default:
            std::cerr << "unimplemented: " << it.first << std::endl;
        }
    }

    return group;
}

osg::Node* readNode(const json::object_t& data) {
  osg::Node* node = new osg::Node;

  for (const auto& it : data) {
      switch (parseKey(it.first)) {
      case Key::Children:
      case Key::UniqueID:
      case Key::Name:
      default:
          std::cerr << "unimplemented: " << it.first << std::endl;
      }
    }

    return node;
}
};
