#pragma once

#include <vector>
#include <map>
#include <string>
#include "ofShader.h"
#include "ofTexture.h"
#include "ofJson.h"

namespace ofx {
namespace ComposableShader {

class Shader : public ofShader
{
public:
	struct Settings {
		std::vector<std::string> vertex, geometry, fragment, compute;
		std::map<std::string, ofJson> gl_state;
		std::map<std::string, ofJson> uniform;
		void loadJson(const ofJson&);
	};
	void setup(const Settings &settings) {
		settings_ = settings;
		reload();
	}
	void reload();
	
	void begin() const;
	void begin(int &uniform_texture_location) const;
	void end() const;

	const std::map<std::string, ofJson>& getUniforms() const { return settings_.uniform; }
	
	bool gui();
	
private:
	Settings settings_;
	std::map<std::string, ofTexture> textures_;
	mutable std::vector<std::function<void()>> gl_state_restores_; 
};
}}

using ofxComposableShader = ofx::ComposableShader::Shader;
