#include "helpers.h"

c_helpers g_helpers = c_helpers( );

void c_helpers::trace_line ( Vector& vec_abs_start, Vector& vec_abs_end, const unsigned int mask, C_BaseEntity* ignore, CGameTrace* ptr )
{
    CTraceFilter filter;
    filter.pSkip = ignore;
    Ray_t ray;
    ray.Init ( vec_abs_start, vec_abs_end );
    interfaces::engine_trace->trace_ray ( ray, mask, &filter, ptr );
}
