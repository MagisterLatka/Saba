#include <pch.h>
#include "OpenGLRendererAPI.h"

#include <Saba/Renderer/Renderer.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Saba {

static GLenum GetComparisonFunc(RendererAPI::ComparisonFunc func) {
    switch (func) {
        case RendererAPI::ComparisonFunc::Never:             return GL_NEVER;
        case RendererAPI::ComparisonFunc::Less:              return GL_LESS;
        case RendererAPI::ComparisonFunc::Equal:             return GL_EQUAL;
        case RendererAPI::ComparisonFunc::LessOrEqual:       return GL_LEQUAL;
        case RendererAPI::ComparisonFunc::Greater:           return GL_GREATER;
        case RendererAPI::ComparisonFunc::GreaterOrEqual:    return GL_GEQUAL;
        case RendererAPI::ComparisonFunc::NotEqual:          return GL_NOTEQUAL;
        case RendererAPI::ComparisonFunc::Always:            return GL_ALWAYS;
        default: break;
    }
    SB_CORE_THROW_INFO("Invalid comparison function");
    return GL_LESS;
}
static GLenum GetStencilOp(RendererAPI::StencilOperation op) {
    switch (op) {
        case RendererAPI::StencilOperation::Keep:            return GL_KEEP;
        case RendererAPI::StencilOperation::Zero:            return GL_ZERO;
        case RendererAPI::StencilOperation::Replace:         return GL_REPLACE;
        case RendererAPI::StencilOperation::Increment:       return GL_INCR;
        case RendererAPI::StencilOperation::IncrementClamp:  return GL_INCR_WRAP;
        case RendererAPI::StencilOperation::Decrement:       return GL_DECR;
        case RendererAPI::StencilOperation::DecrementClamp:  return GL_DECR_WRAP;
        case RendererAPI::StencilOperation::Invert:          return GL_INVERT;
        default: break;
    }
    SB_CORE_THROW_INFO("Invalid stencil operation");
    return GL_KEEP;
}
static GLenum GetBlendOption(RendererAPI::BlendOption op) {
    switch (op) {
        case RendererAPI::BlendOption::Zero:                     return GL_ZERO;
        case RendererAPI::BlendOption::One:                      return GL_ONE;
        case RendererAPI::BlendOption::SourceColor:              return GL_SRC_COLOR;
        case RendererAPI::BlendOption::SourceColorInvert:        return GL_ONE_MINUS_SRC_COLOR;
        case RendererAPI::BlendOption::SourceAlpha:              return GL_SRC_ALPHA;
        case RendererAPI::BlendOption::SourceAlphaInvert:        return GL_ONE_MINUS_SRC_ALPHA;
        case RendererAPI::BlendOption::DestinationColor:         return GL_DST_COLOR;
        case RendererAPI::BlendOption::DestinationColorInvert:   return GL_ONE_MINUS_DST_COLOR;
        case RendererAPI::BlendOption::DestinationAlpha:         return GL_DST_ALPHA;
        case RendererAPI::BlendOption::DestinationAlphaInvert:   return GL_ONE_MINUS_DST_ALPHA;
        case RendererAPI::BlendOption::BlendFactor:              return GL_CONSTANT_COLOR;
        case RendererAPI::BlendOption::BlendFactorInvert:        return GL_ONE_MINUS_CONSTANT_COLOR;
        default: break;
    }
    SB_CORE_THROW_INFO("Invalid blend option");
    return GL_ONE;
}
static GLenum GetBlendOp(RendererAPI::BlendOperation op) {
    switch (op) {
        case RendererAPI::BlendOperation::Add:                return GL_FUNC_ADD;
        case RendererAPI::BlendOperation::Subtract:           return GL_FUNC_SUBTRACT;
        case RendererAPI::BlendOperation::ReverseSubtract:    return GL_FUNC_REVERSE_SUBTRACT;
        case RendererAPI::BlendOperation::Min:                return GL_MIN;
        case RendererAPI::BlendOperation::Max:                return GL_MAX;
        default: break;
    }
    SB_CORE_THROW_INFO("Invalid blend operation");
    return GL_FUNC_ADD;
}
static void OpenGLMessageCallback(GLenum source, [[maybe_unused]] GLenum type, [[maybe_unused]] GLuint id,
    GLenum severity, [[maybe_unused]] GLsizei length, const GLchar* message, [[maybe_unused]] const void* userParam)
{
    const char* Source;

    switch (source) {
        case GL_DEBUG_SOURCE_API:               Source = "API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:     Source = "WINDOW SYSTEM"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:   Source = "SHADER COMPILER"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:       Source = "THIRD PARTY"; break;
        case GL_DEBUG_SOURCE_APPLICATION:       Source = "APPLICATION"; break;
        case GL_DEBUG_SOURCE_OTHER:
        default:                                Source = "UNKNOWN"; break;
    }

    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:            SB_CORE_CRITICAL("({0}): {1}", Source, message); break;
        case GL_DEBUG_SEVERITY_MEDIUM:          SB_CORE_ERROR("({0}): {1}", Source, message); break;
        case GL_DEBUG_SEVERITY_LOW:             SB_CORE_WARN("({0}): {1}", Source, message); break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
        default:                                SB_CORE_TRACE("({0}): {1}", Source, message); break;
    }
}

