
#include <ChibiEngine/Render/Shaders.h>
#include <fstream>
#include <ChibiEngine/Log/LoggingSystem.h>
#include <ChibiEngine/Resource/FileSystem.h>
#include <ChibiEngine/XMLParser/ShadersParser.h>
#include <ChibiEngine/Common/ParserUtils.h>

#define _PARANOID_DEBUG

using namespace game;
using namespace std;
using namespace rapidxml;

static const char* LOGGER_NAME = "ShaderSystem";

ShaderSystem::ShaderSystem(const std::string& name)
:logger(Game::getLoggingSystem()->createLogger(LOGGER_NAME)){
    logger.trace("Initialization of shader system");

    auto root(createRoot<ShadersParserRoot>(name));
    for (ShaderType *iter : root.shaders->shader) {
        iter->file = FileSystem::getPath(name) + "/" + iter->file;
        //GenericShader *shader = new GenericShader(iter, this);
        //shader->setId(shaders.size());
        //shaders.push_back(shader);
        index.emplace(std::piecewise_construct,
                std::forward_as_tuple(iter->name),
                std::forward_as_tuple(iter, this));
    }
    logger.info("Shader system initialized");
}

#ifdef PARANOID_DEBUG
void GenericShader::checkError(const std::string& step) const{
    GLenum er = glGetError();
    if(er!=0){
        Game::getShaderSystem()->getLogger().error("Shader %s. %s GL Error happend %i",name.c_str(), step.c_str(),er);
    }
}
#else
void GenericShader::checkError(const std::string& ) const{
}
#endif

GenericShader::GenericShader(ShaderType* shaderInfo, ShaderSystem* shaderSystem){
    name=shaderInfo->name;
    const Logger& log = shaderSystem->getLogger();
    initProgram(shaderInfo, log);
    int samplersNum=0;
    for(UniformType* uni : shaderInfo->uniform){
        if(uni->sampler){
            samplers[uni->name]=SamplerDescription(program, samplersNum++,uni->name,log);
        }else{
            uniforms[uni->name]=UniformDescriptor(program, uni,log);
        }
    }
    attributesPerVertex = 0;
    for(AttributeType* atr : shaderInfo->attribute){
        attributesPerVertex+=atr->count;
        attributes.push_back(AttributeDescriptor(program, atr,log));
    }
    if(shaderInfo->atrStep>0){
        attributesPerVertex=shaderInfo->atrStep;
    }
    useDrawElements=shaderInfo->useDrawElements;
}

GenericShader::UniformDescriptor::UniformDescriptor(GLuint prog, UniformType* data, const Logger& logger)
:count(data->count),matrix(data->matrix){
    uid = glGetUniformLocation(prog, data->name.c_str());
    if(uid<0)
        logger.error("Uniform name %s not found", data->name.c_str());
}

GenericShader::SamplerDescription::SamplerDescription(GLuint prog, int num, const std::string& name, const Logger& logger)
:num(num){
    uid = glGetUniformLocation(prog, name.c_str());
    if(uid<0)
        logger.error("Sampler name %s not found", name.c_str());
}

GenericShader::AttributeDescriptor::AttributeDescriptor(GLuint prog, AttributeType* data, const Logger& logger)
:offset(data->offset),count(data->count){
    uid = glGetAttribLocation(prog, data->name.c_str());
    if(uid<0)
        logger.error("Attribute name %s not found", data->name.c_str());
}

void GenericShader::draw(const ShaderValues & values) const{
    draw(values, values.buffer, values.index, values.uniformValues);
}

void GenericShader::draw(const ShaderValues & values, const std::unordered_map<std::string, UniformValue>& uniValues) const{
    draw(values, values.buffer, values.index, uniValues);
}

void GenericShader::draw(
        const ShaderValues & values,
        GLuint _buffer, GLuint _index,
        const std::unordered_map<string, UniformValue>& uniValues) const{

    draw(values, _buffer, _index, uniValues, values.samplerValues);
}


