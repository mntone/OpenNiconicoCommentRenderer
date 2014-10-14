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

	enum class comment_vertical_position_type: uint8_t { top = 1, center, bottom };
	const comment_vertical_position_type default_comment_vertical_position = comment_vertical_position_type::center;

	//enum class comment_horizontal_position_type: uint8_t { left = 1, center, right };
	//const comment_horizontal_position_type default_comment_horizontal_position = comment_horizontal_position::center;

	enum class comment_size_type: uint8_t { large = 39u, medium = 24u, small = 15u };
	const comment_size_type default_comment_size = comment_size_type::medium;

	//enum class comment_sound_effect_type: uint8_t { none, se1, se2 };
	//const comment_sound_effect_type default_comment_sound_effect = comment_sound_effect::none;

	using comment_position = float;

	using comment_color = uint32_t;
	const comment_color default_comment_color = 0xffffff;

	using comment_alpha = float;
	const comment_alpha default_comment_alpha = 1.0f;
	const comment_alpha overlap_comment_alpha = 0.6f;

	using comment_time = std::chrono::nanoseconds;

	enum class comment_mode_type: uint8_t { default = 1, top, bottom, both, back };

	enum class comment_font_type: uint8_t
	{
		arial = 1,
		ms_p_gothic,
		sim_sun,
		gulim,
		p_ming_li_u,
		ms_sans_serif,
		segoe_ui_symbol,
	};

	class comment_base
	{
	public:
		virtual const wchar_t* const value() const noexcept = 0;
		virtual size_t length() const noexcept = 0;
		virtual bool self() const noexcept = 0;
		virtual bool cyalume() const noexcept = 0;
		virtual comment_vertical_position_type vertical_position() const noexcept = 0;
		virtual comment_size_type size() const noexcept = 0;
		virtual comment_color color() const noexcept = 0;
		virtual comment_time time() const noexcept = 0; // TS ƒ‚[ƒh‚ÅŽg‚¤
	};

	struct comment_text_info
	{
		comment_position height, width;
	};

	struct comment_analysis_group
	{
		const wchar_t* begin;
		const wchar_t* end;
		comment_font_type font;

		comment_analysis_group( const wchar_t* begin, const wchar_t* end, comment_font_type font )
		{
			this->begin = begin;
			this->end = end;
			this->font = font;
		}
	};
	struct comment_analysis_data
		: public ::std::deque<comment_analysis_group>
	{ };

	class rendering_comment;
	class renderer_driver
	{
	public:
		virtual comment_time now() const noexcept = 0;
		virtual comment_text_info text_info( rendering_comment& comment ) const noexcept = 0;
		virtual void render( const std::deque<rendering_comment*>& comments ) noexcept = 0;

		virtual void* initialize_extra_data() const noexcept = 0;
		virtual void release_extra_data( void* ptr ) const noexcept = 0;
	};

} }