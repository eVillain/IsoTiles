#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    m_tileMap = new TileMap(64, 64, 32);
    addChild(m_tileMap);

//    const cocos2d::Size screen = cocos2d::Director::getInstance()->getVisibleSize();
//    m_viewPosition = screen*0.5;
    
    m_mouseListener = cocos2d::EventListenerMouse::create();
    m_mouseListener->onMouseMove = CC_CALLBACK_1(HelloWorld::onMouseMove, this);
    m_mouseListener->onMouseDown = CC_CALLBACK_1(HelloWorld::onMouseDown, this);
    m_mouseListener->onMouseUp = CC_CALLBACK_1(HelloWorld::onMouseUp, this);
    m_keyListener = cocos2d::EventListenerKeyboard::create();
    m_keyListener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
    m_keyListener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);

    getEventDispatcher()->addEventListenerWithFixedPriority(m_mouseListener, 1);
    getEventDispatcher()->addEventListenerWithFixedPriority(m_keyListener, 1);
    
    scheduleUpdate();
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
}

void HelloWorld::update(float delta)
{
//    cocos2d::Vec2 viewPos = m_tileMap->getPosition();
    m_viewPosition += cocos2d::Vec2(-m_inputDirection.x, m_inputDirection.y)*delta*600.0;
    m_tileMap->setPosition(m_viewPosition);
}


void HelloWorld::onMouseMove(cocos2d::EventMouse* event)
{
//    cocos2d::Vec2 screenPos = event->getLocationInView();
}

void HelloWorld::onMouseDown(cocos2d::EventMouse* event)
{
    cocos2d::Vec2 screenPos = event->getLocation() + cocos2d::Vec2(-m_viewPosition.x, m_viewPosition.y);
    cocos2d::Vec2 isoPos = m_tileMap->screenToIso(screenPos);
    cocos2d::Vec2 isoToScreenPos = m_tileMap->isoToScreen(cocos2d::Vec3(isoPos.x, isoPos.y, 0));

    int isoX = isoPos.x;
    int isoY = isoPos.y;
    
    cocos2d::log("screen x: %f, y: %f, isoPos x: %f, y: %f", screenPos.x, screenPos.y, isoPos.x, isoPos.y);
    cocos2d::log("iso x: %i, y: %i, isoScrnPos x: %f, y: %f", isoX, isoY, isoToScreenPos.x, isoToScreenPos.y);

    auto tile = m_tileMap->getTile(isoX, isoY);
    if (tile)
    {
//        tile->getSprite()->setColor(cocos2d::Color3B::ORANGE);
        cocos2d::log("tile z: %f", tile->getSprite()->getPositionZ());
    }
}

void HelloWorld::onMouseUp(cocos2d::EventMouse* event)
{
    
}

void HelloWorld::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    cocos2d::Vec2 direction;
    switch(keyCode)
    {
        case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_A:
            m_inputDirection.x -= 1.0f;
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_D:
            m_inputDirection.x += 1.0f;
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_W:
            m_inputDirection.y -= 1.0f;
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_S:
            m_inputDirection.y += 1.0f;
            break;
        default:
            break;
    }
}

void HelloWorld::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    switch(keyCode)
    {
        case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_A:
            m_inputDirection.x += 1.0f;
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_D:
            m_inputDirection.x -= 1.0f;
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_W:
            m_inputDirection.y += 1.0f;
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_S:
            m_inputDirection.y -= 1.0f;
            break;
        default:
            break;
    }
}

