#pragma once
#include <windows/directx_renderer_driver.h>
#include "./mnfx/directx_window.h"
#include "comment_window.h"
#include "comment_sample.h"

using namespace nico::renderer;
using namespace nico::renderer::windows;

class renderer_window final
	: public mnfx::directx_window
{
public:
	renderer_window( const std::shared_ptr<virtual_comment_server> cmtsvr );

public:
	virtual HRESULT initialize( HINSTANCE hinstance ) noexcept;

protected:
	virtual HRESULT initialize_resources() noexcept;
	virtual HRESULT initialize_device_dependent_resources() noexcept;
	virtual HRESULT initialize_size_dependent_resources() noexcept;

	virtual HRESULT on_render() noexcept;

private:
	void on_comment( const std::shared_ptr<comment_sample> comment );
	void on_comment_mode_changed( const comment_mode mode );

private:
	std::shared_ptr<virtual_comment_server> cmtsvr_;

	directx_renderer_driver driver_;
	renderer renderer_;

	std::vector<std::shared_ptr<comment_sample>> buffer_;

	//ComPtr<ID2D1Bitmap1> back_buffer_bitmap_;
	//ComPtr<ID2D1Effect> shadowEffect;
	//ComPtr<ID2D1Effect> floodEffect;
	//ComPtr<ID2D1Effect> affineTransformEffect;
	//ComPtr<ID2D1Effect> compositeEffect;
};