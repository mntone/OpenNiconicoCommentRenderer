#pragma once
#include <cstdint>
#include <set>
#include "comment_base.h"

namespace nico { namespace renderer {

	class comment_analyzer final
	{
	public:
		static void initialize();
		static void analysis( rendering_comment& comment );

	private:
		static bool is_arial( wchar_t charactor );
		static bool is_ms_p_gothic( wchar_t charactor );
		static bool is_sim_sun( wchar_t charactor );
		static bool is_gulim( wchar_t charactor );
		static bool is_p_ming_liu( wchar_t charactor );

	private:
		static ::std::set<uint32_t> arial_set_, ms_p_gothic_set_, sim_sun_set_, gulim_set_;
	};

} }