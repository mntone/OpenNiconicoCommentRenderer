#pragma once

namespace Mntone { namespace Nico { namespace Renderer { namespace Demo { namespace Data {

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class SimpleComment sealed
		: public ::Mntone::Nico::Renderer::IComment
	{
	public:
		SimpleComment(
			::Platform::String^ value,
			bool isSelf,
			bool isCyalume,
			::Mntone::Nico::Renderer::CommentVerticalPositionType verticalPosition,
			::Mntone::Nico::Renderer::CommentSizeType size,
			uint32 color );

	public:
		property ::Platform::String^ Value
		{
			virtual ::Platform::String^ get() { return Value_; }
		}
		property bool IsSelf
		{
			virtual bool get() { return IsSelf_; }
		}
		property bool IsCyalume
		{
			virtual bool get() { return IsCyalume_; }
		}
		property ::Mntone::Nico::Renderer::CommentVerticalPositionType VerticalPosition
		{
			virtual ::Mntone::Nico::Renderer::CommentVerticalPositionType get() { return VerticalPosition_; }
		}
		property ::Mntone::Nico::Renderer::CommentSizeType Size
		{
			virtual ::Mntone::Nico::Renderer::CommentSizeType get() { return Size_; }
		}
		property uint32 Color
		{
			virtual uint32 get() { return Color_; }
		}

	private:
		::Platform::String^ Value_;
		bool IsSelf_, IsCyalume_;
		::Mntone::Nico::Renderer::CommentVerticalPositionType VerticalPosition_;
		::Mntone::Nico::Renderer::CommentSizeType Size_;
		uint32 Color_;
	};

} } } } }