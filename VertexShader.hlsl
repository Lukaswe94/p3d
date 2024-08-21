struct VSIn
{
    uint vertexId : SV_VertexID;
};

struct VSOut
{
    float4 pos : SV_Position;
    float4 color : color;
};

VSOut VSmain(VSIn input)
{
    VSOut output;
    output.pos = float4(0, 0, 0, 0);
    output.color = float4(0, 0, 0, 0);
    
    if (input.vertexId == 0)
        output.pos = float4(0.0, 0.5, 0.5, 1);
    else if (input.vertexId == 2)
        output.pos = float4(0.5, -0.5, 0.5, 1);
    else if (input.vertexId == 1)
        output.pos = float4(-0.5, -0.5, 0.5, 1);
    
    output.color = clamp(output.pos, 0, 1);
    
    return output;
}