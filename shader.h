#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include<fstream>
#include<string>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <glm/gtc/type_ptr.hpp>

#include "SOIL2.h"


#ifndef Shader_h
#define Shader_h



class Shader
{
    
private:
    
    GLuint id;
    int version_total;
    
    std::string loadShaderSource(char* fileName)
    {
        
        std::string temp = "";
        std::string src = "";
        
        std::ifstream in_file;
        
        //vertex shader
        in_file.open(fileName);
        
        if(in_file.is_open())
        {
            while(std::getline(in_file, temp))
                src += temp + "\n";
        }
        else
        {
            std::cout << "Error::SHADER::could_not_open_vertex_file: " << fileName << "\n";
        }
        
        in_file.close();
        
        //std::string version = std::to_string(this->versionMajor) + std::to_string(this->versionMinor) + "0";
        //this line of code below is not the error
        std::cout << src.replace(src.find("#version"),12,("#version " + std::to_string(version_total))) << "\n";
        
        return src;
    }
    
    GLuint loadShader(GLenum type, char* fileName)
    {
        char infolog[512];
        GLint successful;
        //type should be GL_VERTEX_SHADER
        GLuint shader = glCreateShader(type);
        std::string str_src = this->loadShaderSource(fileName);
        const GLchar* src = /*this->loadShaderSource(fileName)*/str_src.c_str();
        glShaderSource(shader,1,&src,NULL);
        glCompileShader(shader);
        
        glGetShaderiv(shader, GL_COMPILE_STATUS, &successful);
        if(!successful)
        {
            glGetShaderInfoLog(shader,512,NULL,infolog);
            std::cout << "Error::loadshaders::could_not_compile_vertex_shader:" << fileName << "\n";
            std::cout << infolog << "\n";
            
        }
            return shader;
    }
    
    void linkProgram(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader)
    {
        char infolog[512];
        GLint successful;
        
        this->id = glCreateProgram();
        glUseProgram(this->id);
        
        glAttachShader(this->id,vertexShader);
        
        if(geometryShader)
            glAttachShader(this->id,geometryShader);
        
        if(fragmentShader)
        glAttachShader(this->id,fragmentShader);
        
        glLinkProgram(this->id);
        
        glGetProgramiv(this->id, GL_LINK_STATUS, &successful);
        if(!successful)
        {
            glGetProgramInfoLog(this->id,512,NULL,infolog);
            std::cout << "Error::SHADER::could_not_LINK_PROGRAM" << "\n";
            std::cout << infolog << "\n";
        }
        glUseProgram(0);
        std::cout<<"program unused init"<<std::endl;
        
        
        
        
        
    }
    
    
    enum { arrsize = 1 };
    
public:
    
    
    GLuint getId(){
        
        return this->id;
    }
     
    //constructors
    static const char *emptyFile;
    Shader(const int versionMajor,const int versionMinor, char *vertexFile = (char*)"",char *fragmentFile = (char*)"", char *geometryFile = (char*)"")
    
    {
        this->version_total = (versionMajor*100)+(versionMinor*10);
        
        GLuint vertexShader = 0;
        GLuint geometryShader = 0;
        GLuint fragmentShader = 0;
        
        vertexShader = loadShader(GL_VERTEX_SHADER,vertexFile);
        
        if(strncmp(geometryFile, (char*)"", 1)!=0)
            geometryShader = loadShader(GL_GEOMETRY_SHADER,geometryFile);
        
        fragmentShader = loadShader(GL_FRAGMENT_SHADER,fragmentFile);
        
        this->linkProgram(vertexShader,geometryShader,fragmentShader);
        
        glDeleteShader(vertexShader);
        glDeleteShader(geometryShader);
        glDeleteShader(fragmentShader);
    }
    
    
    ~Shader()
    {
        glDeleteProgram(this->id);
    }
    
    
    void use()
    {
        glUseProgram(this->id);
    }
    
    void unuse()
    {
        glUseProgram(0);
        std::cout<<"program unused standard" <<std::endl;
    }
    
    
    void set1i(GLint value, const GLchar* name)
    {glUniform1i(glGetUniformLocation(this->id,name),value);}
    
    void set1f(GLfloat value, const GLchar* name)
    {glUniform1f(glGetUniformLocation(this->id,name),value);}
    
    void setVec2f(glm::fvec2 value, const GLchar* name)
    {glUniform2fv(glGetUniformLocation(this->id,name),1,glm::value_ptr(value));}
    
    void setVec3f(glm::fvec3 value, const GLchar* name)
    {glUniform3fv(glGetUniformLocation(this->id,name),1,glm::value_ptr(value));}
    
    void setVec4f(glm::fvec4 value, const GLchar* name)
    {glUniform4fv(glGetUniformLocation(this->id,name),1,glm::value_ptr(value));}
    
    void setMat3fv(glm::mat3 value, const GLchar* name, GLboolean transpose = GL_FALSE)
    {glUniformMatrix3fv(glGetUniformLocation(this->id,name),1,transpose,glm::value_ptr(value));}
    
    void setMat4fv(glm::mat4 value, const GLchar* name, GLboolean transpose = GL_FALSE)
    {glUniformMatrix4fv(glGetUniformLocation(this->id,name),1,transpose,glm::value_ptr(value));}
    
    
    
};


#endif /* Shader_h */
