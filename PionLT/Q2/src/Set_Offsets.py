import sys
import ROOT
import uproot
import numpy as np
import awkward as ak
from uproot.writing import create

# Create a function for filling histogram

def fill_histogram(input_file, output_file, tree_name, var_name, cut_expression, offset_variable, offset_values, bins, range_min, range_max):

    with uproot.open(input_file) as file:
        tree = file[tree_name]
        
        branch_names = tree.keys()
        data = tree.arrays(branch_names, library="np")
        
        #mask = cut_expression(data)

        mask = (data["CTime_ROC1"] > 46) & (data["CTime_ROC1"] < 48)
      
        for key in data:
            if key == offset_variable:                 ##Set Offset in offset_variable you can define two variables
                data[key] = data[key]+offset_values
        
        filtered_data = {key: array[mask] for key, array in data.items()}
        
        # Create a new ROOT file and write the histogram
        with uproot.recreate(output_file) as output:
            output[tree_name] = filtered_data 

        print(f"Filtered data written to {output_file}")
       
if __name__ == "__main__":
    # Check if the correct number of arguments is provided
    if len(sys.argv) != 3:
        print("Usage: python script.py <input_file.root> <output_file.root>")
        sys.exit(1)
    
    # Get input and output file names from arguments
    input_file = sys.argv[1]
    output_file = sys.argv[2]
    
    # Define the tree name, variable name, cut expression, and histogram parameters
    tree_name = "Cut_Kaon_Events_prompt_noRF"  
    var_name = "CTime_ROC1"  
    offset_variable = "CTime_ROC1"
    offset_values = -47
    # Define the cut expression using awkward array operations
    cut_expression = lambda data: (data["CTime_ROC1"] > -30) & (data["CTime_ROC1"] < 50)  # Replace with your cut logic
    
    bins = 300  # Number of bins
    range_min = -50  # Minimum value for the histogram
    range_max = 50  # Maximum value for the histogram
    
    # Call the function to fill and write the histogram
    fill_histogram(input_file, output_file, tree_name, var_name, cut_expression, offset_variable, offset_values, bins, range_min, range_max)
    
    print(f"Histogram filled and saved to {output_file}")
