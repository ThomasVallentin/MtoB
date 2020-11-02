//
// Created by Thomas Vallentin on 29/10/2020.
//

#include "MayaUtils.hpp"

MStatus getMatrix(const MDagPath &dagPath, MMatrix &matrix) {
    MStatus status = MS::kSuccess;

    MFnDagNode fnCam(dagPath);
    MPlug plug = fnCam.findPlug("worldMatrix", false);

    if (status != MS::kSuccess) {
        return status; }

    MPlug matrixPlug = plug.elementByLogicalIndex( 0 );
    MObject matrixObject = matrixPlug.asMObject();

    MFnMatrixData worldMatrixData(matrixObject);
    matrix = worldMatrixData.matrix();

    return status;
}

MStatus getMatrix(const MDagPath &dagPath, Matrix4 &matrix) {
    MMatrix mat;
    MStatus status = getMatrix(dagPath, mat);

    if (status != MS::kSuccess)
        return status;

    for( int i=0 ; i<4 ; i++)
        for( int j=0 ; j<4 ; j++)
            matrix.m[i][j] = mat(i, j);

    return status;
}
