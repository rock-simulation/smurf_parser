#include "SMURFParser.h"
#include <stdio.h>

int main(int argc , char** argv)
{
    std::string path="/home/basadi/Desktop/mantis.git/smurf/";
    configmaps::ConfigMap* map=new configmaps::ConfigMap;
    std::string smurffilename="mantis.smurf";
    bool expandURIs;
    std::string absolute_urdf_path;
    std::string absolute_srdf_path;
    std::vector<std::string> absolute_conf_yml_files;

    smurf_parser::getFilesAbsolutePath(path,map,smurffilename, expandURIs, absolute_urdf_path,absolute_srdf_path,absolute_conf_yml_files);
    std::cout<<"absolute_urdf_path: " <<absolute_urdf_path<<std::endl;
    std::cout<<"absolute_srdf_path: " <<absolute_srdf_path<<std::endl;
    for(std::size_t i=0;i<absolute_conf_yml_files.size();i++)
    {
        std::cout<<"absolute_conf_yml_files: " <<absolute_conf_yml_files.at(i) <<std::endl;
    }

}
