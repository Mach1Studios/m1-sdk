attribute highp   vec3  inVertex;
attribute mediump vec3  inNormal;
attribute mediump vec2  inTexCoord;



uniform highp   mat4  MVPMatrix;
uniform mediump vec2 TexCoordMove; 

varying mediump vec2   TexCoord;


void main()
{
	highp vec4 vPos = vec4(0,0,0,1);
	
	vPos.x = ( inTexCoord.x * 2.0 - 1.0 );
	vPos.y = ( inTexCoord.y * 2.0 - 1.0 );
	
	gl_Position = vPos;

	// Pass through texcoords
	//inTexCoord.x += TexCoordMove.x;
	
	
	 mediump vec4 vec4Temp  = vec4(inTexCoord.x - TexCoordMove.x,inTexCoord.y - TexCoordMove.y,0,1.0);
	 vec4Temp  = MVPMatrix * vec4Temp;
	 
	 vec4Temp.xyz = vec4Temp.xyz / vec4Temp.w;
	 TexCoord = vec4Temp.xy; 
	 

	//TexCoord = inTexCoord;
	
}