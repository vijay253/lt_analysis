#### Python script to plot Tree and Save into root file
#### Author, V.K., Dated Jan 15, 2025 
import ROOT
import argparse

# Set up command-line argument parsing
parser = argparse.ArgumentParser(description="Process and plot variables from a ROOT file.")
parser.add_argument("-i", "--input", required=True, help="Input ROOT file name")
parser.add_argument("-o", "--output", required=True, help="Output ROOT file name")
parser.add_argument("-t", "--tree", required=True, help="Name of the TTree")
parser.add_argument("-v", "--variable", required=True, help="Name of the variable to plot")

args = parser.parse_args()

# Assign arguments to variables
input_file_name = args.input
output_file_name = args.output
tree_name = args.tree
variable_name = args.variable

# Open the input ROOT file
input_file = ROOT.TFile.Open(input_file_name, "READ")
if not input_file or input_file.IsZombie():
    print(f"Error: Cannot open input file {input_file_name}")
    exit(1)

# Get the tree from the input file
tree = input_file.Get(tree_name)
if not tree:
    print(f"Error: TTree '{tree_name}' not found in {input_file_name}")
    input_file.Close()
    exit(1)

# Create a histogram for the variable
histogram = ROOT.TH1F("histogram", f"{variable_name} Distribution", 100, 0, 80)  # Adjust bins and range

# Fill the histogram from the TTree
tree.Project("histogram", variable_name)

# Draw the histogram
canvas = ROOT.TCanvas("canvas", "Canvas", 800, 600)
histogram.Draw()
canvas.SaveAs("output_plot.png")  # Save plot as PNG

# Save the histogram to a new ROOT file
output_file = ROOT.TFile.Open(output_file_name, "RECREATE")
if not output_file or output_file.IsZombie():
    print(f"Error: Cannot create output file {output_file_name}")
    input_file.Close()
    exit(1)

histogram.Write()  # Write histogram to output ROOT file
output_file.Close()

print(f"Histogram saved to {output_file_name} and plot saved as 'output_plot.png'.")

# Close the input ROOT file
input_file.Close()
