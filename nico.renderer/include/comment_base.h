#pragma once
#include <cstdint>
#include <deque>
#include <chrono>

namespace nico { namespace renderer {

	const size_t maximum_comment_visible_count = 50;

	using comment_cyalume_size = uint8_t;
	const comment_cyalume_size large_cyalume_size = 4;
	const comment_cyalume_size medium_cyalume_size = 2;
	const comment_cyalume_size small_cyalume_size = 3;

	enum class comment_vertical_position: uint8_t { top = 1, center, bottom };
	const comment_vertical_position default_comment_vertical_position = comment_vertical_position::center;

	//enum class comment_horizontal_position: uint8_t { left = 1, center, right };
	//const comment_horizontal_position default_comment_horizontal_position = comment_horizontal_position::center;

	enum class comment_size: uint8_t { large = 39u, medium = 24u, small = 15u };
	const comment_size default_comment_size = comment_size::medium;

	//enum class comment_sound_effect: uint8_t { none, se1, se2 };
	//const comment_sound_effect default_comment_sound_effect = comment_sound_effect::none;

	using comment_position = float;

	using comment_color = uint32_t;
	const comment_color default_comment_color = 0xffffff;

	using comment_alpha = float;
	const comment_alpha default_comment_alpha = 1.0f;
	const comment_alpha overlap_comment_alpha = 0.6f;

	using comment_time = std::chrono::nanoseconds;

	enum class comment_mode: uint8_t { default = 1, top, bottom, both, back };

	class comment_base
	{
	public:
		virtual const wchar_t* const value() const noexcept = 0;
		virtual bool self() const noexcept = 0;
		virtual bool cyalume() const noexcept = 0;
		virtual comment_vertical_position vertical_position() const noexcept = 0;
		virtual comment_size size() const noexcept = 0;
		virtual comment_color color() const noexcept = 0;
		virtual comment_time time() const noexcept = 0; // TS ÉÇÅ[ÉhÇ≈égÇ§
	};

	struct comment_text_info
	{
		comment_position height, width;
	};

	class rendering_comment;
	class renderer_driver
	{
	public:
		virtual comment_time now() const noexcept = 0;
		virtual comment_text_info text_info( comment_position font_size, const wchar_t* text ) const noexcept = 0;
		virtual void render( const std::deque<rendering_comment*>& comments ) noexcept = 0;
	};

} }