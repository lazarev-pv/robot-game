#include <ChibiEngine/Render/Primitives/PrimitiveDrawer.h>
#include <ChibiEngine/Render/Shaders.h>
#include <ChibiEngine/Render/ScreenSystem.h>

#define GLM_FORCE_RADIANS
#include <glm/gtx/fast_square_root.hpp>
#include <glm/gtx/transform.hpp>

using namespace std;
using namespace glm;
using namespace game;


PrimitiveDrawer::PrimitiveDrawer(){
	glGenBuffers(1, &arrowVertsBuffer);
	glGenBuffers(1, &trianlesIndexBuffer);
	glGenBuffers(1, &barVertsBuffer);
	glGenBuffers(1, &lineIndexBuffer);
	glGenBuffers(1, &drawTextVertsBuffer);

	{
		GLubyte inds[] = {
			0,1,2,1,3,2
		};
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, trianlesIndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,(6)*sizeof(GLubyte), inds, GL_STATIC_DRAW);
	}{
		GLubyte inds2[] = {
			0,1,3,2
		};
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lineIndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,(4)*sizeof(GLubyte), inds2, GL_STATIC_DRAW);
	}{
		GLfloat verts[] = {
			-1.0f, -1.0f, 0.0f,
			-1.0f, 1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			1.0f, 1.0f, 0.0f
		};

		glBindBuffer(GL_ARRAY_BUFFER, barVertsBuffer);
		glBufferData(GL_ARRAY_BUFFER,(3*4)*sizeof(GLfloat), verts, GL_STATIC_DRAW);
	}{
		GLfloat verts2[] = {
			-1.0f, -0.9f, 0.0f,
			 0.0f,  1.0f, 0.0f,
			 0.0f, -1.0f, 0.0f,
			 1.0f,  -0.9f, 0.0f
		};

		glBindBuffer(GL_ARRAY_BUFFER, arrowVertsBuffer);
		glBufferData(GL_ARRAY_BUFFER,(3*4)*sizeof(GLfloat), verts2, GL_STATIC_DRAW);
	}{
        GLfloat verts[] = {
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
                 1.0f,  1.0f, 0.0f, 1.0f, 1.0f
        };

        glBindBuffer(GL_ARRAY_BUFFER, drawTextVertsBuffer);
        glBufferData(GL_ARRAY_BUFFER,(5*4)*sizeof(GLfloat), verts, GL_STATIC_DRAW);
    }

    generateArrow();
}

PrimitiveDrawer::~PrimitiveDrawer(){
	glDeleteBuffers(1,&trianlesIndexBuffer);
	glDeleteBuffers(1,&lineIndexBuffer);
	glDeleteBuffers(1,&barVertsBuffer);
	glDeleteBuffers(1,&arrowVertsBuffer);

	glDeleteBuffers(1,&simpleArrowIndexes);
	glDeleteBuffers(1,&simpleArrowBuffers);
	glDeleteBuffers(1,&drawTextVertsBuffer);
}


void PrimitiveDrawer::generateArrow(){
    glGenBuffers(1, &simpleArrowIndexes);
    glGenBuffers(1, &simpleArrowBuffers);
    vector<GLubyte>inds = {
            0,1,2,1,3
    };
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, simpleArrowIndexes);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,inds.size()*sizeof(GLubyte), &(inds[0]), GL_STATIC_DRAW);

    vector<GLfloat> verts = {
            -1.0f, 0.8f, 0.0f,
            0.0f,  1.0f, 0.0f,
            1.0f, 0.8f, 0.0f,
            0.0f,  -1.0f, 0.0f
    };

    glBindBuffer(GL_ARRAY_BUFFER, simpleArrowBuffers);
    glBufferData(GL_ARRAY_BUFFER,verts.size()*sizeof(GLfloat), &(verts[0]), GL_STATIC_DRAW);
}

void PrimitiveDrawer::drawTexture(const glm::mat4 &transform, const Texture &texture, const std::string& shaderName) const {
    sv.drawMode = GL_TRIANGLES;
    sv.count = 6;
    Game::getShaderSystem()->get(shaderName)
            .draw(  sv,
                    drawTextVertsBuffer,
                    trianlesIndexBuffer,
                    unordered_map<string, UniformValue>({
                        {"u_mvp_matrix",UniformValue(transform)}}),
                    unordered_map<string, GLuint>({
                        {"s_texture",texture.getUID()}}));
}

void PrimitiveDrawer::drawRectangleBorder(const glm::mat4& transform, const Color& color) const{
    sv.drawMode=GL_LINE_LOOP;
    sv.count = 4;
    Game::getShaderSystem()->get("simple").draw(sv,
			barVertsBuffer, lineIndexBuffer,
			unordered_map<string, UniformValue>({
            {"u_mvp_matrix",UniformValue(transform)},
            {"u_color",UniformValue(color)}}));
}

