//
// Created by Thomas Vallentin on 29/10/2020.
//

#ifndef MTOB_MAYAUTILS_HPP
#define MTOB_MAYAUTILS_HPP

#include <core/Matrix.hpp>

#include <maya/MPlug.h>
#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>

#include <maya/MMatrix.h>
#include <maya/MFnMatrixData.h>


MStatus getMatrix(const MDagPath &dagPath, MMatrix &matrix);
MStatus getMatrix(const MDagPath &dagPath, Matrix4 &matrix);

#endif // MTOB_MAYAUTILS_HPP
