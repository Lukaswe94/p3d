// NoBuffer.fx
// Copyright (c) 2004 Microsoft Corporation. All rights reserved.
//

struct VSOut
{
    float4 pos : SV_Position;
    float4 color : color;
};

struct PSOut
{
    float4 color : SV_Target0;
};


PSOut PSmain(VSOut input)
{
    PSOut output;
    output.color = input.color;
    return output;
}