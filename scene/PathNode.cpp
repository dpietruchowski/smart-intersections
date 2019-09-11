#include "PathNode.h"

#include "PathItem.h"

PathNode::PathNode()
{

}

void PathNode::RegisterPrevPath(int id, PathItem* path)
{
    if (id < 0)
        qWarning("Wrong path");

    prevPaths_[id] = path;
}

void PathNode::RegisterNextPath(int id, PathItem* path)
{
    if (id < 0)
        qWarning("Wrong path");

    nextPaths_[id] = path;
}

void PathNode::UnregisterPrevPath(int id, PathItem* path)
{
    if (id < 0)
        qWarning("Wrong path");

    if (prevPaths_.count(id))
        prevPaths_.erase(id);
}

void PathNode::UnregisterNextPath(int id, PathItem* path)
{
    if (id < 0)
        qWarning("Wrong path");

    if (nextPaths_.count(id))
        nextPaths_.erase(id);
}
