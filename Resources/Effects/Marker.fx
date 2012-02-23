struct VS_INPUT
{
	float3		position	: POSITION;
};

struct PS_INPUT
{
	float4		position	: SV_POSITION;
};

RasterizerState NoCulling
{
	CullMode = None;
};

cbuffer cbEveryFrame
{
	matrix		gWVP;
	matrix		gWorld;
};

float4		gColor;

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output;
	
	output.position = mul(float4(input.position, 1.0), gWVP);

	return output;
}

float4 PS(PS_INPUT input) : SV_TARGET0
{
	return gColor;
}

technique10 DrawTechnique
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS()));

		SetRasterizerState(NoCulling);
	}
}

