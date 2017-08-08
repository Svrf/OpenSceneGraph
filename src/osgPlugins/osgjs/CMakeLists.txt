SET(TARGET_SRC
    Animation.cpp
    Base64.cpp
    JSON_Objects.cpp
    ReaderWriterJSON.cpp
    WriteVisitor.cpp)

SET(TARGET_H
    Adaptor.h
    Animation.h
    Base64.h
    CompactBufferVisitor.h
    JSON_Objects.h
    json_stream.h
    utf8_string.h
    WriteVisitor.h
)

IF($ENV{SKETCHFAB_BUILD})
    ADD_DEFINITIONS(-DUSE_COMPRESSED_CHANNELS)
ENDIF()

#### end var setup  ###
SET(TARGET_ADDED_LIBRARIES
    osgAnimation
    osgSim)

SETUP_PLUGIN(osgjs)
