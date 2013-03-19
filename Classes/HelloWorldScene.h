#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

using namespace cocos2d;

//class ShaderNode : public CCNode
//{
//public:
//    ShaderNode();
//    
//    bool initWithVertex(const char *vert, const char *frag);
//    void loadShaderVertex(const char *vert, const char *frag);
//    
//    virtual void update(float dt);
//    virtual void setPosition(const CCPoint &newPosition);
//    virtual void draw();
//    
//    static ShaderNode* shaderNodeWithVertex(const char *vert, const char *frag);
//    
//private:
//    
//    ccVertex2F m_center;
//    ccVertex2F m_resolution;
//    float      m_time;
//    GLuint     m_uniformCenter, m_uniformResolution, m_uniformTime;
//};


class HelloWorld : public cocos2d::CCLayerColor
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
