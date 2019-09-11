#ifndef PATHNODE_H
#define PATHNODE_H

#include <map>

class PathItem;

class PathNode
{
public:
    PathNode();

    void RegisterPrevPath(int id, PathItem* path);
    void RegisterNextPath(int id, PathItem* path);
    void UnregisterPrevPath(int id, PathItem* path);
    void UnregisterNextPath(int id, PathItem* path);

private:
    std::map<int, PathItem*> prevPaths_;
    std::map<int, PathItem*> nextPaths_;
};

#endif // PATHNODE_H
