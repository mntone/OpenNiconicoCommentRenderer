#pragma once

namespace Mntone { namespace Nico { namespace Renderer {

	public enum class CommentVerticalPositionType
	{
		Top = 1,
		Center,
		Bottom,
	};

	public enum class CommentSizeType
	{
		Large = 39,
		Medium = 24,
		Small = 15,
	};

	public enum class CommentModeType
	{
		Default = 1,
		Top,
		Bottom,
		Both,
		Back,
	};

	public interface class IComment
	{
		property ::Platform::String^ Value
		{
			::Platform::String^ get();
		}
		property bool IsSelf
		{
			bool get();
		}
		property bool IsCyalume
		{
			bool get();
		}
		property CommentVerticalPositionType VerticalPosition
		{
			CommentVerticalPositionType get();
		}
		property CommentSizeType Size
		{
			CommentSizeType get();
		}
		property uint32 Color
		{
			uint32 get();
		}
	};

} } }