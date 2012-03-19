struct VS_INPUT
{
	float3		position	: POSITION;
	float2		cellUV		: TEXCOORD;
	float2		boardUV		: UV;
};

struct PS_INPUT
{
	float4		position	: SV_POSITION;
	float2		cellUV		: TEXCOORD;
	float2		boardUV		: UV;
	float3		worldPos	: POSITION;
};

RasterizerState NoCulling
{
	CullMode = None;
};

SamplerState LinearSampler 
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

cbuffer cbEveryFrame
{
	matrix		gModel;
	matrix		gMVP;
	float2		gMarkedCell;
	float		gLeft;		// TEST
	float		gUp;
	float		gRight;
	float		gDown;		// END TEST
};

int gWidth;
float gInterval;
float4 gPickColor;
Texture2D gCellTexture;
Texture2D gBoardTexture;


PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output;

	output.position = mul(float4(input.position, 1.0), gMVP);
	output.worldPos = mul(float4(input.position, 1.0), gModel).xyz;
	output.cellUV = input.cellUV;
	output.boardUV = input.boardUV;

	return output;
}

float4 PS(PS_INPUT input) : SV_TARGET0
{
	float4 col = gCellTexture.Sample(LinearSampler, input.cellUV) * gBoardTexture.Sample(LinearSampler, input.boardUV);

	if (input.worldPos.x > gLeft && input.worldPos.x < gRight &&
		input.worldPos.z > gDown && input.worldPos.z < gUp)
	{
		col = col * (1 - gPickColor.w) + gPickColor * gPickColor.w;
		col.w = 1.0;
	}

	return col;
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

