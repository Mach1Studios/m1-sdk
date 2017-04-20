// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

Shader "Unlit/Unlit_YUV"
{
	Properties
	{
		_MainTex ("Texture", 2D) = "black" {}
		_Y("Y", 2D) = "black" {}
		_U("U", 2D) = "gray" {}
		_V("V", 2D) = "gray" {}
	}
	SubShader
	{
		Tags { "RenderType"="Opaque" }
		LOD 100

		Pass
		{
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			// make fog work
			#pragma multi_compile_fog
			
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

			sampler2D _MainTex;
			sampler2D _Y;
			sampler2D _U;
			sampler2D _V;
			float4 _MainTex_ST;

		
			v2f vert (appdata v)
			{
				v2f o;
				//o.vertex = UnityObjectToClipPos(v.vertex);
				o.vertex = UnityObjectToClipPos(float4(v.vertex.xyz, 1.0));
				o.uv = TRANSFORM_TEX(v.uv, _MainTex);
				UNITY_TRANSFER_FOG(o,o.vertex);
				return o;
			}

			
			fixed4 frag (v2f i) : SV_Target
			{

				float y = tex2D(_Y, i.uv).a;
				float u = tex2D(_U, i.uv).a * 0.872 - 0.436;		
				float v = tex2D(_V, i.uv).a * 1.230 - 0.615;		
	
				float r = clamp(y + 1.13983 * v, 0.0, 1.0);
				float g = clamp(y - 0.39465 * u - 0.58060 * v, 0.0, 1.0);
				float b = clamp(y + 2.03211 * u, 0.0, 1.0);
				
				fixed4 col = fixed4(r, g, b, 1.0);

				return col;
			}
			ENDCG
		}
	}
}