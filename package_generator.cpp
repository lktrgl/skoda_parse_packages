#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <set>
#include <tuple>

//-----------------------------------------------------------------------------

#include "data_loader.h"

//-----------------------------------------------------------------------------

constexpr auto const filename_skoda_options = "skoda_options.txt";
constexpr auto const filename_skoda_options_price = "skoda_options_price.txt";
constexpr auto const filename_skoda_packages = "skoda_packages.txt";
constexpr auto const filename_skoda_parsed_packages = "skoda_parsed_packages.txt";

//-----------------------------------------------------------------------------

void store_packages_human ( packages_t const& packages_human )
{
  std::ofstream out_packages_human ( filename_skoda_parsed_packages );

  for ( auto const& p : packages_human )
  {
    out_packages_human << "* " << p.first << std::endl;

    for ( auto const& d : p.second.entity_set )
    {
      out_packages_human << " ** " << d.description << std::endl;
    }
  }
}

//-----------------------------------------------------------------------------

int main()
{
  entities_t const entities = get_entities ( filename_skoda_options );

  entities_price_t const entities_price = get_entities_price ( filename_skoda_options_price );

  packages_t const packages = get_packages ( filename_skoda_packages );

  packages_t const packages_human = get_packages_human ( entities,
                                    entities_price,
                                    packages );

  store_packages_human ( packages_human );

  return 0;
}
