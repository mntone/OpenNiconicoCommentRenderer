#include "pch.h"
#include "SimpleComment.h"

using namespace Platform;
using namespace Mntone::Nico::Renderer;
using namespace Mntone::Nico::Renderer::Demo::Data;

SimpleComment::SimpleComment( String^ value, bool isSelf, bool isCyalume, CommentVerticalPositionType verticalPosition, CommentSizeType size, uint32 color )
	: Value_( value )
	, IsSelf_( isSelf )
	, IsCyalume_( isCyalume )
	, VerticalPosition_( verticalPosition )
	, Size_( size )
	, Color_( color )
{ }