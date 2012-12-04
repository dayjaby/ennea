#ifndef ENNOVIA_GRAPHICS_HPP
#define ENNOVIA_GRAPHICS_HPP

#include <irrlicht/irrlicht.h>
#include <map>
#include <vector>
#include <string>
#include "map.hpp"

namespace Ennovia {

    using namespace irr;
    using namespace core;
    using namespace scene;
    using namespace video;
    using namespace io;
    using namespace gui;


    template <class T>
    class GUIDataElement : public gui::IGUIElement {
    public:
        GUIDataElement (const T& data_, core::rect<s32> rect, IGUIEnvironment *environment, IGUIElement *parent)
            : IGUIElement(EGUIET_ELEMENT,environment,parent,0,rect), data(data_) {}
        const T& getData() { return data; }
        void setData(const T& data_) { data = data_; }
    private:
        T data;
    };

    class TileSceneNode : public scene::ISceneNode {
        core::aabbox3d<f32> Box;
        video::S3DVertex Vertices[4];
        video::SMaterial Material;
    public:
        TileSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, int x, int y, Map* map, s32 id);
        virtual void OnRegisterSceneNode();
        virtual void render();
        virtual const core::aabbox3d<f32>& getBoundingBox() const;
        virtual u32 getMaterialCount() const;
        virtual video::SMaterial& getMaterial(u32 i);
    };


    class MapSceneNode : public scene::ISceneNode {
        core::aabbox3d<f32> Box;
        std::map<std::string, video::ITexture*> Textures;
        video::ITexture* getTexture(std::string path, video::IVideoDriver* driver);
        std::map<std::pair<int,int>, TileSceneNode*> Tiles;
    public:
        MapSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, Map* map, s32 id);
        virtual ~MapSceneNode();
        virtual void render() {}
        virtual void setTriangleSelector(ITriangleSelector* selector);
        virtual const core::aabbox3d<f32>& getBoundingBox() const;
    };

}


#endif
