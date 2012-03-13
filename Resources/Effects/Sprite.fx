struct VS_INPUT
{
	float2		position	: POSITION;
	float2		uv			: TEXCOORD;
};

struct PS_INPUT
{
	float4		position	: SV_POSITION;
	float2		uv			: TEXCOORD;
};

RasterizerState NoCulling
{
	CullMode = None;
};

SamplerState linearSampler {
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

BlendState blend
{
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
	BlendOp = ADD;
	SrcBlendAlpha = ZERO;
	DestBlendAlpha = ZERO;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;
};

cbuffer cbEveryFrame
{
	float4		gTintColor;
	matrix		gModel;
}

Texture2D image;

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output;

	output.position = mul(float4(float3(input.position, 1.0), 1.0), gModel);
	output.uv = input.uv;

	return output;
}

float4 PS(PS_INPUT input) : SV_TARGET0
{
	float4 returnColor = image.Sample(linearSampler, input.uv);
	return returnColor * gTintColor;
}

technique10 DrawTechnique
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS()));

		SetRasterizerState(NoCulling);
		SetBlendState(blend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
	}
}
