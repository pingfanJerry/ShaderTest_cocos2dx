#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

enum
{
    SIZE_X = 256,
    SIZE_Y = 256,
};

//ShaderNode::ShaderNode()
//:m_center(vertex2(0.0f, 0.0f))
//,m_resolution(vertex2(0.0f, 0.0f))
//,m_time(0.0f)
//,m_uniformCenter(0)
//,m_uniformResolution(0)
//,m_uniformTime(0)
//{
//}
//
//ShaderNode* ShaderNode::shaderNodeWithVertex(const char *vert, const char *frag)
//{
//    ShaderNode *node = new ShaderNode();
//    node->initWithVertex(vert, frag);
//    node->autorelease();
//    
//    return node;
//}
//
//bool ShaderNode::initWithVertex(const char *vert, const char *frag)
//{
//    
//    loadShaderVertex(vert, frag);
//    
//    m_time = 0;
//    m_resolution = vertex2(SIZE_X, SIZE_Y);
//    
//    scheduleUpdate();
//    
//    setContentSize(CCSizeMake(SIZE_X, SIZE_Y));
//    setAnchorPoint(ccp(0.5f, 0.5f));
//    
//    return true;
//}
//
//void ShaderNode::loadShaderVertex(const char *vert, const char *frag)
//{
//    CCGLProgram *shader = new CCGLProgram();
//    shader->initWithVertexShaderFilename(vert, frag);
//    
//    shader->addAttribute("aVertex", kCCVertexAttrib_Position);
//    shader->link();
//    
//    shader->updateUniforms();
//    
//    m_uniformCenter = glGetUniformLocation(shader->getProgram(), "center");
//    m_uniformResolution = glGetUniformLocation(shader->getProgram(), "resolution");
//    m_uniformTime = glGetUniformLocation(shader->getProgram(), "time");
//    
//    this->setShaderProgram(shader);
//    
//    shader->release();
//}
//
//void ShaderNode::update(float dt)
//{
//    m_time += dt;
//}
//
//void ShaderNode::setPosition(const CCPoint &newPosition)
//{
//    CCNode::setPosition(newPosition);
//    CCPoint position = getPosition();
//    m_center = vertex2(position.x * CC_CONTENT_SCALE_FACTOR(), position.y * CC_CONTENT_SCALE_FACTOR());
//}
//
//void ShaderNode::draw()
//{
//    CC_NODE_DRAW_SETUP();
//    
//    float w = SIZE_X, h = SIZE_Y;
//    GLfloat vertices[12] = {0,0, w,0, w,h, 0,0, 0,h, w,h};
//    
//    //
//    // Uniforms
//    //
//    getShaderProgram()->setUniformLocationWith2f(m_uniformCenter, m_center.x, m_center.y);
//    getShaderProgram()->setUniformLocationWith2f(m_uniformResolution, m_resolution.x, m_resolution.y);
//    
//    // time changes all the time, so it is Ok to call OpenGL directly, and not the "cached" version
//    glUniform1f(m_uniformTime, m_time);
//    
//    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
//    
//    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
//    
//    glDrawArrays(GL_TRIANGLES, 0, 6);
//    
//    CC_INCREMENT_GL_DRAWS(1);
//}


//====================================
class SpriteMask : public CCSprite
{
public:
    ~SpriteMask();
    void setMask(const char *pMaskFile);
    static SpriteMask* create(const char *pszFileName);
    
private:
    
    bool initWithTexture(CCTexture2D* texture, const CCRect&  rect);
    void listenBackToForeground(CCObject *obj);
    void initProgram();
    void draw();
    
    CCTexture2D *_maskTexture;
    GLint _maskLocation;
    
    CCPoint offsetWithPosition;
    GLint _offsetLocation;
    
    bool _isSetMask;
    
};

SpriteMask::~SpriteMask()
{
    if(_maskTexture != NULL)
    {
        _maskTexture->release();
    }
    
    if(_isSetMask)
    {
        CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, EVNET_COME_TO_FOREGROUND);
    }
}

SpriteMask* SpriteMask::create(const char *pszFileName)
{
    SpriteMask* pRet = new SpriteMask();
    if (pRet && pRet->initWithFile(pszFileName))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

bool SpriteMask::initWithTexture(CCTexture2D* texture, const CCRect& rect)
{
    if( CCSprite::initWithTexture(texture, rect) )
    {
        CCSize s = getTexture()->getContentSizeInPixels();
        _maskTexture = NULL;
        _isSetMask = false;
        return true;
    }
    
    return false;
}


void SpriteMask::setMask(const char *pMaskFile)
{
    if(_isSetMask)    return;
    
    _maskTexture = CCTextureCache::sharedTextureCache()->addImage(pMaskFile);
    //_maskTexture->setAliasTexParameters();
    _maskTexture->retain();
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SpriteMask::listenBackToForeground), EVNET_COME_TO_FOREGROUND , NULL);
    
    initProgram();
    
    _isSetMask = true;
}

