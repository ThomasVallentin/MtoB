//
// Created by Thomas Vallentin on 02/11/2020.
//

#include "RenderGlobalsData.hpp"


RenderGlobalsData RenderGlobalsData::getBounceRenderGlobalsData() {
    MStatus status = MS::kSuccess;
    RenderGlobalsData data;

    MSelectionList sel;
    status = sel.add("defaultBounceRenderGlobals");

    if (status != MS::kSuccess)
        return data;

    MObject renderGlobals;
    sel.getDependNode(0, renderGlobals);

    MFnDependencyNode fn(renderGlobals);
    MPlug plug;


    //  ==  Sampling  ==

    // Min Samples
    plug = fn.findPlug("minSamples", false);
    data.minSamples = plug.asInt();
    // Max Samples
    plug = fn.findPlug("maxSamples", false);
    data.maxSamples = plug.asInt();
    // Sampler
    plug = fn.findPlug("sampler", false);
    data.sampler = SamplerType(plug.asInt());
    // Adaptive Threshold
    plug = fn.findPlug("adaptiveThreshold", false);
    data.adaptiveThreshold = plug.asFloat();

    //  ==  Ray Depth  ==

    // Max Depth
    plug = fn.findPlug("maxDepth", false);
    data.maxDepth = plug.asInt();
    // Diffuse Depth
    plug = fn.findPlug("diffuseDepth", false);
    data.diffuseDepth = plug.asInt();
    // Specular Depth
    plug = fn.findPlug("specularDepth", false);
    data.specularDepth = plug.asInt();
    // Transmission Depth
    plug = fn.findPlug("transmissionDepth", false);
    data.transmissionDepth = plug.asInt();

    //  ==  Misc  ==

    // Accelerator
    plug = fn.findPlug("accelerator", false);
    data.accelerator = AcceleratorType(plug.asInt());
    // Thread Count
    plug = fn.findPlug("threadCount", false);
    data.threadCount = plug.asInt();

    return data;
}
