#include "PathNode.h"

#include "PathItem.h"
#include "IntersectionScene.h"

PathNode::PathNode(int id, IntersectionScene& intersection)
    : id_(id), intersection_(intersection)
{

}

PathNode::PathNode(IntersectionScene& intersection)
    : id_(-1), intersection_(intersection)
{

}

PathNode::~PathNode()
{
    clearInPaths();
    clearOutPaths();
}

void PathNode::registerOutPath(int id, PathItem* path)
{
    if (id < 0)
        qWarning("Wrong path");

    outPaths_[id] = path;
}

void PathNode::registerInPath(int id, PathItem* path)
{
    if (id < 0)
        qWarning("Wrong path");

    inPaths_[id] = path;
}

void PathNode::unregisterOutPath(int id, PathItem* path)
{
    if (id < 0)
        qWarning("Wrong path");

    if (outPaths_.count(id))
        outPaths_.erase(id);
}

void PathNode::unregisterInPath(int id, PathItem* path)
{
    if (id < 0)
        qWarning("Wrong path");

    if (inPaths_.count(id))
        inPaths_.erase(id);
}

void PathNode::clearInPaths()
{
    for (auto&[id, path]: inPaths_) {
        path->unsetBeginNode();
    }
}

void PathNode::clearOutPaths()
{
    for (auto&[id, path]: outPaths_) {
        path->unsetEndNode();
    }
}

bool PathNode::load(QXmlStreamReader& xmlStream)
{
    if (!xmlStream.isStartElement() ||
            xmlStream.name() != "path-node")
        return false;
    int id = xmlStream.attributes().value("id").toInt();
    id_ = id;

    while (xmlStream.readNextStartElement()) {
        if (xmlStream.name() == "in") {
            loadInPaths(xmlStream);
        } else if (xmlStream.name() == "out") {
            loadOutPaths(xmlStream);
        } else {
            xmlStream.skipCurrentElement();
        }
    }
    return true;
}

void PathNode::save(QXmlStreamWriter& xmlStream) const
{
    xmlStream.writeStartElement("path-node");
    xmlStream.writeAttribute("id", QString::number(getId()));

    xmlStream.writeStartElement("in");
    for (auto&[id, path]: inPaths_) {
        xmlStream.writeTextElement("id", QString::number(id));
    }
    xmlStream.writeEndElement();

    xmlStream.writeStartElement("out");
    for (auto&[id, path]: outPaths_) {
        xmlStream.writeTextElement("id", QString::number(id));
    }
    xmlStream.writeEndElement();

    xmlStream.writeEndElement();
}

bool PathNode::loadInPaths(QXmlStreamReader& xmlStream)
{
    auto ids = loadPathsId(xmlStream);
    for(int pathId: ids) {
        intersection_.connectPathToNode(pathId, getId());
    }
    return true;
}

bool PathNode::loadOutPaths(QXmlStreamReader& xmlStream)
{
    auto ids = loadPathsId(xmlStream);
    for(int pathId: ids) {
        intersection_.connectNodeToPath(getId(), pathId);
    }
    return true;
}

std::vector<int> PathNode::loadPathsId(QXmlStreamReader& xmlStream)
{
    std::vector<int> ids;
    while (xmlStream.readNextStartElement()) {
        if (xmlStream.name() == "id") {
            QString strId = xmlStream.readElementText();
            ids.push_back(strId.toInt());
        } else {
            xmlStream.skipCurrentElement();
        }
    }
    return ids;
}
