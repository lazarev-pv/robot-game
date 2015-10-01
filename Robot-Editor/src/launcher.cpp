#include <GL/glut.h>
#include <ChibiEngine/Platform/Platform.h>
#include <ChibiEngine/Render/ScreenSystem.h>
#include <ChibiEngine/Resource/FileSystem.h>
#include <ChibiEngine/Clock/ClockSystem.h>
#include <ChibiEngine/GameState/GameState.h>
#include <unistd.h>
#include <memory>
#include <EditorLevel.h>

using std::cout;
using std::endl; 
using namespace game;

long oldTime = 0;
float timeMulti = 1.0;


void Reshape(int width, int height){
    game::Game::getScreen()->resize(width, height);
}

const static long FRAME_TIME = 4;

void draw(void){
  long timeNow = glutGet(GLUT_ELAPSED_TIME);
    game::Game::getClockSystem()->updateGlobalTime((timeNow-oldTime)*timeMulti);
    game::Game::gameStep();
  long afterTime = glutGet(GLUT_ELAPSED_TIME);

  long need2wait = FRAME_TIME-(afterTime-timeNow);
  if(need2wait>0)
	  usleep(need2wait*1000);

  oldTime = timeNow;
  glutSwapBuffers();
  glutPostRedisplay();
}


void MoveListener(int x, int y){
    game::ScreenSystem* sm = game::Game::getScreen();
    float _x = ((float)x/(sm->getWidth()-1))*2.0 - 1.0;
    float _y = (1.0-(float)y/(sm->getHeight()-1))*2.0 - 1.0;
    Game::getInputSystem()->mouseMoveEvent(glm::vec2(_x, _y));
}



void MouseClicker(int button, int state,  int x, int y){

    game::ScreenSystem* sm = game::Game::getScreen();
    float _x = ((float)x/sm->getWidth())*2.0 - 1.0;
    float _y = (1.0-(float)y/sm->getHeight())*2.0 - 1.0;

    if(state == GLUT_DOWN)
        Game::getInputSystem()->clickDown(static_cast<MouseButton>(button), glm::vec2(_x, _y));
    else if(state == GLUT_UP)
        Game::getInputSystem()->clickUp(static_cast<MouseButton>(button), glm::vec2(_x, _y));
}

void releaseResources(){
	cout<<"Releasing Resources"<<endl;
    Game::dispose();
	cout<<"Successed"<<endl;
}

void  KeyUpListener(unsigned char key,int x, int y){
    Game::getInputSystem()->keyPressed(key, InputEvent::RELEASED);
}

void  KeyDownListener(unsigned char key,int x, int y){
    Game::getInputSystem()->keyPressed(key, InputEvent::PRESSED);
	cout.flush();
}

void  SpecialKeyDownListener(int key,int x, int y){
    Game::getInputSystem()->keySpecialPressed(key, InputEvent::PRESSED);
	cout.flush();
}

void  SpecialKeyUpListener(int key,int x, int y){
    Game::getInputSystem()->keySpecialPressed(key, InputEvent::RELEASED);
}


int main(int argc, char *argv[]){


	cout<<"Before gluInit"<<endl;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
    cout<<"After gluInit"<<endl;


    Game::initCoreSystems("editor/launcher.xml");

    ScreenType* es = Game::getScreenSettings();

    glutInitWindowSize(es->width, es->height);
	glutInitWindowPosition(es->x, es->y);
	glutCreateWindow(es->title.c_str());
    cout<<"After Create Window"<<endl;
    Game::initGraphSystems();

    Game::setLevelFactory([](const std::string file){
        return make_unique<EditorLevel>(file);
    });


    glutReshapeFunc(Reshape);
	glutDisplayFunc(draw);
	oldTime = glutGet(GLUT_ELAPSED_TIME);

	glutIgnoreKeyRepeat(1);
    glutMotionFunc(MoveListener);
    glutPassiveMotionFunc(MoveListener);
    glutMouseFunc(MouseClicker);
    glutKeyboardFunc(KeyDownListener);
    glutSpecialFunc(SpecialKeyDownListener);
    glutSpecialUpFunc(SpecialKeyUpListener);
    glutKeyboardUpFunc(KeyUpListener);
    atexit(releaseResources);
    
    glutMainLoop();
    
    cout<<"Disposing resources"<<endl;
}
	
