//
// Created by Thomas Vallentin on 27/10/2020.
//

#ifndef MTOB_PLUGIN_HPP
#define MTOB_PLUGIN_HPP

#include "MtoB.hpp"

#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>

//
// Description:
//		register the command
//
MStatus initializePlugin(MObject obj) {
    MFnPlugin plugin(obj, "Thomas Vallentin", "0.0.0");
    MStatus stat;
    stat = plugin.registerCommand(BounceRender::cmdName,
                                  BounceRender::creator,
                                  BounceRender::newSyntax);
    if (!stat)
        stat.perror("Failed to register command \"MtoB::bounceRender\"");

    MGlobal::executePythonCommand("import bounceRenderer; bounceRenderer.register_bounce_renderer()");

    return stat;
}

//
// Description:
//		unregister the command
//
MStatus uninitializePlugin(MObject obj) {
    MFnPlugin plugin(obj);
    MStatus stat;

    MGlobal::executePythonCommand("import bounceRenderer; bounceRenderer.deregister_bounce_renderer()");

    stat = plugin.deregisterCommand(BounceRender::cmdName);
    if (!stat)
        stat.perror("deregisterCommand");
    return stat;
}

#endif // MTOB_PLUGIN_HPP
