struct VS_INPUT
{
	float2		position	: POSITION;
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
	float4		consoleColor;
}

Texture2D textureGrass;

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output;

	output.position = float4(float3(input.position, 0.0), 1.0);

	return output;
}

float4 PS(PS_INPUT input) : SV_TARGET0
{
	return consoleColor;
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

