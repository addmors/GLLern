#version 420 core
//Направленный свет.
struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
uniform DirLight dirLight;
//Точечный источник света
struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
#define NR_POINT_LIGHTS 4  
uniform PointLight pointLights[NR_POINT_LIGHTS];
struct SpotLight{
	vec3 position;
	vec3 direction;
	float constant;
	float linear;
	float quadratic;
	float cutOff;
	float outercutOff;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform SpotLight spotlight;
struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
}; 
out vec4 color;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;
uniform Material material;
// Прототип фонарного света;
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
//Прототип направленного света 
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
//Прототип точечного света 
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
void main()
{
	//Вектро от фрагмента к источкнику 
	vec3 viewDir = normalize(-FragPos);
	vec3 norm = normalize(Normal);
	vec3 result = CalcDirLight(dirLight, norm, viewDir);
	for(int i = 0; i < NR_POINT_LIGHTS; i++)
      result += CalcPointLight(pointLights[i], norm, FragPos, viewDir); 
	result += CalcSpotLight(spotlight, norm, FragPos, viewDir);
	color = vec4(result, 1.0f);
}
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // диффузное освещение
    float diff = max(dot(normal, lightDir), 0.0);
    // освещение зеркальных бликов
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // комбинируем результаты
    vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse1, TexCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse1, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoord));
    return (ambient + diffuse + specular);
} 
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // диффузное освещение
    float diff = max(dot(normal, lightDir), 0.0);
    // освещение зеркальных бликов
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // затухание
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // комбинируем результаты
    vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse1, TexCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse1, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoord));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
	
	vec3 lightDir = normalize(light.position - fragPos);
	float theta = dot(lightDir, normalize(-light.direction));
	//Интенсивность для плавного затемнения к кроям окужности (Инерполяция света)
	float epsilon   = light.cutOff - light.outercutOff;
	float intensity = clamp((theta - light.outercutOff)/epsilon , 0.0, 1.0); 
	if (theta > light.outercutOff){ 
	//фоновый свет
	vec3 ambient = light.ambient * texture(material.texture_diffuse1, TexCoord).rgb;
	//диффузное освещение
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse1, TexCoord).rgb;
	vec3 viewDir = normalize(-fragPos);
	// Отраженный вектор от источника к фрагменту и нормалью 
	vec3 reflectDir = reflect(-lightDir, normal);
	//Сила блика 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	//Блик на бликовую карту и цвет отблка
	vec3 specular = light.specular * spec * texture(material.texture_specular1, TexCoord).rgb;
	//затухание при дальности 
	float distance = length(light.position-fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));  
	ambient  *= attenuation; 
	diffuse  *= attenuation;
	specular *= attenuation;
	diffuse *= intensity;
	specular *= intensity;
	vec3 result = ambient + diffuse + specular;
	return result;
	}
	else{
		vec3 ambient = light.ambient * texture(material.texture_diffuse1, TexCoord).rgb;
		return ambient;
	}
}