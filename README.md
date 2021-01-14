# ofxComposableShader

You can compose ofShader by simple settings like this;
```json
{
	"vertex" : [
		"point_sprite.vert"
	],
	"fragment" : [
		"mono_color.frag"
	],
	"uniform" : {
		"point_size" : {
			"type" : "float",
			"value" : 1,
			"imgui" : {
				"type" : "drag",
				"speed" : 1,
				"min" : 0,
				"max" : 100
			}
		},
		"color" : {
			"type" : "color",
			"value" : [1,1,1,1],
			"imgui" : {}
		}
	},
	"gl_state_bool" : {
		"point_size" : true,
		"point_sprite" : true
	}
}
```

Then you get this result;

![screenshot](https://user-images.githubusercontent.com/1306139/104599153-c5f4e980-56ba-11eb-979e-8cc327356c5a.png)
