//    copyright: 'Cedric Pinson cedric@plopbyte.com'
#include <osg/Image>
#include <osg/Notify>
#include <osg/Geode>
#include <osg/GL>
#include <osg/Version>
#include <osg/Endian>
#include <osg/Projection>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>

#include <osgUtil/UpdateVisitor>
#include <osgDB/ReaderWriter>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgDB/Registry>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>

#include <osgAnimation/UpdateMatrixTransform>
#include <osgAnimation/AnimationManagerBase>
#include <osgAnimation/BasicAnimationManager>

#include <vector>

#include "json.hpp"
#include "json_stream.h"
#include "JSON_Objects.h"
#include "Animation.h"
#include "CompactBufferVisitor.h"
#include "WriteVisitor.h"

using namespace osg;
using json = nlohmann::json;

class ReaderWriterJSON : public osgDB::ReaderWriter
{
public:

    struct OptionsStruct {
         int resizeTextureUpToPowerOf2;
         bool useExternalBinaryArray;
         bool mergeAllBinaryFiles;
         bool disableCompactBuffer;
         bool inlineImages;
         bool varint;
         bool strictJson;
         std::vector<std::string> useSpecificBuffer;
         std::string baseLodURL;
         OptionsStruct() {
             resizeTextureUpToPowerOf2 = 0;
             useExternalBinaryArray = false;
             mergeAllBinaryFiles = false;
             disableCompactBuffer = false;
             inlineImages = false;
             varint = false;
             strictJson = true;
         }
    };


    ReaderWriterJSON()
    {
        supportsExtension("osgjs","OpenSceneGraph Javascript implementation format");
        supportsOption("resizeTextureUpToPowerOf2=<int>","Specify the maximum power of 2 allowed dimension for texture. Using 0 will disable the functionality and no image resizing will occur.");
        supportsOption("useExternalBinaryArray","create binary files for vertex arrays");
        supportsOption("mergeAllBinaryFiles","merge all binary files into one to avoid multi request on a server");
        supportsOption("inlineImages","insert base64 encoded images instead of referring to them");
        supportsOption("varint","Use varint encoding to serialize integer buffers");
        supportsOption("useSpecificBuffer=userkey1[=uservalue1][:buffername1],userkey2[=uservalue2][:buffername2]","uses specific buffers for unshared buffers attached to geometries having a specified user key/value. Buffer name *may* be specificed after ':' and will be set to uservalue by default. If no value is set then only the existence of a uservalue with key string is performed.");
        supportsOption("disableCompactBuffer","keep source types and do not try to optimize buffers size");
        supportsOption("disableStrictJson","do not clean string (to utf8) or floating point (should be finite) values");
    }

    virtual const char* className() const { return "OSGJS Reader / Writer"; }

    ReaderWriterJSON::OptionsStruct parseOptions(const osgDB::ReaderWriter::Options* options) const
    {
        OptionsStruct localOptions;

        if (options)
        {
            osg::notify(NOTICE) << "options " << options->getOptionString() << std::endl;
            std::istringstream iss(options->getOptionString());
            std::string opt;
            while (iss >> opt)
            {
                // split opt into pre= and post=
                std::string pre_equals;
                std::string post_equals;

                size_t found = opt.find("=");
                if(found!=std::string::npos)
                {
                    pre_equals = opt.substr(0,found);
                    post_equals = opt.substr(found+1);
                }
                else
                {
                    pre_equals = opt;
                }

                if (pre_equals == "useExternalBinaryArray")
                {
                    localOptions.useExternalBinaryArray = true;
                }
                if (pre_equals == "mergeAllBinaryFiles")
                {
                    localOptions.mergeAllBinaryFiles = true;
                }
                if (pre_equals == "disableCompactBuffer")
                {
                    localOptions.disableCompactBuffer = true;
                }
                if (pre_equals == "disableStrictJson")
                {
                    localOptions.strictJson = false;
                }


                if (pre_equals == "inlineImages")
                {
                    localOptions.inlineImages = true;
                }
                if (pre_equals == "varint")
                {
                    localOptions.varint = true;
                }

                if (pre_equals == "resizeTextureUpToPowerOf2" && post_equals.length() > 0)
                {
                    int value = atoi(post_equals.c_str());
                    localOptions.resizeTextureUpToPowerOf2 = osg::Image::computeNearestPowerOfTwo(value);
                }

                if (pre_equals == "useSpecificBuffer" && !post_equals.empty())
                {
                    size_t stop_pos = 0, start_pos = 0;
                    while((stop_pos = post_equals.find(",", start_pos)) != std::string::npos) {
                        localOptions.useSpecificBuffer.push_back(post_equals.substr(start_pos,
                                                                                    stop_pos - start_pos));
                        start_pos = stop_pos + 1;
                        ++ stop_pos;
                    }
                    localOptions.useSpecificBuffer.push_back(post_equals.substr(start_pos,
                                                                                post_equals.length() - start_pos));
                }

            }
            if (!options->getPluginStringData( std::string ("baseLodURL" )).empty())
            {
                localOptions.baseLodURL = options->getPluginStringData( std::string ("baseLodURL" ));
            }
        }
        return localOptions;
    }

    virtual ReadResult readNode(const std::string& fileName, const Options* options) const;

    virtual WriteResult writeNode(const Node& node, const std::string& fileName, const osgDB::ReaderWriter::Options* options) const;
    virtual WriteResult writeNode(const Node& node, json_stream& fout, const osgDB::ReaderWriter::Options* options) const;
    virtual WriteResult writeNodeModel(const Node& node, json_stream& fout, const std::string& basename, const OptionsStruct& options) const;
};
