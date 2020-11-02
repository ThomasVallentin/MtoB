//
// Created by Thomas Vallentin on 30/10/2020.
//

#ifndef MTOB_ADAPTER_HPP
#define MTOB_ADAPTER_HPP


#include <core/Scene.hpp>

#include <maya/MItDag.h>
#include <maya/MStatus.h>


class Adapter {
public:
    Adapter() = default;

    virtual MStatus load(MItDag &iter, Scene &scene) = 0;

    const std::string name = "Adapter";
};


#endif //MTOB_ADAPTER_HPP
