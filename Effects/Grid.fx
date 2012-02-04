struct VS_INPUT
{
	float3		position	: POSITION;
	float4		color		: COLOR;
};

struct PS_INPUT
{
	float4		position	: SV_POSITION;
	float4		color		: COLOR;
};

RasterizerState NoCulling
{
	CullMode = None;
};

cbuffer cbEveryFrame
{
	matrix		gVP;
	//matrix		gWorld;
};

Texture2D textureGrass;

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output;

	float3 temp = float3(input.position.x, input.position.z, input.position.y);
	//output.position = float4(temp, 1.0);
	output.position = mul(float4(temp, 1.0), gVP);
	//output.position = mul(float4(input.position, 1.0), gVP);
	//output.position = float4(input.position, 1.0);
	output.color = input.color;

	return output;
}

float4 PS(PS_INPUT input) : SV_Target
{
	return float4(1.0, 1.0, 1.0, 1.0); //input.color;
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

