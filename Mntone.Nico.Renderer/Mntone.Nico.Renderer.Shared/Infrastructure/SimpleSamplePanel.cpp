#include "pch.h"
#include "SimpleSamplePanel.h"
#include "Infrastructure\DirectXHelper.h"

using namespace D2D1;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Globalization::DateTimeFormatting;
using namespace Mntone::DirectX;

SimpleSamplePanel::SimpleSamplePanel()
	: DirectXDynamicPanelBase( DXGI_ALPHA_MODE_PREMULTIPLIED )
{
	Initialize();
}

void SimpleSamplePanel::OnDeviceIndependentResourcesInitialized()
{
	DirectXDynamicPanelBase::OnDeviceIndependentResourcesInitialized();
	DirectXHelper::CheckHResult( dwriteFactory_->CreateTextFormat( L"Segoe UI", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 18.f, L"ja-JP", textFormat_.GetAddressOf() ) );
}

void SimpleSamplePanel::OnDeviceResourcesInitialized()
{
	DirectXDynamicPanelBase::OnDeviceResourcesInitialized();
	DirectXHelper::CheckHResult( d2dDeviceContext_->CreateSolidColorBrush( ColorF( ColorF::White ), blackBrush_.GetAddressOf() ) );
}

void SimpleSamplePanel::OnSizeDependentResourcesInitialized()
{
	DirectXDynamicPanelBase::OnSizeDependentResourcesInitialized();
	d2dDeviceContext_->SetDpi( Width_ / 672.f * Dpi_, Height_ / 384.f * Dpi_ );
}

void SimpleSamplePanel::OnRendering( DirectXRenderingEventArgs^ e )
{
	d2dDeviceContext_->BeginDraw();
	d2dDeviceContext_->Clear( ColorF( 0, 0.f ) );

	DirectXDynamicPanelBase::OnRendering( e );

	// Draw grid lines.
	d2dDeviceContext_->DrawLine( Point2F( 0, 0 ), Point2F( 672, 384 ), blackBrush_.Get(), 2.f );

	// Draw center circle.
	d2dDeviceContext_->DrawEllipse( Ellipse( Point2F( 672 / 2, 384 / 2 ), 672 / 2, 384 / 2 ), blackBrush_.Get(), 2.f );

	FILETIME ft = { 0 };
	GetSystemTimeAsFileTime( &ft );
	auto dateTime = DateTime{ *reinterpret_cast<int64*>( &ft ) };
	auto longDateFormatter = DateTimeFormatter::LongDate;
	auto longTimeFormatter = DateTimeFormatter::LongTime;
	auto s = longDateFormatter->Format( dateTime ) + " " + longTimeFormatter->Format( dateTime ) + ", " + e->FramesPerSecond + " fps, " + e->TotalFrames + " frames";
	d2dDeviceContext_->DrawText( s->Data(), s->Length(), textFormat_.Get(), D2D1::RectF( 0.f, 0.f, 672, 384 ), blackBrush_.Get() );

	d2dDeviceContext_->EndDraw();
}