void GenericShader::draw(
        const ShaderValues & values,
        GLuint _buffer, GLuint _index,
        const std::unordered_map<string, UniformValue>& uniValues,
        const std::unordered_map<std::string, GLuint>& samplerValues) const{
    glUseProgram(program);
    checkError("Program");
    for(auto iter = uniValues.begin();iter!= uniValues.end();++iter){
        auto descriptor = uniforms.find(iter->first);
        if(descriptor==uniforms.end()){
            Game::getShaderSystem()->getLogger().error("Shader: %s. Uniform descriptor %s not found", name.c_str(), iter->first.c_str());
            return;
        }
        if(descriptor->second.matrix){
            glUniformMatrix4fv(descriptor->second.uid, 1, GL_FALSE, &(iter->second.mat4[0][0]));
            checkError("glUniformMatrix4fv");
        }else{
            const UniformValue& u = iter->second;
            switch(descriptor->second.count){
                case 1:
                    glUniform1f(descriptor->second.uid, u.vec1);
                    checkError("glUniform1f");
                    break;
                case 2:
                    glUniform2f(descriptor->second.uid, u.vec2.x,u.vec2.y);
                    checkError("checkError");
                    break;
                case 3:
                    glUniform3f(descriptor->second.uid, u.vec3.x,u.vec3.y,u.vec3.z);
                    checkError("glUniform3f");
                    break;
                case 4:
                    glUniform4f(descriptor->second.uid, u.vec4.x,u.vec4.y,u.vec4.z,u.vec4.w);
                    checkError("glUniform4f");
                    break;
                default:
                    Game::getShaderSystem()->getLogger().error("Incorrect uniform count: %i", descriptor->second.count);
            }
            //glUniform4fv(descriptor.uid, descriptor.count, iter->second);
        }
    }

    for(auto iter = samplerValues.begin();iter!= samplerValues.end();++iter){
        auto descriptor = samplers.find(iter->first);
        if(descriptor==samplers.end()){
            Game::getShaderSystem()->getLogger().error("Sampler descriptor %s not found", iter->first.c_str());
            return;
        }

    //    float fw;
        glActiveTexture(GL_TEXTURE0+descriptor->second.num);
        checkError("glActiveTexture");
        glBindTexture(GL_TEXTURE_2D, iter->second);
  //      glGetTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_WIDTH, &fw);
        checkError("glBindTexture");
        glUniform1i(descriptor->second.uid, descriptor->second.num);
        checkError("glUniform1i");
    }

    GLfloat* offset = 0;
    offset+=values.startOffset;
    glBindBuffer(GL_ARRAY_BUFFER, _buffer);
    for(auto iter = attributes.begin();iter!= attributes.end();++iter){
        glVertexAttribPointer(iter->uid,iter->count,GL_FLOAT,GL_FALSE, attributesPerVertex*sizeof(GLfloat), offset+iter->offset);
        checkError("glVertexAttribPointer");
        glEnableVertexAttribArray(iter->uid);
        checkError("glEnableVertexAttribArray");
    }

    if(useDrawElements && values.useIndex){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _index);
        checkError("glBindBuffer");
        glDrawElements(values.drawMode, values.count,values.indexSize,0);
        checkError("glDrawElements");
    }else{
        glDrawArrays(values.drawMode, 0, values.count);
        checkError("glDrawArrays");
    }

}

string loadTextFromFile(ShaderType* shaderInfo, const Logger& logger){
	ifstream in;
    Game::getFileSystem()->openFileStream(in,shaderInfo->file, ifstream::in);
	std::string contents;
	in.seekg(0, std::ios::end);
	contents.resize(in.tellg());
	in.seekg(0, std::ios::beg);
	in.read(&contents[0], contents.size());
	in.close();
    logger.debug("Loaded shader text: \n" + contents);
	return contents;
}

