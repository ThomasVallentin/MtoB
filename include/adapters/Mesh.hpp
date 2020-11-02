//
// Created by Thomas Vallentin on 30/10/2020.
//

#ifndef MTOB_MESH_HPP
#define MTOB_MESH_HPP


#include "Adapter.hpp"

#include <core/Scene.hpp>
#include <shapes/TriangleMesh.hpp>
#include <materials/Lambert.hpp>

#include <maya/MFnMesh.h>
#include <maya/MPointArray.h>
#include <maya/MIntArray.h>
#include <maya/MFloatPointArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MDagPath.h>


class MeshAdapter : public Adapter {
public:
    MeshAdapter() : name("MeshAdapter") {}

    MStatus load(MItDag &iter, Scene &scene) override;

    const std::string name;
};


#endif //MTOB_MESH_HPP
