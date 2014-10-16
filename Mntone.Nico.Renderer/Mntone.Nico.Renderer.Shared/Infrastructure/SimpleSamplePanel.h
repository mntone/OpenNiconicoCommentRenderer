#pragma once
#include "Infrastructure\DirectXDynamicPanelBase.h"

namespace Mntone { namespace DirectX {

	public ref class SimpleSamplePanel sealed
		: public DirectX::DirectXDynamicPanelBase
	{
	public:
		SimpleSamplePanel();

	protected:
		virtual void OnDeviceIndependentResourcesInitialized() override;
		virtual void OnDeviceResourcesInitialized() override;
		virtual void OnSizeDependentResourcesInitialized() override;
		virtual void OnRendering( DirectX::DirectXRenderingEventArgs^ e ) override;

	private:
		::Microsoft::WRL::ComPtr<IDWriteTextFormat> textFormat_;
		::Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> blackBrush_;
	};

} }