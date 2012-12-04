#include <cmath>
#include "graphics.hpp"
#include "boost/tuple/tuple.hpp"

#define PI 3.14159265

namespace Ennovia {
    TileSceneNode::TileSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, int x, int y, Map* map, s32 id)
     : scene::ISceneNode(parent, mgr, id) {
        Material.Wireframe = false;
        Material.Lighting = false;

        Vertices[0] = video::S3DVertex(x,map->heightmap[x][y],y, 1,1,1,
                        video::SColor(255,255,255,255), 0, 1);
        Vertices[1] = video::S3DVertex(x,map->heightmap[x][y+1],y+1, 1,1,0,
                        video::SColor(255,255,255,255), 1, 1);
        Vertices[2] = video::S3DVertex(x+1,map->heightmap[x+1][y+1],y+1, 0,1,0,
                        video::SColor(255,255,255,255), 1, 0);
        Vertices[3] = video::S3DVertex(x+1,map->heightmap[x+1][y],y, 0,1,1,
                        video::SColor(255,255,255,255), 0, 0);
        Box.reset(Vertices[0].Pos);
        for (s32 i=1; i<4; ++i)
            Box.addInternalPoint(Vertices[i].Pos);
    }
    void TileSceneNode::OnRegisterSceneNode()  {
        if (IsVisible)
            SceneManager->registerNodeForRendering(this);
        ISceneNode::OnRegisterSceneNode();
    }
    void TileSceneNode::render() {
        u16 indices[] = { 0,1,2, 2,3,0 };
        video::IVideoDriver* driver = SceneManager->getVideoDriver();

        driver->setMaterial(Material);
        driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
        driver->drawVertexPrimitiveList(&Vertices[0], 4, &indices[0], 2, video::EVT_STANDARD, scene::EPT_TRIANGLES, video::EIT_16BIT);
    }
    const core::aabbox3d<f32>& TileSceneNode::getBoundingBox() const { return  Box; }
    u32 TileSceneNode::getMaterialCount() const { return 1; }
    video::SMaterial& TileSceneNode::getMaterial(u32 i) { return Material; }

    MapSceneNode::MapSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, Map* map, s32 id)
     : scene::ISceneNode(parent, mgr, id) {
        for(int x=0;x<map->width;x++) {
            for(int y=0;y<map->height;y++) {
                TileSceneNode* tileNode = Tiles[std::make_pair(x,y)] = new TileSceneNode(this,mgr,x,y,map,y*map->width+x);
                tileNode->setMaterialTexture(0, getTexture(map->getTile(x,y).type->getTexturePath(),mgr->getVideoDriver()));
                if(x == 0 && y == 0) Box.reset(tileNode->getBoundingBox());
                else Box.addInternalBox(tileNode->getBoundingBox());
            }
        }
    }

    MapSceneNode::~MapSceneNode() {
        for(std::map<std::pair<int,int>, TileSceneNode*>::iterator i = Tiles.begin(); i!= Tiles.end(); ++i) {
            i->second->drop();
        }
    }

    ITexture* MapSceneNode::getTexture(std::string path, video::IVideoDriver* driver) {
        ITexture* tex = Textures[path];
        return tex ? tex : Textures[path] = driver->getTexture(path.c_str());
    }
    const core::aabbox3d<f32>& MapSceneNode::getBoundingBox() const { return  Box; }

    void MapSceneNode::setTriangleSelector(ITriangleSelector* selector) {
        for(std::map<std::pair<int,int>, TileSceneNode*>::iterator i = Tiles.begin(); i!= Tiles.end(); ++i) {
            i->second->setTriangleSelector(selector);
        }
    }

}
