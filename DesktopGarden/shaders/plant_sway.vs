#version 330

in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec4 vertexColor;

out vec2 fragTexCoord;
out vec4 fragColor;

uniform mat4 mvp;
uniform float time;

void main()
{
  vec3 pos = vertexPosition;

  float heightFactor = 1.0 - vertexTexCoord.y;
  heightFactor = pow(heightFactor, 2.0);
	
	float strength = 4.0;
	float speed = 1.0;
	float frequency = 3.0;

  float sway = sin(time * speed + pos.y * frequency) * strength * heightFactor;

  pos.x += sway;

  gl_Position = mvp * vec4(pos, 1.0);

  fragTexCoord = vertexTexCoord;
  fragColor = vertexColor;
}
