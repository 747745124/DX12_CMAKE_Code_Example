#include "Common/d3dApp.h"
#include "Common/MathHelper.h"
#include "Common/UploadBuffer.h"
#include <iostream>

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

class MyApp : public D3DApp
{
public:
    MyApp(HINSTANCE hInstance) : D3DApp(hInstance){};
    ~MyApp(){};

    void LogAdapters()
    {
        UINT i = 0;
        IDXGIAdapter *adapter = nullptr;
        std::vector<IDXGIAdapter *> adapterList;

        while (mdxgiFactory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND)
        {
            DXGI_ADAPTER_DESC adapterDesc;
            adapter->GetDesc(&adapterDesc);

            std::wstring text = L"***Adapter: ";
            text += adapterDesc.Description;
            text += L"\n";

            OutputDebugStringW(text.c_str());

            adapterList.push_back(adapter);
            ++i;
        }

        for (size_t i = 0; i < adapterList.size(); ++i)
        {
            LogAdapterOutputs(adapterList[i]);
            ReleaseCom(adapterList[i]);
        }

        return;
    };

    virtual bool
    Initialize() override;

private:
    virtual void Update(const GameTimer &gt) override
    {
        LogAdapters();
    };
    virtual void Draw(const GameTimer &gt) override{

    };
};

bool MyApp::Initialize()
{
    if (!D3DApp::Initialize())
        return false;

    // Reset the command list to prep for initialization commands.
    ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));

    // Execute the initialization commands.
    ThrowIfFailed(mCommandList->Close());
    ID3D12CommandList *cmdsLists[] = {mCommandList.Get()};
    mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

    // Wait until initialization is complete.
    FlushCommandQueue();
    return true;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
                   PSTR cmdLine, int showCmd)
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    try
    {
        MyApp theApp(hInstance);
        if (!theApp.Initialize())
            return 0;

        return theApp.Run();
    }
    catch (DxException &e)
    {
        MessageBoxW(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
        return 0;
    }
}