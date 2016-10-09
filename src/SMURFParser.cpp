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

#ifndef M_PI
#define M_PI (3.1415926535897932384626433832795)
#endif

#include "SMURFParser.h"
#include <boost/filesystem.hpp>

namespace smurf_parser {

  urdf::ModelInterfaceSharedPtr parseFile(configmaps::ConfigMap* map,
          std::string path, std::string smurffilename, bool expandURIs) {

    path+="/"; // secure that path and file are combined correctly

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
    urdf::ModelInterfaceSharedPtr model = urdf::parseURDFFile(urdfpath);
    if (!model) {
      return urdf::ModelInterfaceSharedPtr();
    }
    return model;
  }

}
