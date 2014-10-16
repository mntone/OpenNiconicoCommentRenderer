#include "pch.h"
#include "VirtualCommentServerService.h"

using namespace Mntone::Nico::Renderer::Demo::Core;
using namespace Mntone::Nico::Renderer::Demo::Data;

VirtualCommentServerService::VirtualCommentServerService()
{ }

void VirtualCommentServerService::AddComment( SimpleComment^ comment )
{
	CommentReceived( comment );
}