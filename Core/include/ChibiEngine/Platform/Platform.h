#ifndef __HEADER_ENVIROMENT_
#define	 __HEADER_ENVIROMENT_

#define GL_GLEXT_PROTOTYPES

#if defined(MY_WIN)
	#include <windows.h>
	#include <GL/glew.h>
	#include <GL/gl.h>
	#include <GL/glext.h>
	#include <sstream>
	#undef far
	#undef near
	#undef ERROR
#endif


#if defined(MY_ANDROID)
	#include <sstream>
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#endif

#if !(defined(MY_ANDROID) || defined (MY_WIN))
    #include <GL/gl.h>
	#include <GL/glext.h>
#endif


/* Mouse Cursor alteration support */
#if !(defined(MY_ANDROID)) && defined(__INPUT_HEADER_IMPLEMENTATION_)
#include <GL/freeglut.h>

namespace game{
	void setCursorHand(){
		glutSetCursor(GLUT_CURSOR_CROSSHAIR);
	}
	void setCursorUsual(){
		glutSetCursor(GLUT_CURSOR_INHERIT);
	}
	void setCursorText(){
		glutSetCursor(GLUT_CURSOR_TEXT);
	}
}
#elif defined(__INPUT_HEADER_IMPLEMENTATION_)
 namespace game{
 void setCursorHand(){ }
 	void setCursorUsual(){}
 	void setCursorText(){}
 }
#endif


#include <string>


namespace game{

#if defined(MY_ANDROID) || defined (MY_WIN)

	inline std::string to_string(int t) {
		std::ostringstream os;
		os << t;
		return os.str();
	}

#endif



}

#endif

