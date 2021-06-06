#pragma once

//-----------------------------------------------------------------------------

#include "common_types.h"

//-----------------------------------------------------------------------------

#include <tuple>

//-----------------------------------------------------------------------------

struct entity_properties_t
{
  entity_price_t price = 0;
  entity_id_t id;
  entity_desc_t description;

  friend bool operator< ( entity_properties_t const& my,
                          entity_properties_t const& other )
  {
    auto const this_tuple = std::make_tuple ( my.price,
                            my.id,
                            my.description );
    auto const other_tuple = std::make_tuple ( other.price,
                             other.id,
                             other.description );
    return this_tuple < other_tuple;
  }
};

//-----------------------------------------------------------------------------

using entity_set_t = std::set<entity_properties_t>;
using entities_t = std::map<entity_id_t, entity_properties_t>;


