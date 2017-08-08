//    copyright: 'Cedric Pinson cedric@plopbyte.com'
#include "ReaderWriterJSON.h"

using WriteResult = ReaderWriterJSON::WriteResult;

WriteResult ReaderWriterJSON::writeNode(const Node& node,
                                        const std::string& fileName,
                                        const osgDB::ReaderWriter::Options* options) const
{
    std::string ext = osgDB::getFileExtension(fileName);
    if (!acceptsExtension(ext)) return WriteResult::FILE_NOT_HANDLED;


    OptionsStruct _options = parseOptions(options);
    json_stream fout(fileName, _options.strictJson);

    if(fout) {
        WriteResult res = writeNodeModel(node, fout, osgDB::getNameLessExtension(fileName), _options);
        return res;
    }
    return WriteResult("Unable to open file for output");
}

WriteResult ReaderWriterJSON::writeNode(const Node& node,
                                        json_stream& fout,
                                        const osgDB::ReaderWriter::Options* options) const
{
    if (!fout) {
        return WriteResult("Unable to write to output stream");
    }

    OptionsStruct _options;
    _options = parseOptions(options);
    return writeNodeModel(node, fout, "stream", _options);
}

WriteResult ReaderWriterJSON::writeNodeModel(const Node& node,
                                             json_stream& fout,
                                             const std::string& basename,
                                             const OptionsStruct& options) const
{
    // process regular model
    osg::ref_ptr<osg::Node> model = osg::clone(&node);

    if(!options.disableCompactBuffer) {
        CompactBufferVisitor compact;
        model->accept(compact);
    }

    WriteVisitor writer;
    try {
        //osgDB::writeNodeFile(*model, "/tmp/debug_osgjs.osg");
        writer.setBaseName(basename);
        writer.useExternalBinaryArray(options.useExternalBinaryArray);
        writer.mergeAllBinaryFiles(options.mergeAllBinaryFiles);
        writer.setInlineImages(options.inlineImages);
        writer.setMaxTextureDimension(options.resizeTextureUpToPowerOf2);
        writer.setVarint(options.varint);
        writer.setBaseLodURL(options.baseLodURL);
        for(std::vector<std::string>::const_iterator specificBuffer = options.useSpecificBuffer.begin() ;
            specificBuffer != options.useSpecificBuffer.end() ; ++ specificBuffer) {
            writer.addSpecificBuffer(*specificBuffer);
        }
        model->accept(writer);
        if (writer._root.valid()) {
            writer.write(fout);
            return WriteResult::FILE_SAVED;
        }
    } catch (...) {
        osg::notify(osg::FATAL) << "can't save osgjs file" << std::endl;
        return WriteResult("Unable to write to output stream");
    }
    return WriteResult("Unable to write to output stream");
}
