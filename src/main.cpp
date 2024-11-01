#include "LLGL.h"
#include "LLGL/Utils/VertexFormat.h"
#include "Gauss/Gauss.h" // This is the path to the Gauss library

struct MyVertex {
    float   position[2]; // 2D vector for X and Y coordinates
    uint8_t color[4];    // 4D vector for red, green, blue, and alpha components
};

int main()
{

	LLGL::RenderSystemPtr myRenderer;
	for (const char* module : { "Direct3D12", "Direct3D11", "Vulkan", "OpenGL" }) {
		LLGL::Report report;
		myRenderer = LLGL::RenderSystem::Load(module, &report);
		if (myRenderer == nullptr) {
			LLGL::Log::Errorf("%s", report.GetText());
		}
	}
	if (myRenderer == nullptr) {
		/* Error: no suitable renderer found ... */
	}

	LLGL::SwapChainDescriptor mySwapChainDesc;
	mySwapChainDesc.resolution  = { 800, 600 }; // Framebuffer resolution of 800x600 pixels
	mySwapChainDesc.fullscreen  = false;        // No fullscreen, use windowed mode
	mySwapChainDesc.samples     = 8;            // 8 samples for anti-aliasing
	LLGL::SwapChain* mySwapChain = myRenderer->CreateSwapChain(mySwapChainDesc);

	MyVertex myVertices[3] = {
    MyVertex{ {  0.0f,  0.5f }, { 255,   0,   0, 255 } }, // Red
    MyVertex{ {  0.5f, -0.5f }, {   0, 255,   0, 255 } }, // Green
    MyVertex{ { -0.5f, -0.5f }, {   0,   0, 255, 255 } }, // Blue
	};
	
	LLGL::VertexFormat myVertexFormat;
	myVertexFormat.AppendAttribute({ "position", LLGL::Format::RG32Float  });
	myVertexFormat.AppendAttribute({ "color",    LLGL::Format::RGBA8UNorm });

	LLGL::BufferDescriptor myVBufferDesc;
	myVBufferDesc.size          = sizeof(myVertices);            // Size (in bytes) of the buffer
	myVBufferDesc.bindFlags     = LLGL::BindFlags::VertexBuffer; // Use for vertex buffer binding
	myVBufferDesc.vertexAttribs = myVertexFormat.attributes;     // Vertex buffer attributes
	LLGL::Buffer* myVertexBuffer = myRenderer->CreateBuffer(myVBufferDesc, myVertices);

	LLGL::ShaderDescriptor myVSDesc, myFSDesc;
	myVSDesc = { LLGL::ShaderType::Vertex,   "MyShader.vert" };
    myFSDesc = { LLGL::ShaderType::Fragment, "MyShader.frag" };

	myVSDesc.vertex.inputAttribs = myVertexFormat.attributes;

	LLGL::Shader* myVertexShader   = myRenderer->CreateShader(myVSDesc);
	LLGL::Shader* myFragmentShader = myRenderer->CreateShader(myFSDesc);

	for (LLGL::Shader* shader : { myVertexShader, myFragmentShader }) {
		if (const LLGL::Report* report = shader->GetReport()) {
			if (report->HasErrors()) {
				LLGL::Log::Errorf("Shader compile errors:\n%s", report->GetText());
			} else {
				LLGL::Log::Printf("Shader compile warnings:\n%s", report->GetText());
			}
		}
	}

	LLGL::GraphicsPipelineDescriptor myPipelineDesc;
	myPipelineDesc.vertexShader                  = myVertexShader;
	myPipelineDesc.fragmentShader                = myFragmentShader;
	myPipelineDesc.rasterizer.multiSampleEnabled = (mySwapChainDesc.samples > 1);
	LLGL::PipelineState* myPipeline = myRenderer->CreatePipelineState(myPipelineDesc);

	LLGL::CommandBuffer* myCmdBuffer = myRenderer->CreateCommandBuffer(LLGL::CommandBufferFlags::ImmediateSubmit);

	// Cast "Window" from base class "Surface" (only on desktop platforms such as Windows, GNU/Linux, and macOS)
	LLGL::Window& myWindow = LLGL::CastTo<LLGL::Window>(mySwapChain->GetSurface());

	// Set window title (aka. caption) and show window
	myWindow.SetTitle("Hello Triangle");
	myWindow.Show();

	// Process window events (such as user input)
	while (myWindow.ProcessEvents()) {
		myCmdBuffer->Begin();
		myCmdBuffer->SetVertexBuffer(*myVertexBuffer);
		myCmdBuffer->BeginRenderPass(*mySwapChain);
		myCmdBuffer->SetViewport(mySwapChain->GetResolution());
		myCmdBuffer->Clear(LLGL::ClearFlags::Color);
		myCmdBuffer->SetPipelineState(*myPipeline);
		myCmdBuffer->Draw(3, 0);
		myCmdBuffer->EndRenderPass();
		myCmdBuffer->End();
		mySwapChain->Present();
	}
}