struct VS_INPUT
{
	float3		position	: POSITION;
	float4		color		: COLOR;
};

struct PS_INPUT
{
	float4		position	: SV_POSITION;
	float3		worldPos	: POSITION;
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

int gWidth;
float gInterval;

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output;

	output.position = mul(float4(input.position, 1.0), gVP);
	output.worldPos = input.position;
	output.color = input.color;

	return output;
}

float4 PS(PS_INPUT input) : SV_Target
{
	float x = input.worldPos.x;
	float z = input.worldPos.z;
	int roundX = round(x);
	int roundZ = round(z);

	if(abs(roundX - x) <= gInterval && roundX % gWidth == 0)
		return float4(0.0, 0.0, 0.0, 1.0);
	if(abs(roundZ - z) <= gInterval && roundZ % gWidth == 0)
		return float4(0.0, 0.0, 0.0, 1.0);

	return input.color;
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

