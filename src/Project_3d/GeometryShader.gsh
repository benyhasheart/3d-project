struct GSOutput
{
	float4 pos : SV_POSITION;
};

struct VS_DATA
{
	float4 position : SV_POSITION; // vertex pos
	float4 diffuse : COLOR0; // vertex color
};

[maxvertexcount(9)]
void main(
	triangle VS_DATA input[3] ,
	inout TriangleStream< GSOutput > output
)
{
	// primitive 의 중점을 찾는다.
	float3 centerPos = (input[0].position.xyz + input[1].position.xyz + input[2].position.xyz) / 3.0f;
	

	for (uint i = 0; i < 3; i++)
	{
		
		GSOutput element;
		element.pos = input[i].position;
		output.Append(element);

		int nextIndex = (i+1)%3;
		element.pos = input[nextIndex].position;
		output.Append(element);

		element.pos = float4(centerPos, 1);
		output.Append(element);

	}

	output.RestartStrip();
}