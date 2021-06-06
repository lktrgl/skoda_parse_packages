#pragma once

//-----------------------------------------------------------------------------

#include <string>

//-----------------------------------------------------------------------------

using entity_id_t = std::string;
using entity_desc_t = std::string;
using entity_price_t = double;

//-----------------------------------------------------------------------------

using entities_price_t = std::map<entity_id_t, entity_price_t>;
