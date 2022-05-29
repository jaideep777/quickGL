#version 130

in  vec3 ex_pos;
in  vec2 ex_UV;
in  vec4 ex_col;
in  vec3 ex_normal;

out vec4 outColor;

uniform sampler2D tex;

uniform mat4 WM;   // object transformation matrix: world*model
uniform mat3 normalMatrix; // matrix for transforming normals (rotation only)

uniform float mixer = 0.99;

void main()
{	
	vec3 frag_pos = vec3(WM * vec4(ex_pos,1)); // object position in world coordinates (without camera transformations)
	vec3 normal = normalize(normalMatrix * ex_normal); // normal in world coordinates (only rotation transformation)
	
	vec3 light_pos = vec3(1.f,1.f,1.f);
	vec3 surfaceToLight = normalize(light_pos - frag_pos);
	
    // calculate the cosine of the angle of incidence
    float brightness1 = dot(normal, surfaceToLight); // / (length(surfaceToLight) * length(normal));  // lighting outer surface
    float brightness2 = dot(-normal, surfaceToLight); // / (length(surfaceToLight) * length(normal)); // lighting inner surface
    float brightness = max(brightness1, brightness2);
    brightness = clamp(brightness, 0, 1);

	vec4 texColor = mix(texture(tex, ex_UV), ex_col, mixer); 
    outColor = vec4(brightness * texColor.rgb, texColor.a);
}


