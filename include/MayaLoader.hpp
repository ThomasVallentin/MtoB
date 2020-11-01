//
// Created by Thomas Vallentin on 29/10/2020.
//

#ifndef MTOB_MAYALOADER_HPP
#define MTOB_MAYALOADER_HPP


#include "adapters/Mesh.hpp"

#include <iostream>
#include <utility>
#include <functional>

#include <maya/MItDag.h>
#include <maya/MDagPath.h>
#include <maya/MString.h>
#include <map>


class MayaLoader {
public:
    MayaLoader() {
        registerAdapter(MFn::kMesh, new MeshAdapter());
    }

    MStatus load(Scene &scene);
    bool registerAdapter(const MFn::Type &type, Adapter *adapter);

private:
    std::map<const MFn::Type, Adapter*> adapters;
};


#endif //MTOB_MAYALOADER_HPP
