#pragma once
#include <cstdint>
namespace rg {

class IFrameFence;
class IPassRenderable;
class IFrameResource;
class IGraphicsPass;
class IPassConstantBuffer;
class IDrawable;
class IShaderProgram;
class IConstantBuffer;
class ITextureResource;
class IOBjectTranstfromBuffer;
class IGPUIndexBuffer;
class IGPUVertexBuffer;
class IInputLayout;
class IFrameQueue;
class ISamplerResource;

enum class Topology;
enum class DxgiFormat;

using uint64 = std::uint64_t;
using uint32 = std::uint32_t;
using uint16 = std::uint16_t;
using uint8  = std::uint8_t;

struct IndexBufferDesc {
	const void *pData = nullptr;
	uint64 size = 0;
};

struct VertexBufferDesc {
	const void *pData = nullptr;
	uint64 size = 0;
};

struct InputElementDesc {
	const char *pSemanticName;
	uint32	   semanticIndex;
	DxgiFormat format;
	uint32	   inputSlot;
	uint32	   alignedByteOffset;
	uint32	   instanceDataStepRate;
};

struct InputLayoutDesc {
	const InputElementDesc *pElement;
	uint32 numElement;
};

struct ScissorRect {
	float topLeftX;
	float topLeftY;
	float width;
	float height;
	float minDepth;
	float maxDepth;
};



}