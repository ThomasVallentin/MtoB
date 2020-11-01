//
// Created by Thomas Vallentin on 29/10/2020.
//

#include "MayaUtils.hpp"

bool getMatrix(const MDagPath &dagPath, MMatrix &matrix) {
    MStatus status = MS::kSuccess;

    MFnDagNode fnCam(dagPath);
    MPlug plug = fnCam.findPlug("worldMatrix", status);

    if (status != MS::kSuccess)
        return false;

    MPlug matrixPlug = plug.elementByLogicalIndex( 0 );
    MObject matrixObject = matrixPlug.asMObject();

    MFnMatrixData worldMatrixData(matrixObject);
    matrix = worldMatrixData.matrix();

    return true;
}