# This function is used to calcualte the parameters for the effective depth
# of snow shielding for use in the CRN calculator
"""
Created on Tue Mar 31 10:38:44 2015
@author: smudd
"""

import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit # Import the curve fitting module


# this function calculates the root mean square error
def RMSE(data,prediction):
    sum_err = 0
    n = len(data)
    residual = []
    for d,p in zip(data,prediction):
        sum_err = (p-d)*(p-d)
        residual.append(p-d)
    resid = np.asarray(residual)
    sq_resid = np.power(resid,2)
    
    #print "The square residuals are: "
    print sq_resid    
    RMSE = np.sqrt(sum_err/n)
    return RMSE


# this is a bilinear function, returns in same units as PeakSnowpack
# for CRN applications this should be in g/cm^2
def bilinear_effSWEDepth_function(z,SlopeAscend,SlopeDescend,PeakElevation,PeakSnowpack):
    
    eff_SWE_depth = []    
    
    for LocalElev in z:
        ascendEffDepth = SlopeAscend*(LocalElev-PeakElevation)+PeakSnowpack
        descendEffDepth = SlopeDescend*(LocalElev-PeakElevation)+PeakSnowpack
        
        if LocalElev > PeakElevation:
            this_SWE_depth = descendEffDepth
        else:
            this_SWE_depth = ascendEffDepth
            
        if this_SWE_depth < 0:
            this_SWE_depth = 0
        
        eff_SWE_depth.append(this_SWE_depth)
     
     
    return eff_SWE_depth
        

# THis fits a bilinear snow water equivalent curve to a bilinear function, and 
# reports the parameters of the bilinear function in effective depths (g cm^-2)
# SWE should be in mm
# for use in snow shielding calculations
def fit_bilinear_SWE_from_file(filename):
    # State the filename
    #filename = 'c:\\Users\\smudd\\Documents\\Papers\\Tidal_paper_padova\\Wind_Data\\s_andrea_ok.txt'    
    
    # First load the erosion data file. This data just has an elevation and
    # a SWE in mm on each line
    SWE_data = np.loadtxt(filename)
    elevations = SWE_data[:,0]
    SWE_in_mm = SWE_data[:,1]
    
    SWE_in_effDepth = np.divide(SWE_in_mm,10)
    
    print "The elevations: "
    print elevations
    print "And SWE in effective depth g/cm^-2: "
    print SWE_in_effDepth   
    
    
    # get some initial guesses
    test_peak = np.amax(SWE_in_effDepth)
    index_PE = np.argmax(SWE_in_effDepth)
    test_peak_elevation = elevations[index_PE]
    elev_max =  np.amax(elevations) 
    elev_min =  np.amin(elevations)    
    test_ascend = test_peak/(test_peak_elevation-elev_min)
    test_descend = test_peak/(test_peak_elevation-elev_max)
        
    # new initial guess    
    initial_guess = [test_ascend,test_descend,test_peak_elevation,test_peak]
    
    print "The initial guesses are: "
    print initial_guess


    # now fit the erosion data to a double gaussian
    popt_bl, pcov_bl = curve_fit(bilinear_effSWEDepth_function, elevations, SWE_in_effDepth,initial_guess)
    #popt_dg, pcov_dg = curve_fit(sd.double_gaussian, depth, erate)   
   
    # get the fitted pdf
    print "The fitted components are: "
    print popt_bl[0]
    print popt_bl[1]
    print popt_bl[2]
    print popt_bl[3]    
    SWE_bl_fit = bilinear_effSWEDepth_function(elevations,popt_bl[0],popt_bl[1],popt_bl[2],popt_bl[3])        
    
    
    RMSE_val =  RMSE(SWE_in_effDepth,SWE_bl_fit)
    print "The RMSE is: " + str(RMSE_val)
    
    return elevations,SWE_in_effDepth,popt_bl,SWE_bl_fit,RMSE_val



# This function plots the results from the fitting
def plot_SWE_effD_fit(elevations,SWE_in_effDepth,popt_bl,SWE_bl_fit,RMSE_val):      

    ## PLOT THE GRAPH
    plt.figure(figsize=(12,6))
    
    # The first subplot is the data, and the truncated data
    ax1=plt.subplot(111)
    ax1.plot(elevations,SWE_in_effDepth,'ro',label='data')    
    ax1.plot(elevations,SWE_bl_fit,'ko',label='fit data')   
    plt.xlabel('Elevation (m)')
    plt.ylabel('data and fit, in Effective depth g/cm^2')
    ax1.legend(loc='upper left')

    
    plot_fname = "Fit_plot_bl.png"
    plt.savefig(plot_fname, format='png')
    plt.clf()
    #plt.show()

# This function plots the results from the fitting
def write_sparam_file(filename,popt_bl):      
    f = open(filename, 'w')
    f.write("Bilinear\n")
    f.write(str(popt_bl[0])+"\n")
    f.write(str(popt_bl[1])+"\n")
    f.write(str(popt_bl[2])+"\n")
    f.write(str(popt_bl[3])+"\n")
    f.close()
    

if __name__ == "__main__":

    filename = 'T:\\analysis_for_papers\\Manny_idaho\\SWE_idaho.txt'
    sparamname = 'T:\\analysis_for_papers\\Manny_idaho\\HarringCreek.sparam'
    
    elevations,SWE_in_effDepth,popt_bl,SWE_bl_fit,RMSE_val = fit_bilinear_SWE_from_file(filename)
    plot_SWE_effD_fit(elevations,SWE_in_effDepth,popt_bl,SWE_bl_fit,RMSE_val)
    write_sparam_file(sparamname,popt_bl)
    
