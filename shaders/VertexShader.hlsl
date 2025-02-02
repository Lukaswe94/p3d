struct VSIn
{
    uint vertexId : SV_VertexID;
    float3 pos : POSITION;
    float3 col : COLOR;
};

struct VSOut
{
    float4 pos : SV_Position;
    float4 color : COLOR0;
};

VSOut VSmain(VSIn input)
{
    VSOut output;
    output.pos = float4(input.pos, 1);
    output.color = float4(input.col, 1);
    
    return output;
}