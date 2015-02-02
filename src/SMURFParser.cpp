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
    // retrieve yaml data
    map->fromYamlFile(path+smurffilename, expandURIs);

    // parse yaml data and write to provided map, identify path to URDF file
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
        (*map)[filepath.stem().generic_string()] = tmpconfig; //FIXME: use name IN the file?
      }
    }

    // parse URDF model and return
    fprintf(stderr, "  ...loading urdf data from %s.\n", urdfpath.c_str());
    boost::shared_ptr<urdf::ModelInterface> model = urdf::parseURDFFile(urdfpath);
    urdf::Joint ajoint;
    if (!model) {
      return boost::shared_ptr<urdf::ModelInterface>();
    }
    return model;
  }

}
