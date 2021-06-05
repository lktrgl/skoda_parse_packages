#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <set>

//-----------------------------------------------------------------------------

using entity_id_t = std::string;
using entity_desc_t = std::string;
using entities_t = std::map<entity_id_t, entity_desc_t>;

using entity_set_t = std::set<entity_id_t>;
using packages_t = std::map<entity_id_t, entity_set_t>;

//-----------------------------------------------------------------------------

constexpr auto const filename_skoda_options = "skoda_options.txt";
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

      result[id] = desc;
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
          entity_set.emplace ( id_bis );
          id_bis.clear();
        }

        result[id] = entity_set;
      }
    }
  }

  return result;
}

//-----------------------------------------------------------------------------

packages_t get_packages_human ( entities_t const& entities, packages_t const& packages )
{
  packages_t result;

  for ( auto const& e : entities )
  {
    entity_set_t options_descriptions;

    if ( auto const p = packages.find ( e.first ); packages.cend() != p )
    {
      for ( auto const& e2 : p->second )
      {
        if ( auto const d = entities.find ( e2 ); entities.cend() != d )
        {
          options_descriptions.emplace ( "(" + e2 + ") " + entities.at ( d->first ) );
        }
      }

      result["(" + e.first + ") " + e.second] = options_descriptions;
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

    for ( auto const& d : p.second )
    {
      out_packages_human << " ** " << d << std::endl;
    }
  }
}

//-----------------------------------------------------------------------------

int main()
{
  entities_t const entities = get_entities();

  packages_t const packages = get_packages();

  packages_t const packages_human = get_packages_human ( entities, packages );

  store_packages_human ( packages_human );

  return 0;
}
