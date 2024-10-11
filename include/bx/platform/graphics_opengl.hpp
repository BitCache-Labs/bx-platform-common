#pragma once

#include <bx/platform/graphics.hpp>

class BX_PLATFORM_API GraphicsOpenGL final : public Graphics
{
    RTTR_ENABLE(Graphics)

public:
    GraphicsOpenGL() = default;

    static u32 GetTextureHandle(GraphicsHandle texture);

public:
	bool Initialize() override;
	void Reload() override;
	void Shutdown() override;

	void NewFrame() override;
	void EndFrame() override;

	TextureFormat GetColorBufferFormat() override;
	TextureFormat GetDepthBufferFormat() override;

	GraphicsHandle GetCurrentBackBufferRT() override;
	GraphicsHandle GetDepthBuffer() override;

	void SetRenderTarget(const GraphicsHandle renderTarget, const GraphicsHandle depthStencil) override;
	void ReadPixels(u32 x, u32 y, u32 w, u32 h, void* pixelData, const GraphicsHandle renderTarget) override;

	void SetViewport(const f32 viewport[4]) override;

	void ClearRenderTarget(const GraphicsHandle renderTarget, const f32 clearColor[4]) override;
	void ClearDepthStencil(const GraphicsHandle depthStencil, GraphicsClearFlags flags, f32 depth, i32 stencil) override;

	GraphicsHandle CreateShader(const ShaderInfo& info) override;
	void DestroyShader(const GraphicsHandle shader) override;

	GraphicsHandle CreateTexture(const TextureInfo& info) override;
	GraphicsHandle CreateTexture(const TextureInfo& info, const BufferData& data) override;
	void DestroyTexture(const GraphicsHandle texture) override;

	GraphicsHandle CreateResourceBinding(const ResourceBindingInfo& info) override;
	void DestroyResourceBinding(const GraphicsHandle resources) override;
	void BindResource(const GraphicsHandle resources, const char* name, GraphicsHandle resource) override;

	GraphicsHandle CreatePipeline(const PipelineInfo& info) override;
	void DestroyPipeline(const GraphicsHandle pipeline) override;
	void SetPipeline(const GraphicsHandle pipeline) override;
	void CommitResources(const GraphicsHandle pipeline, const GraphicsHandle resources) override;

	GraphicsHandle CreateBuffer(const BufferInfo& info) override;
	GraphicsHandle CreateBuffer(const BufferInfo& info, const BufferData& data) override;
	void DestroyBuffer(const GraphicsHandle buffer) override;
	void UpdateBuffer(const GraphicsHandle buffer, const BufferData& data) override;

	void SetVertexBuffers(i32 i, i32 count, const GraphicsHandle* pBuffers, const u64* offset) override;
	void SetIndexBuffer(const GraphicsHandle buffer, i32 i) override;

	void Draw(const DrawAttribs& attribs) override;
	void DrawIndexed(const DrawIndexedAttribs& attribs) override;

	bool InitializeImGui() override;
	void ShutdownImGui() override;
	void NewFrameImGui() override;
	void EndFrameImGui() override;
};