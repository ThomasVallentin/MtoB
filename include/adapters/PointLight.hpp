//
// Created by Thomas Vallentin on 01/11/2020.
//

#ifndef MTOB_POINTLIGHT_HPP
#define MTOB_POINTLIGHT_HPP


#include "Adapter.hpp"
#include "MayaUtils.hpp"

#include <lights/PointLight.hpp>

#include <maya/MFnDependencyNode.h>
#include <maya/MFnNumericData.h>
#include <maya/MPlug.h>


class PointLightAdapter : public Adapter {
public:
    PointLightAdapter() = default;

    MStatus load(MItDag &iter, Scene &scene) override;
};


#endif //MTOB_POINTLIGHT_HPP
