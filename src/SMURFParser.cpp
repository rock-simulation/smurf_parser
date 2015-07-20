/*
 *  Copyright 2015, DFKI GmbH Robotics Innovation Center
 *
 *  This file is part of the MARS simulation framework.
 *
 *  MARS is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License
 *  as published by the Free Software Foundation, either version 3
 *  of the License, or (at your option) any later version.
 *
 *  MARS is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with MARS.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 *
 * \file SMURFParser.cpp
 * \author Kai von Szadkowski
 */

#include "SMURFParser.h"
#include <urdf_parser/urdf_parser.h>
#include <boost/filesystem.hpp>

namespace smurf_parser {

  boost::shared_ptr<urdf::ModelInterface> parseFile(configmaps::ConfigMap* map,
          std::string path, std::string smurffilename, bool expandURIs) {


    // parse yaml data and write to provided map, identify path to URDF file
    map->append(configmaps::ConfigMap::fromYamlFile(path+smurffilename, expandURIs));
    //map->toYamlFile("smurfparserdebug.yml");
    std::string urdfpath = "";
    configmaps::ConfigVector::iterator it;
    for(it = (*map)["files"].begin(); it!=(*map)["files"].end(); ++it) {
      boost::filesystem::path filepath((std::string)(*it));
      if(filepath.extension().generic_string() == ".urdf") {
        urdfpath = path + filepath.generic_string();
      }
      else if(filepath.extension() == ".yml") {
        configmaps::ConfigMap tmpconfig =
            configmaps::ConfigMap::fromYamlFile(path+filepath.generic_string());
        (*map).append(tmpconfig);
      }
    }

    // parse URDF model and return
    fprintf(stderr, "  ...loading urdf data from %s.\n", urdfpath.c_str());
    boost::shared_ptr<urdf::ModelInterface> model = urdf::parseURDFFile(urdfpath);
    if (!model) {
      return boost::shared_ptr<urdf::ModelInterface>();
    }
    return model;
  }



boost::filesystem::path resolve_path( const boost::filesystem::path& p, const boost::filesystem::path& base )
{
    boost::filesystem::path abs_p = boost::filesystem::absolute(p,base);
    boost::filesystem::path result;
    for(boost::filesystem::path::iterator it=abs_p.begin(); it!=abs_p.end(); ++it)
    {
        if(*it == "..")
        {
            // /a/b/.. is not necessarily /a if b is a symbolic link
            if(boost::filesystem::is_symlink(result) )
            result /= *it;
            // /a/b/../.. is not /a/b/.. under most circumstances
            // We can end up with ..s in our result because of symbolic links
            else if(result.filename() == "..")
            result /= *it;
            // Otherwise it should be safe to resolve the parent
            else
            result = result.parent_path();
        }
        else if(*it == ".")
        {
            // Ignore
        }
        else
        {
            // Just cat other path entries
            result /= *it;
        }
    }
    return result;
}



void getFilesAbsolutePath(std::string path,configmaps::ConfigMap* map, std::string smurffilename,bool expandURIs ,std::string &absolute_urdf_path,std::string &absolute_srd_fpath, std::vector<std::string> &absolute_conf_yml_files)
{
    map->append(configmaps::ConfigMap::fromYamlFile(path+smurffilename, expandURIs));
    configmaps::ConfigVector::iterator it;
    boost::filesystem::path abs_path_of_file;


    for(it = (*map)["files"].begin(); it!=(*map)["files"].end(); ++it)
    {
        boost::filesystem::path filepath((std::string)(*it));
        if(filepath.extension().generic_string() == ".urdf")
        {
            abs_path_of_file=resolve_path(filepath.generic_string(),path) ;
            absolute_urdf_path=abs_path_of_file.string();
        }
        if(filepath.extension().generic_string() == ".srdf")
        {
            abs_path_of_file=resolve_path(filepath.generic_string(),path) ;
            absolute_srd_fpath=abs_path_of_file.string();
        }
        else if(filepath.extension() == ".yml")
        {
            abs_path_of_file=resolve_path(filepath.generic_string(),path) ;
            absolute_conf_yml_files.push_back(abs_path_of_file.string() );
        }
    }
    return;
}

}
