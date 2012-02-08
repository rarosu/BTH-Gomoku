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
	matrix		gVP;
	
};

float4		gColor		= float4(0.2, 0.2, 0.2, 1.0);

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output;
	
	//float3 temp = float3(input.position.x, input.position.z, input.position.y);
	output.position = mul(float4(input.position, 1.0), gVP);
	//output.position = mul(float4(temp, 1.0), gVP);

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

