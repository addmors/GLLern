#version 420 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;


uniform vec3 viewPos;
// �������� ��������� �����;
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 Diffuse, float Specular);
//�������� ������������� ����� 
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir,vec3 Diffuse, float Specular);
//�������� ��������� ����� 
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir,vec3 Diffuse, float Specular);

void main()
{             
    // retrieve data from gbuffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;


    vec3 viewDir = normalize(viewPos- FragPos);

	vec3 result = CalcDirLight(dirLight, normal, viewDir);
	for(int i = 0; i < NR_POINT_LIGHTS; i++)		
		result += CalcPointLight(pointLights[i], normal, FragPos, viewDir); 
	result += CalcSpotLight(spotlight, normal, FragPos, viewDir);
	color = vec4(result, 1.0f)

}



vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir , vec3 Diffuse, float Specular)
{
    vec3 lightDir = normalize( (-light.direction));
    // ��������� ���������
    float diff = max(dot(normal, lightDir), 0.0);
    // ��������� ���������� ������
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // ����������� ����������
    vec3 ambient  = light.ambient  * Diffuse;
    vec3 diffuse  = light.diffuse  * diff * Diffuse;
    vec3 specular = light.specular * spec * Specular;
    return (ambient + diffuse + specular);
} 
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 Diffuse, float Specular)
{
    vec3 lightDir = normalize((light.position - fragPos));
    // ��������� ���������
    float diff = max(dot(lightDir, normal), 0.0);
    // ��������� ���������� ������
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // ���������
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // ����������� ����������
    vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse1, TexCoord));
    vec3 diffuse  = light.diffuse  * diff * Diffuse;
    vec3 specular = light.specular * spec * Specular;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 Diffuse, float Specular){
	
	vec3 lightDir = normalize((light.position - fragPos));
	float theta = dot(lightDir, normalize(-light.direction));
	//������������� ��� �������� ���������� � ����� ��������� (����������� �����)
	float epsilon   = light.cutOff - light.outercutOff;
	float intensity = clamp((theta - light.outercutOff)/epsilon , 0.0, 1.0); 
	if (theta > light.outercutOff){ 
	//������� ����
	vec3 ambient = light.ambient * Diffuse;
	//��������� ���������
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * Diffuse;
	// ���������� ������ �� ��������� � ��������� � �������� 
	vec3 reflectDir = reflect(-lightDir, normal);
	//���� ����� 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	//���� �� �������� ����� � ���� ������
	vec3 specular = light.specular * spec * Specular;
	//��������� ��� ��������� 
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
		vec3 ambient = light.ambient * Diffuse;
		return ambient;
	}
}