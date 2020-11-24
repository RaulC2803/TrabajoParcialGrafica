#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightcolor;

//float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir, vec3 normal){
//    float bias =  max(0.05 * (1.0 - dot(normal, lightDir)), 0.005); 
//
//    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
//    
//    projCoords = projCoords * 0.5 + 0.5;
//    
//    float closestDepth = texture(shadowMap, projCoords.xy).r; 
//  
//    float currentDepth = projCoords.z;
//   
//    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0; 
//
//    if(projCoords.z > 1.0)
//        shadow = 0.0;
//
//    return shadow;
//}

float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir, vec3 normal){
   vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
     projCoords = projCoords * 0.5 + 0.5;

     float closestDepth = texture(shadowMap, projCoords.xy).r; 

     float currentDepth = projCoords.z;
     float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
     float shadow = 0.0;
     vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
     for(int x = -1; x <= 1; ++x){
         for(int y = -1; y <= 1; ++y){
             float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
             shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
         }    
     }
     shadow /= 9.0;
     if(projCoords.z > 1.0)
         shadow = 0.0;
         
     return shadow;
}

void main(){          
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = lightcolor;

    vec3 ambient = 0.3 * texture(diffuseTexture, fs_in.TexCoords).rgb;
  
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor * texture(diffuseTexture, fs_in.TexCoords).rgb;
  
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec =  pow(max(dot(viewDir, reflectDir), 0.0), 64.0);;
    vec3 halfwayDir = normalize(lightDir + viewDir); 
    vec3 specular = spec * lightColor * texture(specularTexture,fs_in.TexCoords).rgb;    
   
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace, lightDir, normal);                      
    vec3 lighting = (ambient + (1.0- shadow) * (diffuse + specular));    
    
    FragColor = vec4(lighting, 1.0);
}