//
// Created by Thomas Vallentin on 27/10/2020.
//

#ifndef MTOB_MTOB_HPP
#define MTOB_MTOB_HPP


// MtoB includes
#include "MayaLoader.hpp"
#include "MayaUtils.hpp"
#include "RenderGlobalsData.hpp"

// Bounce includes
#include <core/RayTracer.hpp>
#include <accelerators/BVH.hpp>
#include <samplers/RandomSampler.hpp>
#include <loaders/OBJLoader.hpp>

#include <lights/GradientLight.hpp>
#include <lights/PointLight.hpp>
#include <lights/DiscLight.hpp>

#include <materials/Lambert.hpp>

// Maya includes
#include <maya/MPxCommand.h>
#include <maya/MArgDatabase.h>
#include <maya/MSyntax.h>
#include <maya/MPlug.h>
#include <maya/MFnMatrixData.h>
#include <maya/MFnCamera.h>

#include <maya/M3dView.h>
#include <maya/MRenderView.h>
#include <maya/MRenderUtil.h>
#include <maya/MCommonRenderSettingsData.h>

#include <cmath>


class BounceRender : public MPxCommand
{
public:
    BounceRender() {};
    ~BounceRender() override {};

    MStatus	doIt ( const MArgList& ) override;

    static void*	creator();

    static MSyntax	newSyntax();
    MStatus parseSyntax (MArgDatabase &argData);

    static bool initializeRayTracer(RayTracer &tracer, Scene &scene, const unsigned int &width, const unsigned int &height);

    static const char * cmdName;

private:
    bool doNotClearBackground;
};


#endif // MTOB_MTOB_HPP
