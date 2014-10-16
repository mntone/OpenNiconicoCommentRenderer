#pragma once
#include "comment_base.h"
#include "IComment.h"

namespace nico { namespace renderer {

	class comment_wrapper final
		: public comment_base
	{
	public:
		comment_wrapper( ::Mntone::Nico::Renderer::IComment^ comment );

		virtual const wchar_t* const value() const noexcept;
		virtual size_t length() const noexcept;
		virtual bool self() const noexcept;
		virtual bool cyalume() const noexcept;
		virtual comment_vertical_position_type vertical_position() const noexcept;
		virtual comment_size_type size() const noexcept;
		virtual comment_color color() const noexcept;

	private:
		::Mntone::Nico::Renderer::IComment^ comment_;
	};

} }