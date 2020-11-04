//
// Created by Thomas Vallentin on 29/10/2020.
//

#include "MayaLoader.hpp"

MStatus MayaLoader::load(Scene &scene) {
    MStatus status = MS::kSuccess;

    for (MItDag iter; !iter.isDone(); iter.next()) {
        MDagPath dagpath;

        // Get current dag path & ensure status is good
        CHECK_MSTATUS(iter.getPath(dagpath))

        if (!dagpath.isVisible())
            continue;

        auto adapterIter = adapters.find(dagpath.apiType());  // map iterator

        if (adapterIter == adapters.end()){
            std::cout << "Cannot find any Adapter for node type " << iter.currentItem().apiTypeStr() << std::endl;
            continue;
        }

        std::cout << "Loading object " << dagpath.fullPathName() << std::endl;
        status = adapterIter->second->load(iter, scene);

        if (status != MS::kSuccess) {
            std::cerr << "Failed to load object " << dagpath.fullPathName() << std::endl;
            return status;
        }
    }

    return status;
}

bool MayaLoader::registerAdapter(const MFn::Type &type, Adapter *adapter) {
    std::cout << "adapter " << adapter << std::endl;
    if (adapters[type]) {
        std::cerr << "Cannot register new adapter " << adapter->name << " for type " << type <<
                     " this type is already managed by adapter " << adapters[type]->name << std::endl;
        return false;
    }

    adapters[type] = adapter;

    return true;
}
