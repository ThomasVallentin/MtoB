////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//
// Produces the MEL command "BounceRender".
//
//
////////////////////////////////////////////////////////////////////////

#include <thread>
#include "MtoB.hpp"

// ============================================================================
//	Flags declaration
// ============================================================================

#define kDoNotClearBackgroundFlag        "-b"
#define kDoNotClearBackgroundFlagLong    "-background"

// ============================================================================
//	Command declaration
// ============================================================================

const char *BounceRender::cmdName = "bounceRender";

void *BounceRender::creator() {
    return new BounceRender;
}

MSyntax BounceRender::newSyntax() {
    MSyntax syntax;
    syntax.addFlag(kDoNotClearBackgroundFlag, kDoNotClearBackgroundFlagLong);

    return syntax;
}

//
// Description:
//		Read the values of the additional flags for this command.
//
MStatus BounceRender::parseSyntax(MArgDatabase &argData) {
    // Get the flag values, otherwise the default values are used.
    doNotClearBackground = argData.isFlagSet(kDoNotClearBackgroundFlag);

    return MS::kSuccess;
}

//
//	Description:
//		Generates a simple procedural circular pattern to be sent to the
//		Render View.
//
//	Arguments:
//		x, y - coordinates in the current tile (the pattern is centred
//			   around (0,0) ).
//
//	Return Value:
//		An RV_PIXEL structure containing the colour of pixel (x,y).
//
MStatus BounceRender::doIt(const MArgList &args) {
    MStatus status = MS::kSuccess;

    // Check if the render view exists. It should always exist, unless
    // Maya is running in batch mode.
    if (!MRenderView::doesRenderEditorExist()) {
        setResult("Cannot render in batch render mode. "
                  "This command can only be run in interactive mode.");
        return MS::kFailure;
    }

    // Parse arguments
    MArgDatabase argData(syntax(), args);
    parseSyntax(argData);

    // Get the active camera
    M3dView curView = M3dView::active3dView();
    MDagPath camDagPath;
    curView.getCamera(camDagPath);

    std::cout << "Rendering with camera " << camDagPath.fullPathName().asChar() << std::endl;

    if (MRenderView::setCurrentCamera(camDagPath) != MS::kSuccess) {
        setResult("[MtoB] ERROR: Error occurred in setCurrentCamera.");
        return MS::kFailure;
    }

    // Get render settings data
    MCommonRenderSettingsData renderSettings;
    MRenderUtil::getCommonRenderSettings(renderSettings);

    RenderGlobalsData renderGlobals = RenderGlobalsData::getBounceRenderGlobalsData();

    if (MRenderView::startRender(renderSettings.width, renderSettings.height, doNotClearBackground) != MS::kSuccess) {
        setResult("[MtoB] ERROR: Something went wrong while starting render.");
        return MS::kFailure;
    }

    RayTracer tracer(0.0001, 100000, 8, 8);
    Accelerator *accelerator = new BVH;
    Scene scene(accelerator);

    unsigned int width = renderSettings.width;
    unsigned int height = renderSettings.height;
    unsigned int pixelCount = renderSettings.width * renderSettings.height;

    MayaLoader loader;
    loader.load(scene);

    initializeRayTracer(tracer, scene, width, height);

    tracer.render(&scene);

    // Create a pixel array that will be passed to the RenderView
    RV_PIXEL *pixels = new RV_PIXEL[pixelCount];
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++) {
            size_t pixelId = (height - 1 - i) * width + j;
            pixels[pixelId].r = tracer.pixels()[(i * width + j) * 3] * 255.0f;
            pixels[pixelId].g = tracer.pixels()[(i * width + j) * 3 + 1] * 255.0f;
            pixels[pixelId].b = tracer.pixels()[(i * width + j) * 3 + 2] * 255.0f;
            pixels[pixelId].a = 255.0f;
        }

    if (MRenderView::updatePixels(0, width - 1, 0, height - 1, pixels) != MS::kSuccess) {
        setResult("[MtoB] ERROR: Something went wrong while updating pixels.");
        delete[] pixels;
        return MS::kFailure;
    }

    delete[] pixels;

    // Force the Render View to refresh the display of the affected
    // region.
    //
    if (MRenderView::refresh(0, width - 1, 0, height - 1) != MS::kSuccess) {
        setResult("[MtoB] ERROR: Something went wrong while refreshing pixels.");
        return MS::kFailure;
    }

    // Inform the Render View that we have completed rendering the entire image.
    //
    if (MRenderView::endRender() != MS::kSuccess) {
        setResult("[MtoB] ERROR: Something went wrong while ending render.");
        return MS::kFailure;
    }

    setResult("[MtoB] SUCCESS: completed.");
    return status;
}

bool BounceRender::initializeRayTracer(RayTracer &tracer, Scene &scene,
                                       const unsigned int &width, const unsigned int &height) {
    MStatus status = MS::kSuccess;

    tracer.setThreadCount(8);
    tracer.setSampler(new RandomSampler());

    EnvironmentLight *gLight = new EnvironmentLight();
    gLight->intensity = 0.2f;
    scene.addLight(gLight);

    // Camera
    M3dView curView = M3dView::active3dView();
    MDagPath camDagPath;
    curView.getCamera(camDagPath);

    Matrix4 camToWorld;
    status = getMatrix(camDagPath, camToWorld);

    CHECK_MSTATUS(status)

    const Transform *camTransform = new Transform(camToWorld, camToWorld.getInversed());

    MFnCamera fnCamera(camDagPath);
    float focal = fnCamera.focalLength();

    Camera cam(camTransform, focal, FilmGate::Film35mm);
    cam.focusDistance = 1300;
    cam.apertureRadius = 0.0f;
    cam.setResolution(width, height);

    tracer.setCamera(cam);

    return true;
}
