#pragma once

//-----------------------------------------------------------------------------

#include "package_properties.h"
#include "entity_properties.h"
#include "common_types.h"

//-----------------------------------------------------------------------------

entities_t get_entities ( std::string const& filename_skoda_options )
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

entities_price_t get_entities_price ( std::string const& filename_skoda_options_price )
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

packages_t get_packages ( std::string const& filename_skoda_packages )
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


