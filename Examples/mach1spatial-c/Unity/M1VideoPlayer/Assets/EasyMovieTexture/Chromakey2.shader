// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

//Shader "Unlit/Chromakey2"
Shader "Unlit/ChromaKey2" {

    Properties {
        _MainTex ("Base (RGB)", 2D) = "white" {}
        _thresh ("Threshold", Range (0, 16)) = 0.8
        _slope ("Slope", Range (0, 1)) = 0.2
        _keyingColor ("Key Colour", Color) = (1,1,1,1)
    }
    
    SubShader {
        Tags {"Queue"="Transparent" "IgnoreProjector"="True" "RenderType"="Transparent"}
        LOD 100
        
        Lighting Off
        ZWrite Off
        AlphaTest Off
        Blend SrcAlpha OneMinusSrcAlpha 
        
        Pass {
            CGPROGRAM
                #pragma vertex vert
                #pragma fragment frag
                #pragma fragmentoption ARB_precision_hint_fastest

                sampler2D _MainTex;
				float4 _MainTex_ST;
                float3 _keyingColor;
                float _thresh; // 0.8
                float _slope; // 0.2

                #include "UnityCG.cginc"

				struct appdata
				{
					float4 vertex : POSITION;
					float2 uv : TEXCOORD0;
				};

				struct v2f
				{
					float2 uv : TEXCOORD0;
					UNITY_FOG_COORDS(1)
						float4 vertex : SV_POSITION;
				};

				v2f vert(appdata v)
				{
					v2f o;
					//o.vertex = UnityObjectToClipPos(v.vertex);
					o.vertex = UnityObjectToClipPos(float4(v.vertex.xyz, 1.0));
					o.uv = TRANSFORM_TEX(v.uv, _MainTex);
					UNITY_TRANSFER_FOG(o, o.vertex);
					return o;
				}

                float4 frag(v2f i) : SV_Target{
                    float3 input_color = tex2D(_MainTex, i.uv).rgb;
                    float d = abs(length(abs(_keyingColor.rgb - input_color.rgb)));
                    float edge0 = _thresh * (1.0 - _slope);
                    float alpha = smoothstep(edge0, _thresh, d);
                    return float4(input_color, alpha);
                }
            ENDCG
        }
    } 
    
    FallBack "Unlit"
}