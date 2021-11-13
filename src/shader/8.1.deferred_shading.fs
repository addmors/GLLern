#version 420 core
out vec4 FragColor;

struct PointLight
{
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec2 gScreenSize;

uniform sampler2D gPosition;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gNormal;


uniform PointLight pointLight;
uniform vec3 viewPos;

vec2 CalcTexCoord()
{
    return gl_FragCoord.xy / gScreenSize;
}

// Прототип фонарного света;
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 Diffuse, float Specular);

void main()
{         
	vec2 TexCoords = CalcTexCoord();
    // retrieve data from gbuffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
   
    vec3 normal = texture(gNormal, TexCoords).rgb;
    vec3 diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    float specular = texture(gAlbedoSpec, TexCoords).a;
    vec3 viewDir = normalize(viewPos- FragPos);

	FragColor = vec4(CalcPointLight(pointLight, normal, FragPos, viewDir, diffuse, specular), 1.0);

}



vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 Diffuse, float Specular)
{
    vec3 lightDir = normalize((light.position - fragPos));
    // диффузное освещение
    float diff = max(dot(lightDir, normal), 0.0);
    // освещение зеркальных бликов
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 20);
    // затухание
    float distance    = length(light.position - fragPos);
    float attenuation = 20.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * distance * distance);    
    // комбинируем результаты
    vec3 ambient  = light.ambient  * Diffuse * attenuation;
    vec3 diffuse  = light.diffuse  * diff * Diffuse * attenuation;
    vec3 specular = light.specular * spec * Specular * attenuation;
   
    return (ambient + diffuse + specular);
} 
