Shader "OVRCutoutRamp" {
	Properties {
		_Color ("Main Color", Color) = (1,1,1,1)
		_MainTex ("Base (RGB) Trans (A)", 2D) = "white" {}
		_ColorRamp ("Cutout (A)", 2D) = "white" {}
		_Cutoff ("Alpha cutoff", Range(0,1)) = 0.5
	}
	SubShader {
		Tags {"Queue"="Transparent" "IgnoreProjector"="True" "RenderType"="Transparent"}
        Ztest [Always] 

		CGPROGRAM
		#pragma surface surf Lambert alpha

		sampler2D _MainTex;
		sampler2D _ColorRamp;
		fixed4 _Color;
		float _Cutoff;

		struct Input {
			float2 uv_MainTex;
		};

		void surf (Input IN, inout SurfaceOutput o) {	
			float4 texel = tex2D(_MainTex, IN.uv_MainTex);
			float2 colorIndex = float2( texel[0], _Cutoff );
			float4 outColor = tex2D(_ColorRamp, colorIndex );
			o.Albedo.xyz = half3( outColor.rgb * _Color.rgb );
			o.Alpha = texel.a;
		}
		ENDCG
	} 
	FallBack "Transparent/VertexLit"
}
