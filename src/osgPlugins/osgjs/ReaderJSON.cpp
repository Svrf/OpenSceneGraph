//    copyright: 'Zach Pomerantz tavoricko@gmail.com'
#include "ReaderWriterJSON.h"

using ReadResult = ReaderWriterJSON::ReadResult;

osg::Group* readGroup(const json::object_t& data);
osg::Node* readNode(const json::object_t& data);

ReadResult ReaderWriterJSON::readNode(const std::string& fileName,
                                      const Options* options) const {
    std::string ext = osgDB::getLowerCaseFileExtension(fileName);
    if (!acceptsExtension(ext)) return ReadResult::FILE_NOT_HANDLED;

    std::string dataFile = osgDB::findDataFile(fileName, options);
    if (dataFile.empty()) return ReadResult::FILE_NOT_FOUND;

    osgDB::ifstream fin(dataFile.c_str());
    if (fin) {
      json data = json::parse(fin);
      return readGroup(data);
    }
    return 0L;
}

enum struct Key {
  Generator,
  Version,
  Node,
  Children,
  UniqueID,
  Name,
  Geometry,
  PrimitiveSetList,
  DrawElementUShort,
  Indices,
  File,
};

Key parseKey(const std::string& key) {

    if (key == "Generator") {
      return Key::Generator;
    } else if (key == "Version") {
      return Key::Version;
    } else if (key == "osg.Node") {
      return Key::Node;
    } else if (key == "Children") {
      return Key::Children;
    } else if (key == "UniqueID") {
      return Key::UniqueID;
    } else if (key == "Name") {
      return Key::Name;
    } else if (key == "osg.Geometry") {
      return Key::Geometry;
    } else if (key == "PrimitiveSetList") {
      return Key::PrimitiveSetList;
    } else if (key == "DrawElementUShort") {
      return Key::DrawElementUShort;
    } else if (key == "Indices") {
      return Key::Indices;
    } else if (key == "File") {
      return Key::File;
    }

    return Key();
}

osg::Group* readGroup(const json::object_t& data) {
    osg::Group* group = new osg::Group;

    for (const auto& it : data) {
        switch (parseKey(it.first)) {
        case Key::Generator:
        case Key::Version:
            std::cout << it.first << std::endl;
            break;
        case Key::Node:
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
