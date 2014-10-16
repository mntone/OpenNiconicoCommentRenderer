#include "pch.h"
#include "comment_wrapper.h"

using namespace nico::renderer;

comment_wrapper::comment_wrapper( ::Mntone::Nico::Renderer::IComment^ comment )
	: comment_( comment )
{ }

const wchar_t* const comment_wrapper::value() const noexcept
{
	return comment_ ? comment_->Value->Data() : nullptr;
}

size_t comment_wrapper::length() const noexcept
{
	return comment_ ? comment_->Value->Length() : 0;
}

bool comment_wrapper::self() const noexcept
{
	return comment_->IsSelf;
}

bool comment_wrapper::cyalume() const noexcept
{
	return comment_->IsCyalume;
}

comment_vertical_position_type comment_wrapper::vertical_position() const noexcept
{
	return static_cast<comment_vertical_position_type>( comment_->VerticalPosition );
}

comment_size_type comment_wrapper::size() const noexcept
{
	return static_cast<comment_size_type>( comment_->Size );
}

comment_color comment_wrapper::color() const noexcept
{
	return static_cast<comment_color>( comment_->Color );
}