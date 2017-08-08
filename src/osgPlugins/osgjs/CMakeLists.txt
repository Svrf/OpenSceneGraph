SET(TARGET_SRC
    Animation.cpp
    Base64.cpp
    JSON_Objects.cpp
    WriteVisitor.cpp
    ReaderJSON.cpp
    WriterJSON.cpp
    ReaderWriterJSON.cpp
)

SET(TARGET_H
    Adaptor.h
    Animation.h
    Base64.h
    CompactBufferVisitor.h
    json.hpp
    JSON_Objects.h
    json_stream.h
    utf8_string.h
    WriteVisitor.h
    ReaderWriterJSON.h
)

IF($ENV{SKETCHFAB_BUILD})
    ADD_DEFINITIONS(-DUSE_COMPRESSED_CHANNELS)
ENDIF()

#### end var setup  ###
SET(TARGET_ADDED_LIBRARIES
    osgAnimation
    osgSim)

SETUP_PLUGIN(osgjs)
