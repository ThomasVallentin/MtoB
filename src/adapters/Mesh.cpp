//
// Created by Thomas Vallentin on 30/10/2020.
//

#include "adapters/Mesh.hpp"

MStatus MeshAdapter::load(MItDag &iter, Scene &scene) {
    Material *material = new LambertMaterial(Color(.5, .5, .5));

    MDagPath dagpath;
    iter.getPath(dagpath);
    MFnMesh fnMesh(dagpath);

    // Init maya data structures

    MFloatPointArray mPoints;
    fnMesh.getPoints(mPoints, MSpace::kWorld);

    MFloatVectorArray mNormals;
    fnMesh.getVertexNormals(false, mNormals, MSpace::kWorld);

    MIntArray triangleCounts, mVertices;
    fnMesh.getTriangles(triangleCounts, mVertices);

    size_t triangleCount = mVertices.length() / 3;
    std::cout << "Loading " << triangleCount << " triangles, " << mPoints.length() << " points, " <<
              mNormals.length() << " normals, " <<
              mVertices.length() << " vertices..." << std::endl;

    // Copy Maya data to c++ arrays
    Point3 *points = new Point3[mPoints.length()];
    Vector3 *normals = new Vector3[mNormals.length()];
    int *vertexIds = new int[mVertices.length()];

    for (int i=0 ; i < mPoints.length() ; i++) {
        points[i] = Point3(mPoints[i].x, mPoints[i].y, mPoints[i].z);
    }

    for (int i=0 ; i < mNormals.length() ; i++) {
        normals[i] = Vector3(mNormals[i].x, mNormals[i].y, mNormals[i].z);
    }

    for (int i=0 ; i < mVertices.length() ; i++) {
        vertexIds[i] = (int)mVertices[i];
    }

    // Create MeshData & Triangles
    const Transform *t = Transform::Identity();
    const TriangleMeshData *meshData = new TriangleMeshData(*t,
                                                            triangleCount,
                                                            mPoints.length(),
                                                            vertexIds,
                                                            points,
                                                            vertexIds,
                                                            normals);

    for (int i=0 ; i < triangleCount ; i++) {
        scene.addShape(new Triangle(t, meshData, i, material));
    }

    return MStatus::kSuccess;
}
