#pragma once
#include <renderer.h>

class comment_sample final
	: private nico::renderer::comment_base
{
public:
	comment_sample() = delete;
	comment_sample( const comment_sample& ) = default;
	comment_sample( comment_sample&& ) = default;

	comment_sample& operator=( const comment_sample& rhs ) noexcept
	{
		comment_sample( rhs ).swap( *this );
		return *this;
	}
	comment_sample& operator=( comment_sample&& ) = default;

	void swap( comment_sample& other ) noexcept
	{
		using ::std::swap;
		swap( value_, other.value_ );
		swap( self_, other.self_ );
		swap( vertical_position_, other.vertical_position_ );
	}

	friend void swap( comment_sample& left, comment_sample& right ) noexcept
	{
		left.swap( right );
	}

public:
	comment_sample(
		std::wstring value,
		bool self,
		bool cyalume,
		nico::renderer::comment_vertical_position_type vertical_position,
		nico::renderer::comment_size_type size,
		nico::renderer::comment_color color )
		: value_( std::move( value ) )
		, self_( std::move( self ) )
		, cyalume_( std::move( cyalume ) )
		, vertical_position_( std::move( vertical_position ) )
		, size_( std::move( size ) )
		, color_( std::move( color ) )
	{ }

	virtual const wchar_t* const value() const noexcept { return value_.c_str(); }
	virtual size_t length() const noexcept { return value_.length(); }
	virtual bool self() const noexcept { return self_; }
	virtual bool cyalume() const noexcept { return cyalume_; }
	virtual nico::renderer::comment_vertical_position_type vertical_position() const noexcept { return vertical_position_; }
	virtual nico::renderer::comment_size_type size() const noexcept { return size_; }
	virtual nico::renderer::comment_color color() const noexcept { return color_; }
	virtual nico::renderer::comment_time time() const noexcept { return nico::renderer::comment_time::zero(); }

private:
	std::wstring value_;
	bool self_, cyalume_;
	nico::renderer::comment_vertical_position_type vertical_position_;
	nico::renderer::comment_size_type size_;
	nico::renderer::comment_color color_;
};