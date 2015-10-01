#include <GL/glut.h>
#include <ChibiEngine/Game.h>
#include <ChibiEngine/Resource/FileSystem.h>
#include <ChibiEngine/Render/ScreenSystem.h>
#include <ChibiEngine/Clock/ClockSystem.h>
#include <ChedLevel.h>
#include <unistd.h>

using namespace game;

const static long FRAME_TIME = 8;
long oldTime = 0;

void Reshape(int width, int height){
	Game::getScreen()->resize(width, height);
}

void draw(void){
  long timeNow = glutGet(GLUT_ELAPSED_TIME);
	Game::getClockSystem()->updateGlobalTime((timeNow - oldTime));
    Game::gameStep();
  long afterTime = glutGet(GLUT_ELAPSED_TIME);
  long need2wait = FRAME_TIME-(afterTime-timeNow);
  if(need2wait>0)
	  usleep(need2wait*1000);

  oldTime = timeNow;
  glutSwapBuffers();
  glutPostRedisplay();
}


void MoveListener(int x, int y){
    float _x = ((float)x/(Game::getScreen()->getWidth()-1))*2.0 - 1.0;
    float _y = (1.0-(float)y/(Game::getScreen()->getHeight()-1))*2.0 - 1.0;
	Game::getInputSystem()->mouseMoveEvent(glm::vec2(_x, _y));
}

void MouseClicker(int button, int state,  int x, int y){
    float _x = ((float)x/(Game::getScreen()->getWidth()-1))*2.0 - 1.0;
	float _y = (1.0-(float)y/(Game::getScreen()->getHeight()-1))*2.0 - 1.0;

    if(state == GLUT_DOWN)
		Game::getInputSystem()->clickDown(static_cast<MouseButton>(button), glm::vec2(_x, _y));
    else if(state == GLUT_UP)
		Game::getInputSystem()->clickUp(static_cast<MouseButton>(button), glm::vec2(_x, _y));
}

void releaseResources(){
    Game::dispose();
}

void  KeyUpListener(unsigned char key,int , int ){
	Game::getInputSystem()->keyPressed(key, InputEvent::RELEASED);
}

void  KeyDownListener(unsigned char key,int , int ){
	Game::getInputSystem()->keyPressed(key, InputEvent::PRESSED);
}

void  SpecialKeyDownListener(int key,int , int ){
	Game::getInputSystem()->keySpecialPressed(key, InputEvent::PRESSED);
}

void  SpecialKeyUpListener(int key,int , int ){
	Game::getInputSystem()->keySpecialPressed(key, InputEvent::RELEASED);
}

int main(int argc, char *argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_MULTISAMPLE);
    Game::initCoreSystems("ched/launcher.xml");
	ScreenType* es = Game::getScreenSettings();
	glutInitWindowSize(es->width, es->height);
	glutInitWindowPosition(es->x, es->y);
	glutCreateWindow(es->title.c_str());

    Game::initGraphSystems();
	glEnable(GL_MULTISAMPLE);
	Game::setLevelFactory([](const std::string file){
		return make_unique<ChedLevel>(file);
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

}
