#include <ChibiEngine/Render/ScreenSystem.h>

#include <ChibiEngine/Log/LoggingSystem.h>
#include <ChibiEngine/Resource/FileSystem.h>
#include <ChibiEngine/UserInterface/LookAndFeel.h>
#include <ChibiEngine/Common/ContainerUtils.h>

using namespace game;
using namespace std;

const static string LOGGER_NAME = "ScreenSystem";

struct ScreenSystem::Frame {
    Frame(const glm::ivec2& size);
    ~Frame();

    Texture renderTexture;
    GLuint frameBuffer, depthBuffer;
};

ScreenSystem::Frame::Frame(const glm::ivec2& size):
        renderTexture(size.x>0?size.x:Game::getScreen()->getWidth(),
                size.y>0?size.y:Game::getScreen()->getHeight()){

    glGenFramebuffers(1, &frameBuffer);
    glGenRenderbuffers(1, &depthBuffer);

    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16,
            renderTexture.getWidth(),
            renderTexture.getHeight());
}

ScreenSystem::Frame::~Frame(){
    glDeleteRenderbuffers(1, &depthBuffer);
    glDeleteFramebuffers(1, &frameBuffer);
}

ScreenSystem::ScreenSystem(const ScreenType* screen):
    logger(Game::getLoggingSystem()->createLogger(LOGGER_NAME)),
	lookAndFeel(),
	aspectRatio(0),
    frames(),
    frameNum(-1){

    logger.debug("Enter ScreenSystem <init>");
    
    if(screen->width>0 && screen->height>0){
        resize(static_cast<unsigned int>(screen->width),
                static_cast<unsigned int>(screen->height));
    }
    blendingEnabled = screen->blending;
    depthTestEnabled = screen->depthTest;

    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    if(depthTestEnabled){
        glEnable(GL_DEPTH_TEST);
    }else{
        glDisable(GL_DEPTH_TEST);
    }
    glDepthFunc(GL_LEQUAL);

    if(blendingEnabled){
        glEnable(GL_BLEND);
    }else{
        glDisable(GL_BLEND);
    }
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glFrontFace(GL_CW);

#ifndef MY_ANDROID
	glEnable(GL_PROGRAM_POINT_SIZE);
#endif

    glGenBuffers(2, buffers);

    GLfloat verts[] = {
            -1, -1, 0, 0,1-1,
            -1, 1, 0, 0, 1-0,
            1, -1, 0, 1, 1-1,
            1, 1, 0, 1, 1-0,
    };

    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER,(5*4)*sizeof(GLfloat), verts, GL_STATIC_DRAW);


    GLubyte inds[] = {
            0,1,2,1,3,2
    };

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,(6)*sizeof(GLubyte), inds, GL_STATIC_DRAW);

    logger.info("Init ScreenSystem -Ok");
}

ScreenSystem::~ScreenSystem(){
    glDeleteBuffers(2, buffers);
}

void ScreenSystem::resize(unsigned int width, unsigned int height){
    logger.debug("Enter ScreenSystem resize %d %d", width, height);
    
    this->width=width;
    this->height=height;
    aspectRatio = static_cast<float>(width)/height;
    glViewport(0,0,width,height);

    logger.trace("ScreenSystem resize - Ok");
}


void ScreenSystem::Clear(){
    glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);
}

void ScreenSystem::setLookAndFeel(LookAndFeel laf){
    lookAndFeel = move(laf);
}

const ScreenSystem::Frame* ScreenSystem::getFrame(ScreenFrameId num){
    assert(num<=frameNum);
    assert(num>=0);

    const ScreenSystem::FrameRequest& f = frames[num];
    if(!f.frame){
        frames[num].frame.reset(new ScreenSystem::Frame(f.size));
    }

    return f.frame.get();
}

const Texture& ScreenSystem::getFrameTexture(ScreenFrameId num){
    const Frame* f = getFrame(num);
    return f->renderTexture;
}

int ScreenSystem::generateFrameNumber(const glm::ivec2& size){
    frameNum++;
    frames.push_back(ScreenSystem::FrameRequest{size,nullptr});
    return frameNum;
}

void ScreenSystem::bindFrame(ScreenFrameId num) {
    const Frame* f = getFrame(num);
    if(f!=nullptr){
        glBindFramebuffer(GL_FRAMEBUFFER, f->frameBuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, f->renderTexture.getUID(), 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER, f->depthBuffer);
        glViewport(0, 0, f->renderTexture.getWidth(), f->renderTexture.getHeight());
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

void ScreenSystem::mainFrame() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width,height);
}
