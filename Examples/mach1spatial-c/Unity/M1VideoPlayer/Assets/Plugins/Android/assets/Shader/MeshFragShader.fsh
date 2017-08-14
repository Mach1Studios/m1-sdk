#extension GL_OES_EGL_image_external : require
uniform samplerExternalOES sTexture;

uniform lowp float AlphaValue;

varying mediump vec2   TexCoord;

void main()
{
	lowp vec4 color = texture2D(sTexture, TexCoord) ;
	
	color = vec4(color.rgb,color.a * AlphaValue);
    gl_FragColor = color;
    
}
