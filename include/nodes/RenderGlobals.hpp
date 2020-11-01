//
// Created by Thomas Vallentin on 01/11/2020.
//

#ifndef MTOB_RENDERGLOBALS_HPP
#define MTOB_RENDERGLOBALS_HPP


#include <maya/MPxNode.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>


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

enum AcceleratorType {
    NONE = 0,
    BVH
};

enum SamplerType {
    RANDOM_JITTER = 0,
    ADAPTIVE
};

#endif //MTOB_RENDERGLOBALS_HPP
