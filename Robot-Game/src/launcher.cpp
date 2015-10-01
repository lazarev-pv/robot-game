#include <GL/glut.h>
#include <ChibiEngine/Platform/Platform.h>
#include <ChibiEngine/Render/ScreenSystem.h>
#include <ChibiEngine/Resource/FileSystem.h>
#include <ChibiEngine/Clock/ClockSystem.h>
#include <ChibiEngine/GameState/GameState.h>
#include <unistd.h>
#include <RobotLevel.h>
#include <GL/freeglut_ext.h>
#include <memory>

using std::cout;
using std::endl; 
using namespace game;
using namespace glm;
using namespace std;

int winHandle = 0;
long oldTime = 0;
float timeMulti = 1.0;
const int MAX_MOUSE_BUTTON = 2;
MouseButton mouseReference[MAX_MOUSE_BUTTON+1];


void Reshape(int width, int height){
    game::Game::getScreen()->resize(width, height);
}

const static long FRAME_TIME = 0;

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
//	cout<<"button "<<button<<" state "<<state<<endl;

	if(button>MAX_MOUSE_BUTTON)
		return;

    game::ScreenSystem* sm = game::Game::getScreen();
    float _x = ((float)x/sm->getWidth())*2.0 - 1.0;
    float _y = (1.0-(float)y/sm->getHeight())*2.0 - 1.0;

    if(state == GLUT_DOWN)
        Game::getInputSystem()->clickDown(mouseReference[button], glm::vec2(_x, _y));
    else if(state == GLUT_UP)
        Game::getInputSystem()->clickUp(mouseReference[button], glm::vec2(_x, _y));
}

void releaseResources(){
	cout<<"Releasing Resources"<<endl;
    Game::dispose();
    //glutDestroyWindow(winHandle);
	cout<<"Successed"<<endl;
}

void  KeyUpListener(unsigned char key,int x, int y){
    Game::getInputSystem()->keyPressed(key, InputEvent::RELEASED);
}

void  KeyDownListener(unsigned char key,int x, int y){
    Game::getInputSystem()->keyPressed(key, InputEvent::PRESSED);
}

void  SpecialKeyDownListener(int key,int x, int y){
    Game::getInputSystem()->keySpecialPressed(key, InputEvent::PRESSED);
}

void  SpecialKeyUpListener(int key,int x, int y){
    Game::getInputSystem()->keySpecialPressed(key, InputEvent::RELEASED);
}


int main(int argc, char *argv[]) {

    mouseReference[0] = MouseButton::LEFT;
    mouseReference[1] = MouseButton::MIDDLE;
    mouseReference[2] = MouseButton::RIGHT;

    cout << "Before gluInit" << endl;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
    cout << "After gluInit" << endl;

    /* Extracting config */
    if (argc > 2 && strcmp(argv[1], "--settings") == 0) {
        Game::initCoreSystems(argv[2], "");
    } else {
        Game::initCoreSystems("game/launcher.xml");
    }

    ScreenType *es = Game::getScreenSettings();
    glutInitWindowSize(es->width, es->height);
    glutInitWindowPosition(es->x, es->y);
    winHandle = glutCreateWindow(es->title.c_str());
    cout << "After Create Window" << endl;
    Game::initGraphSystems();

    //glEnable(GL_MULTISAMPLE);
    Game::setLevelFactory([](const string file){
        return make_unique<RobotLevel>(file);
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

    //glutCloseFunc(releaseResources);
    atexit(releaseResources);

    glutMainLoop();

    cout << "Disposing resources" << endl;
}

	