void PrimitiveDrawer::drawFilledRectangle(const glm::mat4& transform, const Color& color) const{
    sv.drawMode=GL_TRIANGLES;
    sv.count = 6;
    Game::getShaderSystem()->get("simple")
            .draw(sv,barVertsBuffer,trianlesIndexBuffer,
					unordered_map<string, UniformValue>({
            {"u_mvp_matrix",UniformValue(transform)},
            {"u_color",UniformValue(color)}}));
}

void PrimitiveDrawer::drawSimpleArrow(
		const glm::vec3 &beg,
		const glm::vec3 &end,
		const Color &color,
		CameraCHandle camera) const{

    float len = glm::length(end-beg);
    float angle = (beg.x<end.x?-1:1)*glm::acos((end.y-beg.y)/len);

    sv.drawMode = GL_LINE_STRIP;
    sv.count = 5;

    Game::getShaderSystem()->get("simple")
            .draw(sv,simpleArrowBuffers, simpleArrowIndexes,
					unordered_map<string, UniformValue>({
            {"u_mvp_matrix",UniformValue(createMatrix((beg+end)*0.5f,angle,vec2(len*0.08f,len*0.5f),camera))},
            {"u_color",UniformValue(color)}}));
}

void PrimitiveDrawer::drawFilledArrow(const glm::mat4& transform, const Color& color) const{
	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT,  GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT,  GL_NICEST);

    sv.drawMode=GL_TRIANGLES;
    sv.count = 6;
    Game::getShaderSystem()->get("simple")
            .draw(sv, arrowVertsBuffer, trianlesIndexBuffer,
					unordered_map<string, UniformValue>({
            {"u_mvp_matrix",UniformValue(transform)},
            {"u_color",UniformValue(color)}}));

	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_POLYGON_SMOOTH);
}

void PrimitiveDrawer::drawBorderedFilledRectangle(
		const glm::vec3 &pos,
        const glm::vec2& size,
		float borderWidth,
		const Color &backColor,
		const Color &borderColor,
		CameraCHandle camera) const{

	float ky = (camera==nullptr?Game::getScreen()->getAspectRatio():1);
	drawFilledRectangle(createMatrix(pos, size,camera), borderColor);
	drawFilledRectangle(createMatrix(pos, vec2(size.x - borderWidth, size.y - borderWidth * ky),camera), backColor);
}

void PrimitiveDrawer::drawPartialBorderedFilledRectangle(
		const glm::vec3 &pos,
        const glm::vec2& size,
		float borderWidth,
		const Color &backColor,
		const Color &borderColor,
		glm::bvec4 borders,
		CameraCHandle cam) const{

	if(borders.x && borders.y && borders.z && borders.w){
		drawBorderedFilledRectangle(pos, size, borderWidth, backColor, borderColor, cam);
		return;
	}

	drawFilledRectangle(createMatrix(pos, size, cam),backColor);
	drawPartialRectangleBorder(pos, size, borderWidth, borderColor,borders, cam);
}

void PrimitiveDrawer::drawLine(
		const glm::vec3 &beg,
		const glm::vec3 &end,
		const Color &color,
		CameraCHandle cam) const{

	GLuint bufs[2];
	glGenBuffers(2, bufs);

	GLubyte inds[] = {0, 1};

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufs[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,(2)*sizeof(GLubyte), inds, GL_DYNAMIC_DRAW);

	GLfloat verts[] = {
		beg.x, beg.y, beg.z,
		end.x, end.y, end.z
	};

	glBindBuffer(GL_ARRAY_BUFFER, bufs[1]);
	glBufferData(GL_ARRAY_BUFFER,(6)*sizeof(GLfloat), verts, GL_DYNAMIC_DRAW);

	glm::mat4 transMat(1);
	if(cam!=nullptr){
		transMat = cam->getMVPMatrix();
	}

    sv.drawMode=GL_LINES;
    sv.count = 2;
    Game::getShaderSystem()->get("simple")
            .draw(sv,bufs[1],bufs[0],
					unordered_map<string, UniformValue>({
            {"u_mvp_matrix",UniformValue(transMat)},
            {"u_color",UniformValue(color)}}));

	glDeleteBuffers(2,bufs);
}


void PrimitiveDrawer::drawPartialRectangleBorder(
		const glm::vec3 &pos,
        const glm::vec2& size,
		float borderWidth,
		const Color &borderColor,
		glm::bvec4 borders,
		CameraCHandle cam) const{

	float hb = borderWidth*0.5;
	float ar = cam==nullptr?Game::getScreen()->getAspectRatio():1;

	if(borders.x)
		drawFilledRectangle(createMatrix(vec3(pos.x - size.x + hb, pos.y, 0), vec2(hb, size.y), cam), borderColor);
	if(borders.y)
		drawFilledRectangle(createMatrix(vec3(pos.x, pos.y + size.y - hb, 0), vec2(size.x, hb * ar),cam), borderColor);
	if(borders.z)
		drawFilledRectangle(createMatrix(vec3(pos.x + size.x - hb, pos.y, 0), vec2(hb, size.y),cam), borderColor);
	if(borders.w)
		drawFilledRectangle(createMatrix(vec3(pos.x, pos.y - size.y + hb, 0), vec2(size.x, hb * ar),cam), borderColor);
}

