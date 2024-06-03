#pragma once

#include <modules.hpp>

struct ConfigData
{
    std::string language = "en";
    Mcpack::PackManifest manifest = Mcpack::PackManifest(
        "[$ResourceName]", "Generate by the MCAllin.\n[$ResourceName]",
        { 0, 1, 0 }, "[$ResourceName]"
    );
    int filterAttribute = 0;
    Version filterVersion = Version(0, 0, 0);
    std::string exportPath = "[$InstalledPath]";
    int maxSize[2] = { 480, 270 };
    int maxCommandCount = 9000;
    int maxFrameCount = 200;
    Plane plane = Plane::XY_Z;
    bool isFunctionPack = false;
    bool useNewExecute = true;
    bool detachFrame = true;
    bool compress = true;
};
