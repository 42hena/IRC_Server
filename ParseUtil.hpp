#ifndef _PARSE_UTIL_HPP_
#define _PARSE_UTIL_HPP_

#include "Define.hpp"

VECTOR<STRING> split(STRING input, char delimiter);
void ParsePacket(const STRING & str, STRING &delimeter, VECTOR<STRING> &_v);

#endif