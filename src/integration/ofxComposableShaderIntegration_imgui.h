#pragma once

#include "ofxComposableShader.h"
#include "imgui.h"
#include "ofJson.h"

inline bool ofxComposableShader::gui()
{
	bool ret = false;
	using namespace ImGui;
	auto findGet = [](const ofJson &json, const std::string &key, ofJson fallback) {
		auto found = json.find(key);
		return found != std::end(json) ? *found : fallback;
	};
	for(auto &&u : settings_.uniform) {
		if(u.second.find("imgui") == std::end(u.second)) {
			continue;
		}
		auto &&name = u.first;
		auto &&type = u.second["type"];
		auto &&value = u.second["value"];
		auto &&gui = u.second["imgui"];
		auto gui_type = gui.find("type") == std::end(gui) ? "typical" : gui["type"];
		if(gui_type == "color") {
			std::vector<float> v = value;
			if(type=="vec3" ? ColorEdit3(name.c_str(), v.data()) : ColorEdit4(name.c_str(), v.data())) {
				for(int i = 0; i < value.size(); ++i) {
					value[i] = v[i];
				}
				ret |= true;
			}
			continue;
		}
		else if(gui_type == "combo") {
			int v = value;
			std::vector<std::string> item = gui["item"];
			if(BeginCombo(name.c_str(), item.size() > v ? item[v].c_str() : nullptr)) {
				for(int i = 0; i < item.size(); ++i) {
					if(Selectable(item[i].c_str(), i == v)) {
						value = i;
						ret |= true;
					}
				}
				EndCombo();
			}
			continue;
		}
		float v_speed = findGet(gui, "speed", 1.f);
		float v_step = findGet(gui, "step", 0.f);
		float v_step_fast = findGet(gui, "stepFast", 0.f);
		float v_min = findGet(gui, "min", 0.f);
		float v_max = findGet(gui, "max", v_min);
		std::string format = findGet(gui, "format", "%.3f");
		float power = findGet(gui, "power", 1.f);
		
		ImGuiDataType data_type;
		int num_components;
		if(type == "float" || type == "vec2" || type == "vec3" || type == "vec4"
		   || type == "mat3" || type == "mat4") {
			data_type = ImGuiDataType_Float;
		}
		else if(type == "int" || type == "ivec2" || type == "ivec3" || type == "ivec4") {
			data_type = ImGuiDataType_S32;
		}
		if(type == "float" || type == "int") { num_components = 1; }
		else if(type == "vec2" || type == "ivec2") { num_components = 2; }
		else if(type == "vec3" || type == "ivec3") { num_components = 3; }
		else if(type == "vec4" || type == "ivec4") { num_components = 4; }
		else if(type == "mat3") { num_components = 9; }
		else if(type == "mat4") { num_components = 16; }
		
		auto makeFloatVector = [num_components](const ofJson &json) {
			return num_components > 1 ? json.get<std::vector<float>>() : std::vector<float>{json};
		};
		auto restoreFloatVector = [num_components](ofJson &json, const std::vector<float> &value) {
			if(num_components > 1) {
				for(int i = 0; i < num_components; ++i) {
					json[i] = value[i];
				}
			}
			else {
				json = value[0];
			}
		};
		auto makeIntVector = [num_components](const ofJson &json) {
			return num_components > 1 ? json.get<std::vector<int>>() : std::vector<int>{json};
		};
		auto restoreIntVector = [num_components](ofJson &json, const std::vector<int> &value) {
			if(num_components > 1) {
				for(int i = 0; i < num_components; ++i) {
					json[i] = value[i];
				}
			}
			else {
				json = value[0];
			}
		};
		if(gui_type == "slider") {
			switch(data_type) {
				case ImGuiDataType_Float: {
					auto v = makeFloatVector(value);
					if(SliderScalarN(name.c_str(), data_type, v.data(), num_components, &v_min, &v_max, format.c_str(), power)) {
						restoreFloatVector(value, v);
						ret |= true;
					}
				}	break;
				case ImGuiDataType_S32: {
					auto v = makeIntVector(value);
					if(SliderScalarN(name.c_str(), data_type, v.data(), num_components, &v_min, &v_max, format.c_str(), power)) {
						restoreIntVector(value, v);
						ret |= true;
					}
				}	break;
			}
		}
		else if(gui_type == "input") {
			switch(data_type) {
				case ImGuiDataType_Float: {
					auto v = makeFloatVector(value);
					if(InputScalarN(name.c_str(), data_type, v.data(), num_components, v_step != 0 ? &v_step : nullptr, v_step_fast != 0 ? &v_step_fast : nullptr, format.c_str(), power)) {
						restoreFloatVector(value, v);
						ret |= true;
					}
				}	break;
				case ImGuiDataType_S32: {
					auto v = makeIntVector(value);
					if(InputScalarN(name.c_str(), data_type, v.data(), num_components, v_step != 0 ? &v_step : nullptr, v_step_fast != 0 ? &v_step_fast : nullptr, format.c_str(), power)) {
						restoreIntVector(value, v);
						ret |= true;
					}
				}	break;
			}
		}
		else if(gui_type == "range") {
			assert(num_components == 2);
			switch(data_type) {
				case ImGuiDataType_Float: {
					auto v = makeFloatVector(value);
					if(DragFloatRange2(name.c_str(), &v[0], &v[1], v_speed, v_min, v_max, format.c_str(), format.c_str(), power)) {
						restoreFloatVector(value, v);
						ret |= true;
					}
				}	break;
				case ImGuiDataType_S32: {
					auto v = makeIntVector(value);
					if(DragIntRange2(name.c_str(), &v[0], &v[1], v_speed, v_min, v_max, format.c_str(), format.c_str())) {
						restoreIntVector(value, v);
						ret |= true;
					}
				}	break;
			}
		}
		else /*if(gui_type == "drag")*/ {	// fallback
			switch(data_type) {
				case ImGuiDataType_Float: {
					auto v = makeFloatVector(value);
					if(DragScalarN(name.c_str(), data_type, v.data(), num_components, v_speed, &v_min, &v_max, format.c_str(), power)) {
						restoreFloatVector(value, v);
						ret |= true;
					}
				}	break;
				case ImGuiDataType_S32: {
					auto v = makeIntVector(value);
					if(DragScalarN(name.c_str(), data_type, v.data(), num_components, v_speed, &v_min, &v_max, format.c_str(), power)) {
						restoreIntVector(value, v);
						ret |= true;
					}
				}	break;
			}
		}

	}
	return ret;
}
