//
// Created by Thomas Vallentin on 01/11/2020.
//


#include "nodes/RenderGlobals.hpp"


MString RenderGlobalsNode::name("bounceRenderGlobals");
MTypeId RenderGlobalsNode::id( 0x00050000 );

// Sampling
MObject RenderGlobalsNode::minSamplesAttr;
MObject RenderGlobalsNode::maxSamplesAttr;
MObject RenderGlobalsNode::samplerAttr;
MObject RenderGlobalsNode::adaptiveThresholdAttr;

// Ray Depth
MObject RenderGlobalsNode::maxDepthAttr;
MObject RenderGlobalsNode::diffuseDepthAttr;
MObject RenderGlobalsNode::specularDepthAttr;
MObject RenderGlobalsNode::transmissionDepthAttr;

// Misc
MObject RenderGlobalsNode::threadCountAttr;
MObject RenderGlobalsNode::acceleratorAttr;


RenderGlobalsNode::~RenderGlobalsNode() {}

void *RenderGlobalsNode::creator() {
    return new RenderGlobalsNode;
}


MStatus RenderGlobalsNode::initialize() {
    MFnNumericAttribute nAttr;
    MFnEnumAttribute enumAttr;

    // Sampling

    minSamplesAttr = nAttr.create("minSamples", "mns", MFnNumericData::kInt, 8);
    nAttr.setMin(0);
    nAttr.setWritable(true);
    nAttr.setStorable(true);
    addAttribute(minSamplesAttr);

    maxSamplesAttr = nAttr.create("maxSamples", "mxs", MFnNumericData::kInt, 8);
    nAttr.setMin(0);
    nAttr.setWritable(true);
    nAttr.setStorable(true);
    addAttribute(maxSamplesAttr);

    samplerAttr = enumAttr.create("sampler", "splr", 0);
    enumAttr.addField("Random Jitter", SamplerType::RANDOM_JITTER);
    enumAttr.addField("Adaptive", SamplerType::ADAPTIVE);
    nAttr.setWritable(true);
    nAttr.setStorable(true);
    addAttribute(samplerAttr);

    adaptiveThresholdAttr = nAttr.create("adaptiveThreshold", "adpth", MFnNumericData::kFloat, 0.001);
    nAttr.setMin(0);
    nAttr.setWritable(true);
    nAttr.setStorable(true);
    addAttribute(adaptiveThresholdAttr);

    // Ray Depth

    maxDepthAttr = nAttr.create("maxDepth", "mxdpth", MFnNumericData::kInt, 3);
    nAttr.setMin(0);
    nAttr.setWritable(true);
    nAttr.setStorable(true);
    addAttribute(maxDepthAttr);

    diffuseDepthAttr = nAttr.create("diffuseDepth", "dfdpth", MFnNumericData::kInt, 1);
    nAttr.setMin(0);
    nAttr.setWritable(true);
    nAttr.setStorable(true);
    addAttribute(diffuseDepthAttr);

    specularDepthAttr = nAttr.create("specularDepth", "spdpth", MFnNumericData::kInt, 1);
    nAttr.setMin(0);
    nAttr.setWritable(true);
    nAttr.setStorable(true);
    addAttribute(specularDepthAttr);

    transmissionDepthAttr = nAttr.create("transmissionDepth", "trdpth", MFnNumericData::kInt, 3);
    nAttr.setMin(0);
    nAttr.setWritable(true);
    nAttr.setStorable(true);
    addAttribute(transmissionDepthAttr);

    // Misc

    acceleratorAttr = enumAttr.create("accelerator", "acc", 1);
    enumAttr.addField("None", AcceleratorType::NONE);
    enumAttr.addField("BVH", AcceleratorType::BOUNDING_VOLUME_HIERARCHY);
    nAttr.setWritable(true);
    nAttr.setStorable(true);
    addAttribute(acceleratorAttr);

    threadCountAttr = nAttr.create("threads", "thrds", MFnNumericData::kInt, 1);
    nAttr.setMin(1);
    nAttr.setWritable(true);
    nAttr.setStorable(true);
    addAttribute(threadCountAttr);

    return MS::kSuccess;
}

MStatus RenderGlobalsNode::compute(const MPlug &, MDataBlock &) {
    return MS::kSuccess;
}
