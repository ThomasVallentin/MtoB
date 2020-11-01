//
// Created by Thomas Vallentin on 29/10/2020.
//

#ifndef MTOB_MAYAUTILS_HPP
#define MTOB_MAYAUTILS_HPP

#include <maya/MPlug.h>
#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>

#include <maya/MMatrix.h>
#include <maya/MFnMatrixData.h>


bool getMatrix(const MDagPath &dagPath, MMatrix &matrix);


#endif //MTOB_MAYAUTILS_HPP
