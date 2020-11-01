//
// Created by Thomas Vallentin on 27/10/2020.
//

#ifndef MTOB_PLUGIN_HPP
#define MTOB_PLUGIN_HPP

#include "MtoB.hpp"
#include "nodes/RenderGlobals.hpp"

#include <maya/MFnPlugin.h>
#include <maya/MNodeMessage.h>
#include <maya/MGlobal.h>

//
// Description:
//		register MtoB plugin
//
MStatus initializePlugin(MObject obj) {
    MFnPlugin plugin(obj, "Thomas Vallentin", "0.0.0");
    MStatus status = MS::kSuccess;

    status = plugin.registerCommand(BounceRender::cmdName,
                                    BounceRender::creator,
                                    BounceRender::newSyntax);
    CHECK_MSTATUS(status)

    status = plugin.registerNode(RenderGlobalsNode::name,
    RenderGlobalsNode::id,
    RenderGlobalsNode::creator,
    RenderGlobalsNode::initialize,
    MPxNode::kDependNode);

    CHECK_MSTATUS(status)

    MGlobal::executePythonCommand("import bounceRenderer; bounceRenderer.create_render_globals_node()");
    MGlobal::executePythonCommand("import bounceRenderer; bounceRenderer.register_bounce_renderer()");

    return status;
}

//
// Description:
//		unregister MtoB plugin
//
MStatus uninitializePlugin(MObject obj) {
    MFnPlugin plugin(obj);
    MStatus status = MS::kSuccess;

    MGlobal::executePythonCommand("import bounceRenderer; bounceRenderer.delete_render_globals_node()");
    MGlobal::executePythonCommand("import bounceRenderer; bounceRenderer.deregister_bounce_renderer()");

    status = plugin.deregisterCommand(BounceRender::cmdName);
    CHECK_MSTATUS(status)

    status = plugin.deregisterNode(RenderGlobalsNode::id);

    return status;
}

#endif // MTOB_PLUGIN_HPP
