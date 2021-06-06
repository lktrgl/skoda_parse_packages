#pragma once

//-----------------------------------------------------------------------------

#include "entity_properties.h"
#include "common_types.h"

//-----------------------------------------------------------------------------

#include <tuple>

//-----------------------------------------------------------------------------

struct package_properties_t
{
  size_t entities_count = 0;
  size_t entities_price_count = 0;
  entity_price_t package_price = 0;
  entity_price_t options_price = 0;
  entity_set_t entity_set;

  friend bool operator< ( package_properties_t const& my,
                          package_properties_t const& other )
  {
    auto const this_tuple = std::make_tuple ( my.entities_count,
                            my.entities_price_count,
                            my.package_price,
                            my.options_price,
                            my.entity_set );
    auto const other_tuple = std::make_tuple ( other.entities_count,
                             other.entities_price_count, other.package_price,
                             other.options_price,
                             other.entity_set );
    return this_tuple < other_tuple;
  }
};

//-----------------------------------------------------------------------------

using packages_t = std::map<entity_id_t, package_properties_t>;
