//
// Created by Thomas Vallentin on 02/11/2020.
//

#ifndef MTOB_RENDERGLOBALSDATA_HPP
#define MTOB_RENDERGLOBALSDATA_HPP


#include <maya/MStatus.h>
#include <maya/MSelectionList.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>


enum AcceleratorType {
    NONE,
    BOUNDING_VOLUME_HIERARCHY
};

enum SamplerType {
    RANDOM_JITTER,
    ADAPTIVE
};


struct RenderGlobalsData {
    RenderGlobalsData() = default;
    RenderGlobalsData(const unsigned int &minSamples, const  unsigned int &maxSamples,
                      const SamplerType &sampler, const float &adaptiveThreshold,
                      const unsigned int &maxDepth, const unsigned int &diffuseDepth,
                      const unsigned int &specularDepth, const unsigned int &transmissionDepth,
                      const AcceleratorType &accelerator, const unsigned int &threadCount) :
            minSamples(minSamples), maxSamples(maxSamples),
            sampler(sampler), adaptiveThreshold(adaptiveThreshold),
            maxDepth(maxDepth), diffuseDepth(diffuseDepth),
            specularDepth(specularDepth), transmissionDepth(transmissionDepth),
            accelerator(accelerator), threadCount(threadCount) {}

    static RenderGlobalsData getBounceRenderGlobalsData();

    // Sampling
    unsigned int minSamples = 8;
    unsigned int maxSamples = 8;
    SamplerType sampler = SamplerType::RANDOM_JITTER;
    float adaptiveThreshold = 0.001;

    // Ray depth
    unsigned int maxDepth = 3;
    unsigned int diffuseDepth = 1;
    unsigned int specularDepth = 1;
    unsigned int transmissionDepth = 3;

    // Misc
    AcceleratorType accelerator = AcceleratorType::BOUNDING_VOLUME_HIERARCHY;
    unsigned int threadCount = 8;
};


#endif //MTOB_RENDERGLOBALSDATA_HPP
