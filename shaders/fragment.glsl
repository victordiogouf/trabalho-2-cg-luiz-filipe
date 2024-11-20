#version 450

in vec3 position;
in vec3 normal;

uniform vec3 camera_position;

out vec4 frag_color;

void main() {
  vec3 ambient = vec3(0.08, 0.11, 0.9);

  vec3 norm = normalize(normal);
  vec3 light_dir = normalize(vec3(1.0, 1.0, 1.0));
  vec3 diffuse = max(dot(norm, light_dir), 0.0) * vec3(1.0, 1.0, 1.0);

  vec3 view_dir = normalize(camera_position - position);
  vec3 reflect_dir = reflect(-light_dir, norm);
  vec3 specular = pow(max(dot(view_dir, reflect_dir), 0.0), 32) * vec3(0.9, 0.9, 1.0);

  frag_color = vec4(ambient + diffuse + specular, 1.0);
}
