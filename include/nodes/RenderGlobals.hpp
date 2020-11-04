//
// Created by Thomas Vallentin on 01/11/2020.
//

#ifndef MTOB_RENDERGLOBALS_HPP
#define MTOB_RENDERGLOBALS_HPP


#include "RenderGlobalsData.hpp"

#include <maya/MPxNode.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>

#include <maya/MFnDependencyNode.h>
#include <maya/MSelectionList.h>


class RenderGlobalsNode : public MPxNode {
public:
    RenderGlobalsNode() = default;
    ~RenderGlobalsNode();
    static void* creator();

    static MStatus initialize();
    MStatus compute(const MPlug &, MDataBlock &) override;

public:
    // Sampling
    static MObject minSamplesAttr;
    static MObject maxSamplesAttr;
    static MObject samplerAttr;
    static MObject adaptiveThresholdAttr;

    // Ray Depth
    static MObject maxDepthAttr;
    static MObject diffuseDepthAttr;
    static MObject specularDepthAttr;
    static MObject transmissionDepthAttr;

    // Misc
    static MObject acceleratorAttr;
    static MObject threadCountAttr;

    static MString name;
    static MTypeId id;
};


#endif //MTOB_RENDERGLOBALS_HPP
