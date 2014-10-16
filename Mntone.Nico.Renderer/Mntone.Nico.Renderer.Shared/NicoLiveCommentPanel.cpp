#include "pch.h"
#include "NicoLiveCommentPanel.h"
#include "Infrastructure\DirectXHelper.h"
#include "native\comment_wrapper.h"

using namespace D2D1;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Globalization::DateTimeFormatting;
using namespace Mntone::DirectX;
using namespace nico::renderer;
using namespace Mntone::Nico::Renderer;

NicoLiveCommentPanel::NicoLiveCommentPanel()
	: DirectXDynamicPanelBase( DXGI_ALPHA_MODE_PREMULTIPLIED )
{
	renderer_ = ::std::make_shared<renderer>( *reinterpret_cast<renderer_driver*>( &driver_ ) );

	Initialize();
}

void NicoLiveCommentPanel::AddComment( IComment^ comment )
{
	renderer_->add( comment );
}

void NicoLiveCommentPanel::OnDeviceIndependentResourcesInitialized()
{
	DirectXDynamicPanelBase::OnDeviceIndependentResourcesInitialized();
	driver_.set_dwrite_factory( dwriteFactory_.Get() );

#if _DEBUG
	DirectXHelper::CheckHResult( dwriteFactory_->CreateTextFormat( L"Segoe UI", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 18.f, L"ja-JP", textFormat_.GetAddressOf() ) );
#endif
}

void NicoLiveCommentPanel::OnDeviceResourcesInitialized()
{
	DirectXDynamicPanelBase::OnDeviceResourcesInitialized();
	driver_.initialize( d2dDeviceContext_.Get() );

#if _DEBUG
	DirectXHelper::CheckHResult( d2dDeviceContext_->CreateSolidColorBrush( ColorF( ColorF::White ), whiteBrush_.GetAddressOf() ) );
	DirectXHelper::CheckHResult( d2dDeviceContext_->CreateSolidColorBrush( ColorF( ColorF::Black, 0.6f ), blackBrush_.GetAddressOf() ) );
#endif
}

void NicoLiveCommentPanel::OnSizeDependentResourcesInitialized()
{
	DirectXDynamicPanelBase::OnSizeDependentResourcesInitialized();
	d2dDeviceContext_->SetDpi( Width_ / 672.f * Dpi_, Height_ / 384.f * Dpi_ );
	driver_.size_change();
}

void NicoLiveCommentPanel::OnRendering( DirectXRenderingEventArgs^ e )
{
	renderer_->prepare();

	d2dDeviceContext_->BeginDraw();
	d2dDeviceContext_->Clear( ColorF( 0, 0.f ) );

	DirectXDynamicPanelBase::OnRendering( e );

	renderer_->render();

#if _DEBUG
	auto drawRect = RectF( 0, 384.f - 28, 400, 384.f );
	d2dDeviceContext_->FillRectangle( drawRect, blackBrush_.Get() );

	FILETIME ft = { 0 };
	GetSystemTimeAsFileTime( &ft );
	auto dateTime = DateTime{ *reinterpret_cast<int64*>( &ft ) };
	auto longDateFormatter = DateTimeFormatter::LongDate;
	auto longTimeFormatter = DateTimeFormatter::LongTime;
	auto s = longDateFormatter->Format( dateTime ) + " " + longTimeFormatter->Format( dateTime ) + ", " + e->FramesPerSecond + " fps, " + e->TotalFrames + " frames";
	d2dDeviceContext_->DrawText( s->Data(), s->Length(), textFormat_.Get(), drawRect, whiteBrush_.Get() );
#endif

	d2dDeviceContext_->EndDraw();
}

bool NicoLiveCommentPanel::IsHeaderView::get()
{
	return renderer_->is_header_view();
}

void NicoLiveCommentPanel::IsHeaderView::set( bool value )
{
	renderer_->set_is_header_view( value );
}

CommentModeType NicoLiveCommentPanel::CommentMode::get()
{
	return static_cast<CommentModeType>( renderer_->mode() );
}

void NicoLiveCommentPanel::CommentMode::set( CommentModeType value )
{
	renderer_->set_mode( static_cast<comment_mode_type>( value ) );
}