void OpenGLRendererAPI::Init() {
#if defined(SB_DEBUG)
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(OpenGLMessageCallback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif

    InitShaders();
}
void OpenGLRendererAPI::Shutdown() {}

void OpenGLRendererAPI::Draw(Topology topology, uint32_t verticesCount) {
    glDrawArrays(GetTopology(topology), 0, static_cast<int>(verticesCount));
}
void OpenGLRendererAPI::DrawIndexed(Topology topology, uint32_t indicesCount) {
    glDrawElements(GetTopology(topology), static_cast<int>(indicesCount), GL_UNSIGNED_INT, nullptr);
}
void OpenGLRendererAPI::DrawIndexedInstanced(Topology topology, uint32_t indicesCount, uint32_t instancesCount) {
    glDrawElementsInstanced(GetTopology(topology), static_cast<int>(indicesCount), GL_UNSIGNED_INT, nullptr, static_cast<int>(instancesCount));
}

uint32_t OpenGLRendererAPI::GetTopology(Topology topology) {
    switch (topology) {
        case RendererAPI::Topology::None:           SB_CORE_THROW_INFO("None topology cannot be used"); return 0u;
        case RendererAPI::Topology::Points:         return GL_POINTS;
        case RendererAPI::Topology::Lines:          return GL_LINES;
        case RendererAPI::Topology::LineStrip:      return GL_LINE_STRIP;
        case RendererAPI::Topology::Triangles:      return GL_TRIANGLES;
        case RendererAPI::Topology::TriangleStrip:  return GL_TRIANGLE_STRIP;
    }
    SB_CORE_THROW_INFO("Unknown topology");
    return 0u;
}

void OpenGLRendererAPI::InitShaders() {
    const std::string vertexFullscreen = R"(
        #version 460 core

        layout(location = 0) in vec4 i_Pos;
        layout(location = 1) in vec2 i_UV;

        out Data {
            vec2 uv;
        } vs_out;

        void main() {
            gl_Position = i_Pos;
            vs_out.uv = i_UV;
        }
    )";
    const std::string fragmentFullscreen = R"(
        #version 460 core

        layout(location = 0) out vec4 o_Color;

        in Data {
            vec2 uv;
        } fs_in;

        layout(binding = 0) uniform sampler2D u_Tex;

        void main() {
            o_Color = texture(u_Tex, fs_in.uv);
        }
    )";

    Renderer::GetShaderLibrary().Load("fullscreenQuadShader", vertexFullscreen, fragmentFullscreen);

    const std::string vertexQuad = R"(
        #version 460 core

        layout(location = 0) in vec4 i_Pos;
        layout(location = 1) in vec4 i_Color;
        layout(location = 2) in vec2 i_UV;
        layout(location = 3) in uint i_TID;
        layout(location = 4) in float i_TillingFactor;
        layout(location = 5) in uvec4 i_ID;

        layout(std140, binding = 0) uniform RendererData {
            mat4 u_ViewProjMat;
        };

        out Data {
            vec4 color;
            vec2 uv;
            flat uint tid;
            flat float tillingFactor;
            flat uint id;
        } vs_out;

        void main() {
            gl_Position = u_ViewProjMat * i_Pos;
            vs_out.color = i_Color;
            vs_out.uv = i_UV;
            vs_out.tid = i_TID;
            vs_out.tillingFactor = i_TillingFactor;
            vs_out.id = i_ID.x;
        }
    )";

    const std::string fragmentQuad = R"(
        #version 460 core

        layout(location = 0) out vec4 o_Color;
        layout(location = 1) out uint o_ID;

        in Data {
            vec4 color;
            vec2 uv;
            flat uint tid;
            flat float tillingFactor;
            flat uint id;
        } fs_in;

        layout(binding = 0) uniform sampler2D u_Tex0;
        layout(binding = 1) uniform sampler2D u_Tex1;
        layout(binding = 2) uniform sampler2D u_Tex2;
        layout(binding = 3) uniform sampler2D u_Tex3;
        layout(binding = 4) uniform sampler2D u_Tex4;
        layout(binding = 5) uniform sampler2D u_Tex5;
        layout(binding = 6) uniform sampler2D u_Tex6;
        layout(binding = 7) uniform sampler2D u_Tex7;
        layout(binding = 8) uniform sampler2D u_Tex8;
        layout(binding = 9) uniform sampler2D u_Tex9;
        layout(binding = 10) uniform sampler2D u_Tex10;
        layout(binding = 11) uniform sampler2D u_Tex11;
        layout(binding = 12) uniform sampler2D u_Tex12;
        layout(binding = 13) uniform sampler2D u_Tex13;
        layout(binding = 14) uniform sampler2D u_Tex14;
        layout(binding = 15) uniform sampler2D u_Tex15;

        vec4 GetDataFromTexture(uint tid, vec2 uv, float tillingFactor) {
            vec4 color = texture(u_Tex0, uv * tillingFactor) * (1 - abs(sign(tid - 0)));
            color += texture(u_Tex1, uv * tillingFactor) * (1 - abs(sign(tid - 1)));
            color += texture(u_Tex2, uv * tillingFactor) * (1 - abs(sign(tid - 2)));
            color += texture(u_Tex3, uv * tillingFactor) * (1 - abs(sign(tid - 3)));
            color += texture(u_Tex4, uv * tillingFactor) * (1 - abs(sign(tid - 4)));
            color += texture(u_Tex5, uv * tillingFactor) * (1 - abs(sign(tid - 5)));
            color += texture(u_Tex6, uv * tillingFactor) * (1 - abs(sign(tid - 6)));
            color += texture(u_Tex7, uv * tillingFactor) * (1 - abs(sign(tid - 7)));
            color += texture(u_Tex8, uv * tillingFactor) * (1 - abs(sign(tid - 8)));
            color += texture(u_Tex9, uv * tillingFactor) * (1 - abs(sign(tid - 9)));
            color += texture(u_Tex10, uv * tillingFactor) * (1 - abs(sign(tid - 10)));
            color += texture(u_Tex11, uv * tillingFactor) * (1 - abs(sign(tid - 11)));
            color += texture(u_Tex12, uv * tillingFactor) * (1 - abs(sign(tid - 12)));
            color += texture(u_Tex13, uv * tillingFactor) * (1 - abs(sign(tid - 13)));
            color += texture(u_Tex14, uv * tillingFactor) * (1 - abs(sign(tid - 14)));
            color += texture(u_Tex15, uv * tillingFactor) * (1 - abs(sign(tid - 15)));
            return color;
        }

        void main() {
            o_Color = fs_in.color * GetDataFromTexture(fs_in.tid, fs_in.uv, fs_in.tillingFactor);
            o_ID = fs_in.id;
        }
    )";

    Renderer::GetShaderLibrary().Load("quadShader", vertexQuad, fragmentQuad);

    const std::string vertexCircle = R"(
        #version 460 core

        layout(location = 0) in vec4 i_Pos;
        layout(location = 1) in vec4 i_Color;
        layout(location = 2) in float i_Thickness;
        layout(location = 3) in float i_Fade;
        layout(location = 4) in uvec2 i_ID;

        layout(std140, binding = 0) uniform RendererData {
            mat4 u_ViewProjMat;
        };

        out Data {
            vec4 localPos;
            vec4 color;
            flat float thickness;
            flat float fade;
            flat uint id;
        } vs_out;

        const vec4 localPos[4] = {
            { -1.0f,  1.0f, 0.0f, 0.0f },
            {  1.0f,  1.0f, 0.0f, 0.0f },
            {  1.0f, -1.0f, 0.0f, 0.0f },
            { -1.0f, -1.0f, 0.0f, 0.0f }
        };

        void main() {
            gl_Position = u_ViewProjMat * i_Pos;
            vs_out.localPos = localPos[gl_VertexID % 4];
            vs_out.color = i_Color;
            vs_out.thickness = i_Thickness;
            vs_out.fade = i_Fade;
            vs_out.id = i_ID.x;
        }
    )";

    const std::string fragmentCircle = R"(
        #version 460 core

        layout(location = 0) out vec4 o_Color;
        layout(location = 1) out uint o_ID;

        in Data {
            vec4 localPos;
            vec4 color;
            flat float thickness;
            flat float fade;
            flat uint id;
        } fs_in;

        void main() {
            const float distance = 1.0f - length(fs_in.localPos);
            const float alpha = smoothstep(0.0f, fs_in.fade, distance) * smoothstep(fs_in.thickness + fs_in.fade, fs_in.thickness, distance);
            if (alpha == 0.0f)
                discard;
            o_Color = fs_in.color * vec4(1.0f, 1.0f, 1.0f, alpha);
            o_ID = fs_in.id;
        }
    )";
    Renderer::GetShaderLibrary().Load("circleShader", vertexCircle, fragmentCircle);

    const std::string vertexMeshClear = R"(
        #version 460 core

        layout(location = 0) in vec4 i_Pos;
        layout(location = 1) in vec3 i_Normal;
        layout(location = 2) in vec3 i_Tangent;
        layout(location = 3) in vec2 i_UV;
        layout(location = 4) in vec4 i_Color;
        layout(location = 5) in mat4 i_ModelMat;
        layout(location = 9) in uvec4 i_TIDs;
        layout(location = 10) in uint i_ID;

        layout(std140, binding = 0) uniform RendererData {
            mat4 u_ViewProjMat;
        };

        out Data {
            vec4 color;
            vec2 uv;
            flat uint id;
        } vs_out;

        void main() {
            gl_Position = u_ViewProjMat * i_ModelMat * i_Pos;
            vs_out.color = i_Color;
            vs_out.uv = i_UV;
            vs_out.id = i_ID;
        }
    )";

    const std::string fragmentMeshClear = R"(
        #version 460 core

        layout(location = 0) out vec4 o_Color;
        layout(location = 1) out uint o_ID;

        in Data {
            vec4 color;
            vec2 uv;
            flat uint id;
        } fs_in;

        void main() {
            o_Color = fs_in.color;
            o_ID = fs_in.id;
        }
    )";

    Renderer::GetShaderLibrary().Load("meshClearShader", vertexMeshClear, fragmentMeshClear);

    const std::string vertexMesh = R"(
        #version 460 core

        layout(location = 0) in vec4 i_Pos;
        layout(location = 1) in vec3 i_Normal;
        layout(location = 2) in vec3 i_Tangent;
        layout(location = 3) in vec2 i_UV;
        layout(location = 4) in vec4 i_Color;
        layout(location = 5) in mat4 i_ModelMat;
        layout(location = 9) in uvec4 i_TIDs;
        layout(location = 10) in uint i_ID;

        layout(std140, binding = 0) uniform RendererData {
            mat4 u_ViewProjMat;
        };

        out Data {
            vec4 pos;
            vec4 color;
            vec3 normal;
            vec3 tangent;
            vec2 uv;
            flat uint colorTID;
            flat uint normalTID;
            flat uint id;
        } vs_out;

        void main() {
            vs_out.pos = i_ModelMat * i_Pos;
            gl_Position = u_ViewProjMat * vs_out.pos;
            vs_out.color = i_Color;

            mat3 normalMatrix = transpose(inverse(mat3(i_ModelMat)));
            vs_out.normal = normalize(normalMatrix * i_Normal);
            vs_out.tangent = normalize(normalMatrix * i_Tangent);
            vs_out.tangent = normalize(vs_out.tangent - dot(vs_out.tangent, vs_out.normal) * vs_out.normal);

            vs_out.uv = i_UV;
            vs_out.colorTID = i_TIDs.x;
            vs_out.normalTID = i_TIDs.y;
            vs_out.id = i_ID;
        }
    )";

    const std::string fragmentMesh = R"(
        #version 460 core

        layout(location = 0) out vec4 o_Color;
        layout(location = 1) out uint o_ID;

        in Data {
            vec4 pos;
            vec4 color;
            vec3 normal;
            vec3 tangent;
            vec2 uv;
            flat uint colorTID;
            flat uint normalTID;
            flat uint id;
        } fs_in;

        const int c_MaxLights = 100;
        struct LightData {
            vec4 lightPos;
            vec4 lightColor;
            vec4 lightConstants;
        };
        layout(std140, binding = 1) uniform LightsData {
            LightData u_Lights[c_MaxLights];
            vec4 u_ViewPos_LightsCount;
        };

        layout(binding = 0) uniform sampler2D u_Tex0;
        layout(binding = 1) uniform sampler2D u_Tex1;
        layout(binding = 2) uniform sampler2D u_Tex2;
        layout(binding = 3) uniform sampler2D u_Tex3;
        layout(binding = 4) uniform sampler2D u_Tex4;
        layout(binding = 5) uniform sampler2D u_Tex5;
        layout(binding = 6) uniform sampler2D u_Tex6;
        layout(binding = 7) uniform sampler2D u_Tex7;
        layout(binding = 8) uniform sampler2D u_Tex8;
        layout(binding = 9) uniform sampler2D u_Tex9;
        layout(binding = 10) uniform sampler2D u_Tex10;
        layout(binding = 11) uniform sampler2D u_Tex11;
        layout(binding = 12) uniform sampler2D u_Tex12;
        layout(binding = 13) uniform sampler2D u_Tex13;
        layout(binding = 14) uniform sampler2D u_Tex14;
        layout(binding = 15) uniform sampler2D u_Tex15;

        vec4 GetDataFromTexture(uint tid, vec2 uv) {
            vec4 color = texture(u_Tex0, uv) * (1 - abs(sign(tid - 0)));
            color += texture(u_Tex1, uv) * (1 - abs(sign(tid - 1)));
            color += texture(u_Tex2, uv) * (1 - abs(sign(tid - 2)));
            color += texture(u_Tex3, uv) * (1 - abs(sign(tid - 3)));
            color += texture(u_Tex4, uv) * (1 - abs(sign(tid - 4)));
            color += texture(u_Tex5, uv) * (1 - abs(sign(tid - 5)));
            color += texture(u_Tex6, uv) * (1 - abs(sign(tid - 6)));
            color += texture(u_Tex7, uv) * (1 - abs(sign(tid - 7)));
            color += texture(u_Tex8, uv) * (1 - abs(sign(tid - 8)));
            color += texture(u_Tex9, uv) * (1 - abs(sign(tid - 9)));
            color += texture(u_Tex10, uv) * (1 - abs(sign(tid - 10)));
            color += texture(u_Tex11, uv) * (1 - abs(sign(tid - 11)));
            color += texture(u_Tex12, uv) * (1 - abs(sign(tid - 12)));
            color += texture(u_Tex13, uv) * (1 - abs(sign(tid - 13)));
            color += texture(u_Tex14, uv) * (1 - abs(sign(tid - 14)));
            color += texture(u_Tex15, uv) * (1 - abs(sign(tid - 15)));
            return color;
        }

        vec3 GetLight(const vec3 viewDir, const vec3 color, const vec3 normal, const int lightIndex) {
            const vec3 lightDir = normalize(u_Lights[lightIndex].lightPos.xyz - fs_in.pos.xyz);
            const float distance = length(u_Lights[lightIndex].lightPos.xyz - fs_in.pos.xyz);
            const float attenuation = 1.0f / (u_Lights[lightIndex].lightConstants.x + u_Lights[lightIndex].lightConstants.y * distance
                + u_Lights[lightIndex].lightConstants.z * distance * distance);

            const float diff = max(dot(normal, lightDir), 0.0f);
            const vec3 diffuse = u_Lights[lightIndex].lightColor.rgb * diff * color.rgb * attenuation;

            const float shininess = 8.0f;
            const vec3 halfwayDir = normalize(lightDir + viewDir);
            const float spec = pow(max(dot(normal, halfwayDir), 0.0f), shininess);
            const vec3 specular = u_Lights[lightIndex].lightColor.rgb * spec * color.rgb * attenuation;

            return (diffuse + specular) * max(sign(dot(fs_in.normal, lightDir)), 0.0f);
        }
        void main() {
            vec4 color = GetDataFromTexture(fs_in.colorTID, fs_in.uv) * fs_in.color;
            color.rgb = pow(color.rgb, vec3(2.2f));
            const vec3 bitangent = normalize(cross(fs_in.normal, fs_in.tangent));
            vec3 normal = (GetDataFromTexture(fs_in.normalTID, fs_in.uv).xyz * 2.0f - 1.0f) * abs(sign(fs_in.normalTID));
            normal += vec3(0.0f, 0.0f, 1.0f) * (1 - abs(sign(fs_in.normalTID)));
            normal = normalize(mat3(fs_in.tangent, bitangent, fs_in.normal) * normal);

            const float ambientStrength = 0.005f;
            const vec3 ambient = ambientStrength * color.rgb;

            const vec3 viewDir = normalize(u_ViewPos_LightsCount.xyz - fs_in.pos.xyz);
            vec3 finalColor = ambient;
            for (int i = 0; i < int(u_ViewPos_LightsCount.w + 0.5f); ++i) {
                finalColor += GetLight(viewDir, color.rgb, normal, i);
            }

            finalColor = finalColor / (finalColor + vec3(1.0f));
            finalColor = pow(finalColor, vec3(1.0f / 2.2f));
            o_Color = vec4(finalColor, color.a);
            o_ID = fs_in.id;
        }
    )";

    Renderer::GetShaderLibrary().Load("meshShader", vertexMesh, fragmentMesh);

    const std::string vertexMeshPBR = R"(
        #version 460 core

        layout(location = 0) in vec4 i_Pos;
        layout(location = 1) in vec3 i_Normal;
        layout(location = 2) in vec3 i_Tangent;
        layout(location = 3) in vec2 i_UV;
        layout(location = 4) in vec4 i_Color;
        layout(location = 5) in mat4 i_ModelMat;
        layout(location = 9) in uvec4 i_TIDs;
        layout(location = 10) in uint i_ID;

        layout(std140, binding = 0) uniform RendererData {
            mat4 u_ViewProjMat;
        };

        out Data {
            vec4 pos;
            vec4 color;
            vec3 normal;
            vec3 tangent;
            vec2 uv;
            flat uint colorTID;
            flat uint normalTID;
            flat uint metallicTID;
            flat uint roughnessTID;
            flat uint id;
        } vs_out;

        void main() {
            vs_out.pos = i_ModelMat * i_Pos;
            gl_Position = u_ViewProjMat * vs_out.pos;
            vs_out.color = i_Color;

            mat3 normalMatrix = transpose(inverse(mat3(i_ModelMat)));
            vs_out.normal = normalize(normalMatrix * i_Normal);
            vs_out.tangent = normalize(normalMatrix * i_Tangent);
            vs_out.tangent = normalize(vs_out.tangent - dot(vs_out.tangent, vs_out.normal) * vs_out.normal);

            vs_out.uv = i_UV;
            vs_out.colorTID = i_TIDs.x;
            vs_out.normalTID = i_TIDs.y;
            vs_out.metallicTID = i_TIDs.z;
            vs_out.roughnessTID = i_TIDs.w;
            vs_out.id = i_ID;
        }
    )";

    const std::string fragmentMeshPBR = R"(
        #version 460 core
        const float pi = 3.1415926535897932384626433832795f;

        layout(location = 0) out vec4 o_Color;
        layout(location = 1) out uint o_ID;

        in Data {
            vec4 pos;
            vec4 color;
            vec3 normal;
            vec3 tangent;
            vec2 uv;
            flat uint colorTID;
            flat uint normalTID;
            flat uint metallicTID;
            flat uint roughnessTID;
            flat uint id;
        } fs_in;

        const int c_MaxLights = 100;
        struct LightData {
            vec4 lightPos;
            vec4 lightColor;
            vec4 lightConstants;
        };
        layout(std140, binding = 1) uniform LightsData {
            LightData u_Lights[c_MaxLights];
            vec4 u_ViewPos_LightsCount;
        };

        layout(binding = 0) uniform sampler2D u_Tex0;
        layout(binding = 1) uniform sampler2D u_Tex1;
        layout(binding = 2) uniform sampler2D u_Tex2;
        layout(binding = 3) uniform sampler2D u_Tex3;
        layout(binding = 4) uniform sampler2D u_Tex4;
        layout(binding = 5) uniform sampler2D u_Tex5;
        layout(binding = 6) uniform sampler2D u_Tex6;
        layout(binding = 7) uniform sampler2D u_Tex7;
        layout(binding = 8) uniform sampler2D u_Tex8;
        layout(binding = 9) uniform sampler2D u_Tex9;
        layout(binding = 10) uniform sampler2D u_Tex10;
        layout(binding = 11) uniform sampler2D u_Tex11;
        layout(binding = 12) uniform sampler2D u_Tex12;
        layout(binding = 13) uniform sampler2D u_Tex13;
        layout(binding = 14) uniform sampler2D u_Tex14;
        layout(binding = 15) uniform sampler2D u_Tex15;

        vec4 GetDataFromTexture(uint tid, vec2 uv);
        float DistributionGGX(vec3 normal, vec3 halfwayDir, float roughness);
        float GeometrySmith(vec3 normal, vec3 viewDir, vec3 lightDir, float roughness);
        vec3 FresnelSchlick(float cosTheta, vec3 F0);

        void main() {
            vec4 albedo = GetDataFromTexture(fs_in.colorTID, fs_in.uv) * fs_in.color;
            albedo.rgb = pow(albedo.rgb, vec3(2.2f));

            const vec3 bitangent = normalize(cross(fs_in.normal, fs_in.tangent));
            vec3 normal = (GetDataFromTexture(fs_in.normalTID, fs_in.uv).xyz * 2.0f - 1.0f) * abs(sign(fs_in.normalTID))
                + vec3(0.0f, 0.0f, 1.0f) * (1 - abs(sign(fs_in.normalTID)));
            normal = normalize(mat3(fs_in.tangent, bitangent, fs_in.normal) * normal);

            const float metallic = GetDataFromTexture(fs_in.metallicTID, fs_in.uv).r * abs(sign(fs_in.metallicTID))
                + 0.5f * (1 - abs(sign(fs_in.metallicTID)));
            const float roughness = GetDataFromTexture(fs_in.roughnessTID, fs_in.uv).r * abs(sign(fs_in.roughnessTID))
                + 0.5f * (1 - abs(sign(fs_in.roughnessTID)));

            const float ambientStrength = 0.005f;
            const vec3 ambient = ambientStrength * albedo.rgb;

            const vec3 viewDir = normalize(u_ViewPos_LightsCount.xyz - fs_in.pos.xyz);

            const vec3 F0 = mix(vec3(0.04), albedo.rgb, metallic);
            vec3 Lo = ambient;
            for (int i = 0; i < int(u_ViewPos_LightsCount.w + 0.5f); ++i) {
                const vec3 lightDir = normalize(u_Lights[i].lightPos.xyz - fs_in.pos.xyz);
                const vec3 halfwayDir = normalize(viewDir + lightDir);
                const float distance = length(u_Lights[i].lightPos.xyz - fs_in.pos.xyz);
                const float attenuation = 1.0f / (u_Lights[i].lightConstants.x + u_Lights[i].lightConstants.y * distance
                    + u_Lights[i].lightConstants.z * distance * distance);
                const vec3 radiance = u_Lights[i].lightColor.rgb * attenuation;

                const float NDF = DistributionGGX(normal, halfwayDir, roughness);
                const float G = GeometrySmith(normal, viewDir, halfwayDir, roughness);
                const vec3 F = FresnelSchlick(max(dot(halfwayDir, viewDir), 0.0f), F0);

                const vec3 numerator = NDF * G * F;
                const float denominator = max(4.0f * max(dot(normal, viewDir), 0.0f) * max(dot(normal, lightDir), 0.0f), 0.0001f);
                const vec3 specular = numerator / denominator;

                const vec3 kD = (vec3(1.0f) - F) * (1.0f - metallic);
                Lo += (kD * albedo.rgb / pi + specular) * radiance * max(dot(normal, lightDir), 0.0f) * max(sign(dot(fs_in.normal, lightDir)), 0.0f);
            }

            Lo = Lo / (Lo + vec3(1.0f));
            Lo = pow(Lo, vec3(1.0f / 2.2f)); //temp HDR and gamma correction

            o_Color = vec4(Lo, albedo.a);
            o_ID = fs_in.id;
        }
        vec4 GetDataFromTexture(uint tid, vec2 uv) {
            vec4 color = texture(u_Tex0, uv) * (1 - abs(sign(tid - 0)));
            color += texture(u_Tex1, uv) * (1 - abs(sign(tid - 1)));
            color += texture(u_Tex2, uv) * (1 - abs(sign(tid - 2)));
            color += texture(u_Tex3, uv) * (1 - abs(sign(tid - 3)));
            color += texture(u_Tex4, uv) * (1 - abs(sign(tid - 4)));
            color += texture(u_Tex5, uv) * (1 - abs(sign(tid - 5)));
            color += texture(u_Tex6, uv) * (1 - abs(sign(tid - 6)));
            color += texture(u_Tex7, uv) * (1 - abs(sign(tid - 7)));
            color += texture(u_Tex8, uv) * (1 - abs(sign(tid - 8)));
            color += texture(u_Tex9, uv) * (1 - abs(sign(tid - 9)));
            color += texture(u_Tex10, uv) * (1 - abs(sign(tid - 10)));
            color += texture(u_Tex11, uv) * (1 - abs(sign(tid - 11)));
            color += texture(u_Tex12, uv) * (1 - abs(sign(tid - 12)));
            color += texture(u_Tex13, uv) * (1 - abs(sign(tid - 13)));
            color += texture(u_Tex14, uv) * (1 - abs(sign(tid - 14)));
            color += texture(u_Tex15, uv) * (1 - abs(sign(tid - 15)));
            return color;
        }
        float DistributionGGX(vec3 normal, vec3 halfwayDir, float roughness) {
            const float a = roughness * roughness;
            const float cosNH = max(dot(normal, halfwayDir), 0.0f);
            const float nominator = a * a;
            float denominator = (cosNH * cosNH * (nominator - 1.0f) + 1.0f);
            denominator = pi * denominator * denominator;
            return nominator / denominator;
        }
        float GeometrySchlickGGX(float cosNV, float roughness) {
            const float r = roughness + 1.0f;
            const float k = (r * r) / 8.0f;
            return cosNV / (cosNV * (1.0f - k) + k);
        }
        float GeometrySmith(vec3 normal, vec3 viewDir, vec3 lightDir, float roughness) {
            const float cosNV = max(dot(normal, viewDir), 0.0f);
            const float cosNL = max(dot(normal, lightDir), 0.0f);
            return GeometrySchlickGGX(cosNV, roughness) * GeometrySchlickGGX(cosNL, roughness);
        }
        vec3 FresnelSchlick(float cosTheta, vec3 F0) {
            return F0 + (1.0f + F0) * pow(clamp(1.0f - cosTheta, 0.0f, 1.0f), 5.0f);
        }
    )";

    Renderer::GetShaderLibrary().Load("meshPBRShader", vertexMeshPBR, fragmentMeshPBR);
}

