#include "ofxComposableShader.h"
#include "ofFileUtils.h"
#include "ofImage.h"

using namespace std;
using namespace ofx::ComposableShader;

void Shader::reload()
{
	auto &s = settings_;
	auto attachShader = [](ofShader &shader, GLenum type, string filepath) {
		GLuint shaderObj = glCreateShader(type);
		string source = ofBufferFromFile(filepath).getText();
		const char* sptr = source.c_str();
		int ssize = source.size();
		glShaderSource(shaderObj, 1, &sptr, &ssize);
		glCompileShader(shaderObj);
		glAttachShader(shader.getProgram(), shaderObj);
		glDeleteShader(shaderObj);
		return true;
	};
	auto attachShaders = [this,attachShader](ofShader &shader, GLenum type, vector<string> filepath) {
		for(std::size_t i = 0; i < filepath.size(); ++i) {
			i == filepath.size()-1
			? setupShaderFromFile(type, filepath[i])
			: attachShader(shader, type, filepath[i]);
		}
	};
	unload();
	setupShaderFromSource(GL_VERTEX_SHADER, "");
	
	attachShaders(*this, GL_VERTEX_SHADER, s.vertex);
	attachShaders(*this, GL_GEOMETRY_SHADER, s.geometry);
	attachShaders(*this, GL_FRAGMENT_SHADER, s.fragment);
	attachShaders(*this, GL_COMPUTE_SHADER, s.compute);
	linkProgram();
	
	textures_.clear();
	for(auto &&t: s.uniform) {
		if(t.second["type"] != "texture") {
			continue;
		}
		ofTexture tex;
		std::string filename = t.second["value"];
		if(ofLoadImage(tex, filename)) {
			textures_.insert(make_pair(t.first, tex));
		}
	}
}
void Shader::begin() const
{
	int texture_location = 0;
	begin(texture_location);
}
void Shader::begin(int &uniform_texture_location) const
{
	ofShader::begin();
	for(auto &&u : settings_.uniform) {
		auto &&name = u.first;
		auto &&type = u.second["type"];
		auto &&value = u.second["value"];
		if(type == "float") { setUniform1f(name, value); }
		else if(type == "vec2") { setUniform2f(name, value[0], value[1]); }
		else if(type == "vec3") { setUniform3f(name, value[0], value[1], value[2]); }
		else if(type == "vec4") { setUniform4f(name, value[0], value[1], value[2], value[3]); }
		else if(type == "mat3") { setUniformMatrix3f(name, glm::mat3(value[0].get<float>(), value[1].get<float>(), value[2].get<float>(), value[3].get<float>(), value[4].get<float>(), value[5].get<float>(), value[6].get<float>(), value[7].get<float>(), value[8].get<float>())); }
		else if(type == "mat4") { setUniformMatrix4f(name, glm::mat4(value[0].get<float>(), value[1].get<float>(), value[2].get<float>(), value[3].get<float>(), value[4].get<float>(), value[5].get<float>(), value[6].get<float>(), value[7].get<float>(), value[8].get<float>(), value[9].get<float>(), value[10].get<float>(), value[11].get<float>(), value[12].get<float>(), value[13].get<float>(), value[14].get<float>(), value[15].get<float>())); }
		else if(type == "int") { setUniform1i(name, value); }
		else if(type == "ivec2") { setUniform2i(name, value[0], value[1]); }
		else if(type == "ivec3") { setUniform3i(name, value[0], value[1], value[2]); }
		else if(type == "ivec4") { setUniform4i(name, value[0], value[1], value[2], value[3]); }
		else if(type == "texture") { setUniformTexture(name, textures_.find(name)->second, uniform_texture_location++); }
	}
	auto pushGLBoolState = [&](GLenum cap, bool value) {
		auto cache = glIsEnabled(cap);
		value ? glEnable(cap) : glDisable(cap);
		return [cap, cache]() {
			cache ? glEnable(cap) : glDisable(cap);
		};
	};
	for(auto &&state : settings_.gl_state_bool) {
		if(state.first == "point_size") {
			gl_state_restores_.emplace_back(pushGLBoolState(GL_PROGRAM_POINT_SIZE, state.second));
		}
		if(state.first == "point_sprite") {
			gl_state_restores_.emplace_back(pushGLBoolState(GL_POINT_SPRITE, state.second));
		}
	}
}

void Shader::end() const
{
	for(auto &&restore : gl_state_restores_) {
		restore();
	}
	gl_state_restores_.clear();
	ofShader::end();
}
