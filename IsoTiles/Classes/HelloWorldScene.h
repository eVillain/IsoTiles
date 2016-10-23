#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "TileMap.h"

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init() override;
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    virtual void update(float delta) override;

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
private:
    void onMouseMove(cocos2d::EventMouse* event);
    void onMouseDown(cocos2d::EventMouse* event);
    void onMouseUp(cocos2d::EventMouse* event);
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;

    cocos2d::EventListenerMouse* m_mouseListener;
    cocos2d::EventListenerKeyboard* m_keyListener;
    cocos2d::RefPtr<TileMap> m_tileMap;
    cocos2d::Vec2 m_inputDirection;
    cocos2d::Vec2 m_viewPosition;
};

#endif // __HELLOWORLD_SCENE_H__
