#pragma once
#include "Infrastructure\DirectXDynamicPanelBase.h"
#include "IComment.h"

#include "renderer.h"
#include "windows\directx_renderer_driver.h"

namespace Mntone { namespace Nico { namespace Renderer {

	public ref class NicoLiveCommentPanel sealed
		: public DirectX::DirectXDynamicPanelBase
	{
	public:
		NicoLiveCommentPanel();

		void AddComment( IComment^ comment );
		void ResetComment();

	protected:
		virtual void OnDeviceIndependentResourcesInitialized() override;
		virtual void OnDeviceResourcesInitialized() override;
		virtual void OnSizeDependentResourcesInitialized() override;
		virtual void OnRendering( DirectX::DirectXRenderingEventArgs^ e ) override;

	public:
		property bool IsHeaderView
		{
			bool get();
			void set( bool value );
		}
		property CommentModeType CommentMode
		{
			CommentModeType get();
			void set( CommentModeType value );
		}
		property float32 CommentScale
		{
			float32 get();
			void set( float32 value );
		}

	private:
		::std::shared_ptr<::nico::renderer::renderer> renderer_;
		::nico::renderer::windows::directx_renderer_driver driver_;

#if _DEBUG
		::Microsoft::WRL::ComPtr<IDWriteTextFormat> textFormat_;
		::Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> whiteBrush_;
		::Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> blackBrush_;
#endif
	};

} } }