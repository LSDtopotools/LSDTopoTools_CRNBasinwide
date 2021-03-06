//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Soil_cosmogenic_analysis.cpp
// A program to compute cosmogenic erosion rates from soil samples.
//
// The function take two arguments to main
// The first is the data folder. This is the folder in which the parameter
// files are stored
// The second is the prefix of the parameter files
//
// Developed by:
//  Simon M. Mudd, University of Edinburgh, School of GeoSciences
//  Stuart W.D. Grieve, University of Edinburgh, School of GeoSciences
//  Marie-Alice Harel, University of Edinburgh, School of GeoSciences
//  Martin D. Hurst, British Geological Survey
//
//
// Copyright (C) 2015 Simon M. Mudd 2015
//
// Developer can be contacted by simon.m.mudd _at_ ed.ac.uk
//
//    Simon Mudd
//    University of Edinburgh
//    School of GeoSciences
//    Drummond Street
//    Edinburgh, EH8 9XP
//    Scotland
//    United Kingdom
//
// This program is free software;
// you can redistribute it and/or modify it under the terms of the
// GNU General Public License as published by the Free Software Foundation;
// either version 2 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY;
// without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// You should have received a copy of the
// GNU General Public License along with this program;
// if not, write to:
// Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301
// USA
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include "../LSDRaster.hpp"
#include "../LSDStatsTools.hpp"
#include "../LSDFlowInfo.hpp"
#include "../LSDJunctionNetwork.hpp"
#include "../LSDStrahlerLinks.hpp"
#include "../LSDBasin.hpp"
#include "../LSDParticle.hpp"
#include "../LSDCRNParameters.hpp"
#include "../LSDShapeTools.hpp"
#include "../LSDCosmoData.hpp"
using namespace std;

int main (int nNumberofArgs,char *argv[])
{
  // the driver version
  string driver_version = "Driver_version: 0.02";

  // some paramters
  //Test for correct input arguments
  if (nNumberofArgs!=3)
  {
    cout << "================================================================" << endl;
    cout << "|| Welcome to the cosmogenic analysis tool for soil samples!  ||" << endl;
    cout << "================================================================" << endl;
    cout << "This program requires two inputs: " << endl;
    cout << "* First the path to the parameter files." << endl;
    cout << "   The path must have a slash at the end." << endl;
    cout << "  (Either \\ or / depending on your operating system.)" << endl;
    cout << "* Second the prefix of the parameter files." << endl;
    cout << "---------------------------------------------------------" << endl;
    cout << "There must be three parameter files in the named path." << endl;
    cout << "The first MUST have the extension _CRNRasters.csv" << endl;
    cout << " This contains the filenames (including full path) of" << endl;
    cout << " names of the DEMs (without file extensions) to be analysed." << endl;
    cout << "The second file contains the CRN data" << endl;
    cout << "  and MUST have the extension _CRNData.csv" << endl;
    cout << "The third file must have the extension _CRNSoilInfo.csv." << endl;
    cout << "This file contains information about the soil density and sampling thickness." << endl;
    cout << "See documentation for details." << endl;
    cout << "There is also an optional field to set parameter values. " << endl;
    cout << "This file has the extension .CRNParam" << endl;
    cout << "---------------------------------------------------------" << endl;
    cout << "For example if you parameter files are in home/fieldwork/Chile/CRN/" << endl;
    cout << " in this folder you will have two parameter files, for example" << endl;
    cout << " My_data_CRNData.csv and My_data_CRNRasters.csv" << endl;
    cout << "Then the command line argument will be: " << endl;
    cout << "In linux:" << endl;
    cout << "./Basinwide_CRN.exe home/fieldwork/Chile/CRN/ My_data" << endl;
    cout << "In windows (the slash directions will change and there is no leading ./)" << endl;
    cout << "Basinwide_CRN.exe c:\\fieldwork\\Chile\\CRN\\ My_data" << endl;
    cout << "=========================================================" << endl;
    cout << "For more documentation on cosmo data file format, " << endl;
    cout << " see readme and online documentation." << endl;
    cout << " http://lsdtopotools.github.io/LSDTT_book/#_basin_averaged_cosmogenic_analysis" << endl;
    cout << "=========================================================" << endl;
    exit(EXIT_SUCCESS);
  }

  string path_name = argv[1];
  string param_name_prefix = argv[2];

  // now load the CRNCosmoData object
  cout << "I'm gonna load some data for you buddy" << endl;
  LSDCosmoData CosmoData(path_name,param_name_prefix);
  cout << "Got the data" << endl;

  cout << "===========================================================" << endl;
  cout << "Welcome to the Basinwide cosmogenic analysis tool" << endl;
  cout << "This software was developed at the University of Edinburgh," << endl;
  cout << "by the Land Surface Dynamics group. For questions email" << endl;
  cout << "simon.m.mudd _at_ ed.ac.uk" << endl;
  cout << "This software is released under a GNU public license." << endl;
  cout << "You are using " << driver_version << endl;
  cout << "===========================================================" << endl;
  cout << "Your cosmogenic data, parameters and file structures are" << endl;
  cout << "Listed in the file within path " << path_name << endl;
  cout << "With the extension .CRNParamReport" << endl;
  cout << "++IMPORTANT++ There must be ENVI bil files with these names." << endl;
  cout << "ENVI bil files are required because, unlike asc or flt files, " << endl;
  cout << "they use georeferencing information, which is used in the analyses." << endl;
  cout << "For more information about changing DEM formatting, see: " << endl;
  cout << "http://lsdtopotools.github.io/LSDTT_book/#_gdal_2" << endl;
  cout << "===========================================================" << endl;

  //cout << "Getting data" << endl;
  int method_flag = 3;
  CosmoData.calculate_erosion_rates(method_flag);

  //cout << "Printing results " << endl;
  // now print the data to a csv file
  CosmoData.print_results();
  //CosmoData.print_rasters();

}
