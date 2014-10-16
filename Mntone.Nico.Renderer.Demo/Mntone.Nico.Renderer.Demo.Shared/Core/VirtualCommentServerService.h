#pragma once
#include "Data\SimpleComment.h"
#include "CommentReceivedEventHandler.h"

namespace Mntone { namespace Nico { namespace Renderer { namespace Demo { namespace Core {

	public ref class VirtualCommentServerService sealed
	{
	public:
		VirtualCommentServerService();

		void AddComment( Data::SimpleComment^ comment );

	public:
		event CommentReceivedEventHandler^ CommentReceived;
	};

} } } } }