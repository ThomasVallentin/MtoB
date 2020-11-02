//
// Created by Thomas Vallentin on 01/11/2020.
//

#include "adapters/PointLight.hpp"
#include <shapes/Sphere.h>
#include <materials/Lambert.hpp>

MStatus PointLightAdapter::load(MItDag &iter, Scene &scene) {
    MStatus status = MS::kSuccess;

    MFnDependencyNode fn(iter.currentItem());

    MPlug intensityPlug = fn.findPlug("intensity", false);
    float intensity = intensityPlug.asFloat();

    MPlug colorPlug = fn.findPlug("color", false);
    float r = colorPlug.child(0).asFloat();
    float g = colorPlug.child(1).asFloat();
    float b = colorPlug.child(2).asFloat();

    std::cout << "Loading light with " << intensity <<  " intensity and color " << r << ", " << g << ", " << b << std::endl;

    Matrix4 lightToWorld;
    MDagPath dagpath;
    iter.getPath(dagpath);
    status = getMatrix(dagpath, lightToWorld);

    CHECK_MSTATUS(status)

    const Transform *lightTransform = new Transform(lightToWorld, lightToWorld.getInversed());

    scene.addLight(new PointLight(lightTransform, Color(r, g, b), intensity));

    return status;
}
