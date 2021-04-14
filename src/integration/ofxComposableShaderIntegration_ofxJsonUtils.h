#pragma once

#include "ofxComposableShader.h"

#include "ofJson.h"
#include "ofxJsonUtils.h"

inline void ofxComposableShader::Settings::loadJson(const ofJson &json) {
	ofxJsonUtils::load(json, kv(vertex), kv(geometry), kv(fragment), kv(compute), kv(uniform), kv(gl_state_bool), kv(gl_state));
}
