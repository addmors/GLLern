#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
    float visibility;
    float visibilityToPlane;
} fs_in;

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

float shininess = 2.0;
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 totalColor);
//Прототип направленного света 
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 totalColor);
//Прототип точечного света 
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 totalColor);

uniform sampler2D blendMap;
uniform sampler2D backgroundTexture;
uniform sampler2D rTexture;
uniform sampler2D gTexture;
uniform sampler2D bTexture;


uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 skyColor;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)

    float bias = max(0.05 * (1.0 - dot(normalize(fs_in.Normal), normalize(lightPos - fs_in.FragPos))), 0.005);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

void main()
{   
    vec4 blendMapColour = texture(blendMap, fs_in.TexCoords);
    float backTextureAmount = 1-(blendMapColour.r+ blendMapColour.g+blendMapColour.b);
    vec2 tiledCoord = fs_in.TexCoords * 70.0;
    vec4 backGroundTextureColour = texture(backgroundTexture, tiledCoord)*backTextureAmount;
    vec4 rTextureClour = texture(rTexture, tiledCoord)*blendMapColour.r;
    vec4 gTextureClour = texture(gTexture, tiledCoord)*blendMapColour.g;
    vec4 bTextureClour = texture(bTexture, tiledCoord)*blendMapColour.b;
    vec4 totalColor = rTextureClour+gTextureClour+bTextureClour+backGroundTextureColour;
    //vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    
    vec3 viewDir = normalize(-fs_in.FragPos);
    vec3 normal = normalize(fs_in.Normal);
    
	vec3 result = CalcDirLight(dirLight, normal, viewDir, totalColor.rgb);
	for(int i = 0; i < NR_POINT_LIGHTS; i++)		
		result += CalcPointLight(pointLights[i], normal, fs_in.FragPos, viewDir, totalColor.rgb); 
	result += CalcSpotLight(spotlight, normal, fs_in.FragPos, viewDir, totalColor.rgb);
    // calculate shadow
    FragColor = vec4(result, 1.0);
    FragColor = mix(vec4(skyColor,1.0),FragColor, fs_in.visibility);
    //FragColor = mix(FragColor ,vec4(1.0,1.0,1.0,1.0), fs_in.visibilityToPlane);
}


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 totalColor)
{
    vec3 lightDir = normalize( (-light.direction));
    // диффузное освещение
    float diff = max(dot(normal, lightDir), 0.0);
    // освещение зеркальных бликов
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // комбинируем результаты
    vec3 ambient  = light.ambient  * totalColor;
    vec3 diffuse  = light.diffuse  * diff * totalColor;
    
    vec3 specular = light.specular * spec * 0.02;
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);  
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular));
    return lighting;
} 

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 totalColor)
{
    vec3 lightDir = normalize((light.position - fragPos));
    // диффузное освещение
    float diff = max(dot(normal, lightDir), 0.0);
    // освещение зеркальных бликов
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // затухание
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // комбинируем результаты
    vec3 ambient  = light.ambient  * totalColor;
    vec3 diffuse  = light.diffuse  * totalColor;
    vec3 specular = light.specular * spec * 0.5;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 totalColor){
	
	vec3 lightDir = normalize((light.position - fragPos));
	float theta = dot(lightDir, normalize(-light.direction));
	//Интенсивность для плавного затемнения к кроям окужности (Инерполяция света)
	float epsilon   = light.cutOff - light.outercutOff;
	float intensity = clamp((theta - light.outercutOff)/epsilon , 0.0, 1.0); 
	if (theta > light.outercutOff){ 
	//фоновый свет
	vec3 ambient = light.ambient * totalColor;
	//диффузное освещение
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * totalColor;
	vec3 viewDir = normalize(-fragPos);
	// Отраженный вектор от источника к фрагменту и нормалью 
	vec3 reflectDir = reflect(-lightDir, normal);
	//Сила блика 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	//Блик на бликовую карту и цвет отблка
    vec3 specular = light.specular * spec * 0.2;
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
		vec3 ambient = light.ambient * totalColor;
		return ambient;
	}
}