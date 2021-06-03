#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <set>

using entity_id_t = std::string;
using entity_desc_t = std::string;
using entities_t = std::map<entity_id_t, entity_desc_t>;

using entity_set_t=std::set<entity_id_t>;
using packages_t = std::map<entity_id_t, entity_set_t>;

int main()
{
  entities_t entities;
  {
    std::ifstream in_options("skoda_options.txt");
    if (in_options.is_open())
    {
      while (not in_options.eof())
      {
        std::string txt;
        std::getline(in_options, txt);

        std::stringstream ss(txt);

        entity_id_t id;
        ss >> id;
        entity_desc_t desc;
        desc = ss.str().substr(id.length());

        entities[id] = desc;
      }
    }
  }
  packages_t packages;
  {
    std::ifstream in_packages("skoda_packages.txt");

    if (in_packages.is_open())
    {
      while (not in_packages.eof())
      {
        std::string txt;
        std::getline(in_packages, txt);

        std::stringstream ss(txt);

        entity_id_t id;
        ss >> id;

        if (not id.empty())
        {
          entity_set_t entity_set;
          entity_id_t id_bis;
          for (ss >> id_bis; id_bis.length(); ss >> id_bis)
          {
            entity_set.emplace(id_bis);
            id_bis.clear();
          }

          packages[id] = entity_set;
        }
      }
    }
  }

  packages_t packages_human;
  for (auto const &e : entities)
  {
    entity_set_t options_descriptions;
    if (auto const p = packages.find(e.first); packages.cend() != p)
    {
      for (auto const &e2 : p->second)
      {
        if (auto const d = entities.find(e2); entities.cend() != d)
        {
          options_descriptions.emplace("(" + e2 + ") " + entities[d->first]);
        }
      }
      packages_human["(" + e.first + ") " + e.second] = options_descriptions;
    }
  }

  {
    std::ofstream out_packages_human("skoda_parsed_packages.txt");

    for (auto const &p : packages_human)
    {
      out_packages_human << "* " << p.first << std::endl;

      for (auto const &d : p.second)
      {
        out_packages_human << " ** " << d << std::endl;
      }
    }
  }

  return 0;
}
