#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <set>
#include <tuple>

//-----------------------------------------------------------------------------

using entity_id_t = std::string;
using entity_desc_t = std::string;
using entity_price_t = double;

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

using entities_price_t = std::map<entity_id_t, entity_price_t>;

using entities_t = std::map<entity_id_t, entity_properties_t>;

using entity_set_t = std::set<entity_properties_t>;

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

using packages_t = std::map<entity_id_t, package_properties_t>;

//-----------------------------------------------------------------------------

constexpr auto const filename_skoda_options = "skoda_options.txt";
constexpr auto const filename_skoda_options_price = "skoda_options_price.txt";
constexpr auto const filename_skoda_packages = "skoda_packages.txt";
constexpr auto const filename_skoda_parsed_packages = "skoda_parsed_packages.txt";

//-----------------------------------------------------------------------------

entities_t get_entities()
{
  entities_t result;

  std::ifstream in_options ( filename_skoda_options );

  if ( in_options.is_open() )
  {
    while ( not in_options.eof() )
    {
      std::string txt;
      std::getline ( in_options, txt );

      std::stringstream ss ( txt );

      entity_id_t id;
      ss >> id;
      entity_desc_t desc;
      desc = ss.str().substr ( id.length() );

      result[id] = {0.0, id, desc}; // TODO: add price
    }
  }

  return result;
}

//-----------------------------------------------------------------------------

entities_price_t get_entities_price()
{
  entities_price_t result;

  std::ifstream in_options ( filename_skoda_options_price );

  if ( in_options.is_open() )
  {
    while ( not in_options.eof() )
    {
      std::string txt;
      std::getline ( in_options, txt );

      std::stringstream ss ( txt );

      entity_id_t id;
      ss >> id;
      entity_price_t price;
      ss >> price;

      result[id] = price;
    }
  }

  return result;
}

//-----------------------------------------------------------------------------

packages_t get_packages()
{
  packages_t result;

  std::ifstream in_packages ( filename_skoda_packages );

  if ( in_packages.is_open() )
  {
    while ( not in_packages.eof() )
    {
      std::string txt;
      std::getline ( in_packages, txt );

      std::stringstream ss ( txt );

      entity_id_t id;
      ss >> id;

      if ( not id.empty() )
      {
        entity_set_t entity_set;
        entity_id_t id_bis;

        for ( ss >> id_bis; id_bis.length(); ss >> id_bis )
        {
          entity_set.emplace ( 0.0, // TODO: add price
                               id_bis,
                               "" ); // TODO: add description
          id_bis.clear();
        }

        result[id].entity_set = entity_set;
      }
    }
  }

  return result;
}

//-----------------------------------------------------------------------------

packages_t get_packages_human ( entities_t const& entities,
                                entities_price_t const& entities_price,
                                packages_t const& packages )
{
  ( void ) entities_price;
  packages_t result;

  for ( auto const& entity : entities )
  {
    entity_set_t options_descriptions;
    auto const package_ptr = packages.find ( entity.first );
    auto const is_package = packages.cend() != package_ptr;

    if ( is_package )
    {
      for ( auto const& entity_of_package : package_ptr->second.entity_set )
      {
        auto const description_ptr = entities.find ( entity_of_package.id );
        auto const is_entity_in_package = entities.cend() != description_ptr;

        if ( is_entity_in_package )
        {
          options_descriptions.emplace (
            0.0, // TODO: add price
            entity_of_package.id,
            "(" + entity_of_package.id + ") " + entities.at ( description_ptr->first ).description
          );
        }
      }

      result["(" + entity.first + ") " + entity.second.description] =
      {
        options_descriptions.size(),
        0, // TODO: add entities price count
        package_ptr->second.package_price,
        0, // TODO: app options price
        options_descriptions
      };
    }
  }

  return result;
}

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
  entities_t const entities = get_entities();

  entities_price_t const entities_price = get_entities_price();

  packages_t const packages = get_packages();

  packages_t const packages_human = get_packages_human ( entities,
                                    entities_price,
                                    packages );

  store_packages_human ( packages_human );

  return 0;
}
