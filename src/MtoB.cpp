////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//
// Produces the MEL command "BounceRender".
//
//
////////////////////////////////////////////////////////////////////////

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
    MRenderUtil::getCommonRenderSettings(commonRenderSettings);
    bounceRenderSettings = RenderGlobalsData::getBounceRenderGlobalsData();

    if (MRenderView::startRender(commonRenderSettings.width, commonRenderSettings.height, doNotClearBackground) != MS::kSuccess) {
        setResult("[MtoB] ERROR: Something went wrong while starting render.");
        return MS::kFailure;
    }

    unsigned int pixelCount = commonRenderSettings.width * commonRenderSettings.height;

    status = initializeRayTracer();

    CHECK_MSTATUS(status)

    raytracer->render(scene);

    // Create a pixel array that will be passed to the RenderView
    RV_PIXEL *pixels = new RV_PIXEL[pixelCount];
    for (int i = 0; i < commonRenderSettings.height; i++)
        for (int j = 0; j < commonRenderSettings.width; j++) {
            size_t pixelId = (commonRenderSettings.height - 1 - i) * commonRenderSettings.width + j;
            pixels[pixelId].r = raytracer->pixels()[(i * commonRenderSettings.width + j) * 3] * 255.0f;
            pixels[pixelId].g = raytracer->pixels()[(i * commonRenderSettings.width + j) * 3 + 1] * 255.0f;
            pixels[pixelId].b = raytracer->pixels()[(i * commonRenderSettings.width + j) * 3 + 2] * 255.0f;
            pixels[pixelId].a = 255.0f;
        }

    if (MRenderView::updatePixels(0, commonRenderSettings.width - 1, 0, commonRenderSettings.height - 1, pixels) != MS::kSuccess) {
        setResult("[MtoB] ERROR: Something went wrong while updating pixels.");
        delete[] pixels;
        return MS::kFailure;
    }

    delete[] pixels;

    // Force the Render View to refresh the display of the affected region.
    if (MRenderView::refresh(0, commonRenderSettings.width - 1, 0, commonRenderSettings.height - 1) != MS::kSuccess) {
        setResult("[MtoB] ERROR: Something went wrong while refreshing pixels.");
        return MS::kFailure;
    }

    // Inform the Render View that we have completed rendering the entire image.
    if (MRenderView::endRender() != MS::kSuccess) {
        setResult("[MtoB] ERROR: Something went wrong while ending render.");
        return MS::kFailure;
    }

    setResult("[MtoB] SUCCESS: completed.");
    return status;
}

MStatus BounceRender::initializeRayTracer() {
    MStatus status = MS::kSuccess;

    // Get rendered camera
    M3dView curView = M3dView::active3dView();
    MDagPath camDagPath;
    curView.getCamera(camDagPath);
    MFnCamera fnCamera(camDagPath);

    raytracer = new RayTracer(0.0001, 100000,
                              bounceRenderSettings.minSamples,
                              bounceRenderSettings.maxSamples);

    // Threading
    raytracer->setThreadCount(bounceRenderSettings.threadCount);

    // Sampler
    if (bounceRenderSettings.sampler == SamplerType::ADAPTIVE)
        raytracer->setSampler(new HierarchicalAdaptive(bounceRenderSettings.adaptiveThreshold));
    else
        raytracer->setSampler(new RandomSampler());

    scene = new Scene();

    // Accelerator
    if (bounceRenderSettings.accelerator == AcceleratorType::BOUNDING_VOLUME_HIERARCHY)
        scene->accelerator = new BVH();

    // Camera
    Matrix4 camToWorld;
    status = getMatrix(camDagPath, camToWorld);

    CHECK_MSTATUS(status)

    const Transform *camTransform = new Transform(camToWorld, camToWorld.getInversed());
    Camera cam(camTransform, (float) fnCamera.focalLength(), FilmGate::Film35mm);

    // DoF
    cam.focusDistance = (float) fnCamera.focusDistance();
    cam.apertureRadius = fnCamera.isDepthOfField() ? (float) fnCamera.horizontalFilmAperture() / 10.0f : 0.0f;
    cam.setResolution(commonRenderSettings.width, commonRenderSettings.height);

    raytracer->setCamera(cam);

    // Load scene content
    MayaLoader loader;
    loader.load(*scene);

    Light *gLight = new GradientLight();
    gLight->intensity = .5f;

    scene->addLight(gLight);

    return status;
}
