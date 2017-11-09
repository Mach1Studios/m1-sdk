
// Unlit shader. Simplest possible textured shader.
// - SUPPORTS lightmap
// - no lighting
// - no per-material color

Shader "Custom/Unlit_SphereInside" {
Properties {
	_MainTex ("Base (RGB)", 2D) = "white" {}
}

SubShader {
	Tags { "RenderType"="Opaque" }
	LOD 100
	
	// Non-lightmapped
	Pass {
		
		Lighting Off
		Cull Front
		SetTexture [_MainTex] { combine texture } 
	}
	
	
	

}
}



