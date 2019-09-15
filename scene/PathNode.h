#ifndef PATHNODE_H
#define PATHNODE_H

#include <map>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

class PathItem;
class IntersectionScene;

class PathNode
{
public:
    explicit PathNode(int id, IntersectionScene& intersection);
    explicit PathNode(IntersectionScene& intersection);
    ~PathNode();

    int getId() const { return id_; }
    void setId(int id) { id_ = id; }

    void registerOutPath(int id, PathItem* path);
    void registerInPath(int id, PathItem* path);
    void unregisterOutPath(int id, PathItem* path);
    void unregisterInPath(int id, PathItem* path);

    void clearInPaths();
    void clearOutPaths();

    bool load(QXmlStreamReader& xmlStream);
    void save(QXmlStreamWriter& xmlStream) const;

private:
    bool loadInPaths(QXmlStreamReader& xmlStream);
    bool loadOutPaths(QXmlStreamReader& xmlStream);
    std::vector<int> loadPathsId(QXmlStreamReader& xmlStream);

private:
    int id_ = -1;
    IntersectionScene& intersection_;
    std::map<int, PathItem*> inPaths_;
    std::map<int, PathItem*> outPaths_;
};

#endif // PATHNODE_H