void SpriteMask::listenBackToForeground(CCObject *obj)
{
    setShaderProgram(NULL);
    initProgram();
}

void SpriteMask::initProgram()
{
    GLchar * fragSource = (GLchar*) CCString::createWithContentsOfFile(
                                                                       CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("example_mask.fsh"))->getCString();
    CCGLProgram* pProgram = new CCGLProgram();
    pProgram->initWithVertexShaderByteArray(ccPositionTextureColor_vert, fragSource);
    setShaderProgram(pProgram);
    pProgram->release();
    
    CHECK_GL_ERROR_DEBUG();
    
    getShaderProgram()->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
    getShaderProgram()->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
    getShaderProgram()->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
    
    CHECK_GL_ERROR_DEBUG();
    
    getShaderProgram()->link();
    
    CHECK_GL_ERROR_DEBUG();
    
    getShaderProgram()->updateUniforms();
    
    CHECK_GL_ERROR_DEBUG();
    
    _maskLocation = glGetUniformLocation(getShaderProgram()->getProgram(), "u_mask");
    _offsetLocation = glGetUniformLocation(getShaderProgram()->getProgram(), "v_offset");
    
    CHECK_GL_ERROR_DEBUG();
}

void SpriteMask::draw()
{
    if(!_isSetMask)   //沒有設定就直接用預設方法
    {
        CCSprite::draw();
        return;
    }
    
    CC_PROFILER_START_CATEGORY(kCCProfilerCategorySprite, "CCSprite - draw");
    
    CCAssert(!m_pobBatchNode, "If CCSprite is being rendered by CCSpriteBatchNode, CCSprite#draw SHOULD NOT be called");
    
    CC_NODE_DRAW_SETUP();
    
    ccGLBlendFunc( m_sBlendFunc.src, m_sBlendFunc.dst );
    
    if (m_pobTexture != NULL)
    {
        ccGLBindTexture2D( m_pobTexture->getName() );
    }
    else
    {
        ccGLBindTexture2D(0);
    }
    
    //mask
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _maskTexture->getName());
    glUniform1i(_maskLocation, 1);
    glUniform2f(_offsetLocation, offsetWithPosition.x, offsetWithPosition.y);
    
    //
    // Attributes
    //
    
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_PosColorTex );
    
#define kQuadSize sizeof(m_sQuad.bl)
    long offset = (long)&m_sQuad;
    
    // vertex
    int diff = offsetof( ccV3F_C4B_T2F, vertices);
    glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*) (offset + diff));
    
    // texCoods
    diff = offsetof( ccV3F_C4B_T2F, texCoords);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));
    
    // color
    diff = offsetof( ccV3F_C4B_T2F, colors);
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));
    
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    CHECK_GL_ERROR_DEBUG();
    
    
#if CC_SPRITE_DEBUG_DRAW == 1
    // draw bounding box
    CCPoint vertices[4]={
        ccp(m_sQuad.tl.vertices.x,m_sQuad.tl.vertices.y),
        ccp(m_sQuad.bl.vertices.x,m_sQuad.bl.vertices.y),
        ccp(m_sQuad.br.vertices.x,m_sQuad.br.vertices.y),
        ccp(m_sQuad.tr.vertices.x,m_sQuad.tr.vertices.y),
    };
    ccDrawPoly(vertices, 4, true);
#elif CC_SPRITE_DEBUG_DRAW == 2
    // draw texture box
    CCSize s = this->getTextureRect().size;
    CCPoint offsetPix = this->getOffsetPosition();
    CCPoint vertices[4] = {
        ccp(offsetPix.x,offsetPix.y), ccp(offsetPix.x+s.width,offsetPix.y),
        ccp(offsetPix.x+s.width,offsetPix.y+s.height), ccp(offsetPix.x,offsetPix.y+s.height)
    };
    ccDrawPoly(vertices, 4, true);
#endif // CC_SPRITE_DEBUG_DRAW
    
    CC_INCREMENT_GL_DRAWS(1);
    
    glActiveTexture(GL_TEXTURE0);//must set to texture0
    
    CC_PROFILER_STOP_CATEGORY(kCCProfilerCategorySprite, "CCSprite - draw");
}


//====================================

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)) )
    {
        return false;
    }

    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    SpriteMask* pSprite2 = SpriteMask::create("wall_77.png");
    pSprite2->setPosition( ccp(size.width/2 - 30, size.height/2) );
    this->addChild(pSprite2, 0);
    
    SpriteMask* pSprite3 = SpriteMask::create("wall_77.png");
    pSprite3->setMask("wall0.png");
    pSprite3->setPosition( ccp(size.width/2 + 30, size.height/2) );
    this->addChild(pSprite3, 0);
    
    return true;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