void OpenGLRendererAPI::SetDepthTestOptions(bool enable, bool writeMask, ComparisonFunc compFunc) {
    if (enable)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);

    glDepthMask(static_cast<uint8_t>(writeMask));
    glDepthFunc(GetComparisonFunc(compFunc));
}
void OpenGLRendererAPI::SetStencilTestOptions(bool enable, uint8_t writeMask, uint8_t readMask, ComparisonFunc frontFaceFunc,
    ComparisonFunc backFaceFunc, uint32_t stencilRefVal)
{
    if (enable)
        glEnable(GL_STENCIL_TEST);
    else
        glDisable(GL_STENCIL_TEST);

    glStencilMask(writeMask);
    glStencilFuncSeparate(GL_FRONT, GetComparisonFunc(frontFaceFunc), static_cast<int>(stencilRefVal), readMask);
    glStencilFuncSeparate(GL_BACK, GetComparisonFunc(backFaceFunc), static_cast<int>(stencilRefVal), readMask);
}
void OpenGLRendererAPI::SetFrontFaceStencilOperations(StencilOperation stencilFail, StencilOperation depthFail, StencilOperation pass) {
    glStencilOpSeparate(GL_FRONT, GetStencilOp(stencilFail), GetStencilOp(depthFail), GetStencilOp(pass));
}
void OpenGLRendererAPI::SetBackFaceStencilOperations(StencilOperation stencilFail, StencilOperation depthFail, StencilOperation pass) {
    glStencilOpSeparate(GL_BACK, GetStencilOp(stencilFail), GetStencilOp(depthFail), GetStencilOp(pass));
}
void OpenGLRendererAPI::SetRasterizerOptions(TriangleFillMode fillMode, TriangleCullMode cullMode, bool isFrontFaceCounterClockwise) {
    GLenum fill = 0;
    switch (fillMode) {
        case Saba::RendererAPI::TriangleFillMode::Full:
            fill = GL_FILL;
            break;
        case Saba::RendererAPI::TriangleFillMode::Wireframe:
            fill = GL_LINE;
            break;
        default:
            SB_CORE_THROW_INFO("Invalid triangle fill mode");
            break;
    }
    glPolygonMode(GL_FRONT_AND_BACK, fill);

    switch (cullMode) {
        case Saba::RendererAPI::TriangleCullMode::DrawAll:
            glDisable(GL_CULL_FACE);
            break;
        case Saba::RendererAPI::TriangleCullMode::DrawFrontFace:
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            break;
        case Saba::RendererAPI::TriangleCullMode::DrawBackFace:
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
            break;
        default:
            SB_CORE_THROW_INFO("Invalid triangle cull mode");
            break;
    }

    if (isFrontFaceCounterClockwise)
        glFrontFace(GL_CCW);
    else
        glFrontFace(GL_CW);
}
void OpenGLRendererAPI::SetBlendOptions(uint32_t i, bool enable, BlendOption sourceBlend, BlendOption destinationBlend, BlendOperation operation,
    BlendOption sourceAlphaBlend, BlendOption destinationAlphaBlend, BlendOperation alphaOperation, uint8_t writeMask, glm::vec4 blendFactor)
{
    if (enable)
        glEnablei(GL_BLEND, i);
    else
        glDisablei(GL_BLEND, i);

    glBlendFuncSeparatei(i, GetBlendOption(sourceBlend), GetBlendOption(destinationBlend), GetBlendOption(sourceAlphaBlend), GetBlendOption(destinationAlphaBlend));
    glBlendEquationSeparatei(i, GetBlendOp(operation), GetBlendOp(alphaOperation));
    glBlendColor(blendFactor.r, blendFactor.g, blendFactor.b, blendFactor.a);
}

}