GLuint loadShader(ShaderType* shaderInfo, GLenum type, const Logger& logger){
	const char* cname = shaderInfo->name.c_str();
    logger.trace("Loading shader with type %d and name %s", type, cname);
	string text = loadTextFromFile(shaderInfo,logger);
	if(type==GL_VERTEX_SHADER){
		text="#version 120\n#define COMPILE_VERTEX\n"+text;
	}else if(type==GL_FRAGMENT_SHADER){
		text="#version 120\n#define COMPILE_FRAGMENT\n"+text;
	}

	GLuint shader = glCreateShader(type);
	GLint status;

	if (shader == 0){
        GLenum errorNo = glGetError();
        logger.error("Can not create shader: %s, errNo: %d", cname, errorNo);
		return 0;
	}
	const char* cText = text.c_str();

	glShaderSource(shader,1,&cText,0);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if(!status){
        logger.error("Bad compiling status: %d shader: %s", status, cname);

		GLint infoLen = 0;
		glGetShaderiv(shader,GL_INFO_LOG_LENGTH, &infoLen);

		char* infoLog = 0;
		if(infoLen){
			infoLog = (char*)malloc(sizeof(char)*infoLen);
			glGetShaderInfoLog(shader,infoLen,0, infoLog);
            logger.error("error while compiling shader %s: %s", cname, infoLog);
			free(infoLog);
		}
		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

void GenericShader::initProgram(ShaderType* shaderInfo, const Logger& logger){

	vertexShader = loadShader(shaderInfo, GL_VERTEX_SHADER,logger);
	pixelShader = loadShader(shaderInfo, GL_FRAGMENT_SHADER,logger);

	program = glCreateProgram();
	if(program == 0){
        logger.error("Can not create program " + shaderInfo->name);
		return;
	}

	glAttachShader(program,vertexShader);
	glAttachShader(program,pixelShader);
	glLinkProgram(program);

	int linked;
	glGetProgramiv(program,GL_LINK_STATUS, &linked);

	if(!linked){
		GLint infoLen = 0;
		glGetProgramiv(program,GL_INFO_LOG_LENGTH, &infoLen);

		char* infoLog = nullptr;

		if(infoLen>1){
			infoLog = (char *)malloc(sizeof(char)*infoLen);
			glGetProgramInfoLog(program,infoLen,0, infoLog);
		}

		glDeleteProgram(program);
        logger.error("error while linking program %s: %s", shaderInfo->name.c_str(), infoLog);
		return;
	}
    logger.info("Shader Program '" + shaderInfo->name + "' created");
}

GenericShader::~GenericShader(){
	glDeleteProgram(program);
	glDeleteShader(pixelShader);
	glDeleteShader(vertexShader);
}


ShaderValues::~ShaderValues(){
    if(bufferCreated)
        glDeleteBuffers(1, &buffer);
    if(indexCreated)
        glDeleteBuffers(1, &index);
}

void ShaderValues::recreateBuffer(){
    if(bufferCreated)
        glDeleteBuffers(1, &buffer);
    glGenBuffers(1, &buffer);
    bufferCreated=true;
}

void ShaderValues::recreateIndex(){
    if(indexCreated)
        glDeleteBuffers(1, &index);
    glGenBuffers(1, &index);
    indexCreated=true;
}

void ShaderValues::fillBuffer(const std::vector<GLfloat>& buf){
    recreateBuffer();
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, buf.size()*sizeof(GLfloat), &(buf[0]), GL_STATIC_DRAW);
}

void ShaderValues::fillBuffer(GLfloat* buf, int size){
    recreateBuffer();
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, size*sizeof(GLfloat), buf, GL_STATIC_DRAW);
}

void ShaderValues::fillIndex(const std::vector<GLubyte>& inds){
    recreateIndex();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,inds.size()*sizeof(GLubyte), &(inds[0]), GL_STATIC_DRAW);
}

void ShaderValues::fillIndex(const std::vector<GLuint>& inds){
    recreateIndex();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,inds.size()*sizeof(GLuint), &(inds[0]), GL_STATIC_DRAW);
}

void ShaderValues::fillIndex(GLubyte* inds, int size){
    recreateIndex();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size*sizeof(GLubyte), &(inds[0]), GL_STATIC_DRAW);
}


ShaderValues::ShaderValues(ShaderValues&& other)
    :count(other.count),
     uniformValues(move(other.uniformValues)),
     samplerValues(move(other.samplerValues)),
     drawMode(other.drawMode),
     indexSize(other.indexSize),
     startOffset(other.startOffset),
     useIndex(other.useIndex),
     bufferCreated(other.bufferCreated),
     indexCreated(other.indexCreated),
     buffer(other.buffer),
     index(other.index){

    other.bufferCreated = false;
    other.indexCreated = false;
}

ShaderValues& ShaderValues::operator=(ShaderValues&& other){

    count = other.count;
    uniformValues = move(other.uniformValues);
    samplerValues = move(other.samplerValues);
    drawMode = other.drawMode;
    indexSize = other.indexSize;
    startOffset = other.startOffset;
    useIndex = other.useIndex;
    bufferCreated = other.bufferCreated;
    indexCreated = other.indexCreated;
    buffer = other.buffer;
    index = other.index;

    other.bufferCreated = false;
    other.indexCreated = false;
    return *this;
}