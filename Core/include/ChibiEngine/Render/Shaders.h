#ifndef __SHADERS_H_
#define __SHADERS_H_

#include <ChibiEngine/Game.h>
#include <ChibiEngine/Platform/Platform.h>
#include <unordered_map>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <ChibiEngine/Log/Logger.h>
#include <ChibiEngine/Render/Color.h>

class ShaderType;
class UniformType;
class AttributeType;
namespace game{

    class ShaderSystem;

    struct UniformValue{
        UniformValue(){};
        UniformValue(float vec1):vec1(vec1){};
        UniformValue(const glm::vec2& vec2):vec2(vec2){};
        UniformValue(const glm::vec3& vec3):vec3(vec3){};
        UniformValue(const glm::vec4& vec4):vec4(vec4){};
        UniformValue(const Color& color):vec4(color.toVector()){};
        UniformValue(const glm::mat4& mat4):mat4(mat4){};

        float vec1;
        glm::vec2 vec2;
        glm::vec3 vec3;
        glm::vec4 vec4;
        glm::mat4 mat4;
    };

    class GenericShader;
    struct ShaderValues {
        inline ShaderValues();
        inline ShaderValues(GLuint  buffer, GLuint index, GLsizei count);
        ~ShaderValues();

        ShaderValues(const ShaderValues& other) = delete;
        ShaderValues& operator=(const ShaderValues& other) = delete;
        ShaderValues(ShaderValues&& other);
        ShaderValues& operator=(ShaderValues&& other);

        void fillBuffer(const std::vector<GLfloat>& buffer);
        void fillBuffer(GLfloat* buffer, int size);
        void fillIndex(const std::vector<GLubyte>& index);
        void fillIndex(const std::vector<GLuint>& index);
        void fillIndex(GLubyte* index, int size);


        GLsizei count;
        mutable std::unordered_map<std::string, UniformValue> uniformValues;
        std::unordered_map<std::string, GLuint> samplerValues;
        GLenum drawMode, indexSize;
        mutable int startOffset;
        bool useIndex;

    private:
        friend class GenericShader;
        void recreateBuffer();
        void recreateIndex();

        bool bufferCreated = false;
        bool indexCreated = false;
        GLuint buffer, index;
    };

    class GenericShader : private UniqueClass {
    public:
        GenericShader(ShaderType* shaderInfo, ShaderSystem* shaderSystem);

        inline GLuint getSamplerUid(const std::string& name) const;

        void draw(const ShaderValues& values,
                GLuint _buffer,
                GLuint _index,
                const std::unordered_map<std::string, UniformValue>& uniValues,
                const std::unordered_map<std::string, GLuint>& samplerValues) const;

        void draw(const ShaderValues& values,
                GLuint _buffer,
                GLuint _index,
                const std::unordered_map<std::string, UniformValue>& uniValues) const;

        void draw(const ShaderValues & values,
                const std::unordered_map<std::string, UniformValue>& uniValues) const;

        void draw(const ShaderValues& values) const;

        ~GenericShader();
    private:
        //friend class ShaderSystem;

        void checkError(const std::string& step)const;
        void initProgram(ShaderType* shaderInfo, const Logger& logger);
        inline void setId(size_t id);
        inline size_t getid();

        struct UniformDescriptor{
            UniformDescriptor(){}
            UniformDescriptor(GLuint prog, UniformType* data, const Logger& logger);
            int count;
            bool matrix;
            GLint uid;
        };
        struct SamplerDescription{
            SamplerDescription(){}
            SamplerDescription(GLuint prog, int num, const std::string& name, const Logger& logger);
            int num;
            GLint uid;
        };
        struct AttributeDescriptor{
            AttributeDescriptor(){}
            AttributeDescriptor(GLuint prog, AttributeType* data, const Logger& logger);
            int offset;
            int count;
            GLint uid;
        };

        GLuint program, pixelShader, vertexShader;
        size_t id;
        std::unordered_map<std::string, UniformDescriptor> uniforms;
        std::unordered_map<std::string, SamplerDescription> samplers;
        std::vector<AttributeDescriptor> attributes;
        GLsizei attributesPerVertex;
        bool useDrawElements;
        std::string name;
    };

    class ShaderSystem : private UniqueClass {
    public:
        ShaderSystem(const std::string& name);
        //inline const GenericShader& get(int id);
        inline const GenericShader& get(const std::string& name);

        inline const Logger& getLogger() const;
    private:
        const Logger logger;
        //std::vector<GenericShader> shaders;
        std::unordered_map<std::string, const GenericShader> index;

    };

    inline void GenericShader::setId(size_t id){
        GenericShader::id = id;
    }

    inline size_t GenericShader::getid(){
        return id;
    }

    inline GLuint GenericShader::getSamplerUid(const std::string& name) const{
        return samplers.find(name)->second.uid;
    }

    inline ShaderValues::ShaderValues(){
        drawMode=GL_TRIANGLES;
        indexSize= GL_UNSIGNED_BYTE;
        count = 6;
        startOffset=0;
        ShaderValues::useIndex = true;
    }

    inline ShaderValues::ShaderValues(GLuint buffer, GLuint index, GLsizei count)
    : ShaderValues(){
        ShaderValues::buffer = buffer;
        ShaderValues::index = index;
        ShaderValues::count = count;
    }


    inline const Logger& ShaderSystem::getLogger() const{
        return logger;
    }

    /* inline const GenericShader* ShaderSystem::get(int id){
         return shaders[id];
     }*/

    inline const GenericShader& ShaderSystem::get(const std::string& name){
        auto it = index.find(name);
        if(it == index.end()){
            logger.error("shader [%s] not found",name.c_str());
        }
        return it->second;
    }

}

#endif
