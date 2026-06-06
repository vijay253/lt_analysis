import uproot
import awkward as ak
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.path import Path

# Function to check if a point is inside a diamond defined by four vertices
def is_in_diamond(x, y, vertices):
    """
    Check if a point (x, y) lies within a diamond shape defined by four vertices.
    
    Parameters:
    - x, y: Coordinates of the point.
    - vertices: List of four vertices [(x1, y1), (x2, y2), (x3, y3), (x4, y4)] in order.
    
    Returns:
    - True if the point is inside the diamond, False otherwise.
    """
    
    def cross_product(a, b, c):
        return (b[0] - a[0]) * (c[1] - a[1]) - (b[1] - a[1]) * (c[0] - a[0])
    
    # Check if the point is on the same side of all edges
    d1 = cross_product(vertices[0], vertices[1], (x, y))
    d2 = cross_product(vertices[1], vertices[2], (x, y))
    d3 = cross_product(vertices[2], vertices[3], (x, y))
    d4 = cross_product(vertices[3], vertices[0], (x, y))
    
    return (d1 >= 0 and d2 >= 0 and d3 >= 0 and d4 >= 0) or (d1 <= 0 and d2 <= 0 and d3 <= 0 and d4 <= 0) 
    
# Define the ROOT file paths and tree/branch names
file_paths = ["/volatile/hallc/c-kaonlt/vijay/Analysis/Q2/OUTPUT_PYTHON/Analysed_Data_Q0p425W2p2_lowe_Center.root", "/volatile/hallc/c-kaonlt/vijay/Analysis/Q2/OUTPUT_PYTHON/Analysed_Data_Q0p425W2p2_mide_Center.root", "/volatile/hallc/c-kaonlt/vijay/Analysis/Q2/OUTPUT_PYTHON/Analysed_Data_Q0p425W2p2_highe_Center.root"]
tree_name = "Cut_Kaon_Events_prompt_noRF"
branch_x = "Q2"  # Replace with your actual branch name for the x variable
branch_y = "W"  # Replace with your actual branch name for the y variable
cut_var = "CTime_ROC1"
cut_value1 =43
cut_value2 = 45

sacceptace = "ssdelta"
hacceptance = "hsdelta"

sacceptace1_cut_value = -10
sacceptace2_cut_value =  20
hacceptance1_cut_value = -8
hacceptance2_cut_value = 8

# Open the first file to determine the diamond vertices
with uproot.open(file_paths[0]) as file:
    tree = file[tree_name]
    sacceptace1 = tree[sacceptace].array()
    sacceptace1 = ak.to_numpy(sacceptace1)
    scondition1 = (sacceptace1 > sacceptace1_cut_value)
    
    sacceptace2 = tree[sacceptace].array()
    sacceptace2 = ak.to_numpy(sacceptace2)
    scondition2 = (sacceptace2 < sacceptace2_cut_value)
    
    hacceptance1 = tree[hacceptance].array()
    hacceptance1 = ak.to_numpy(hacceptance1)
    hcondition1 = (hacceptance1 > hacceptance1_cut_value)
    
    hacceptance2 = tree[hacceptance].array()
    hacceptance2 = ak.to_numpy(hacceptance2)
    hcondition2 = (hacceptance2 < hacceptance2_cut_value)
    
    coitime1 = tree[cut_var].array()
    coitime1 = ak.to_numpy(coitime1)
    coitime2 = tree[cut_var].array()
    coitime2 = ak.to_numpy(coitime2)
    coitimecut1 = (coitime1 > cut_value1)
    coitimecut2 = (coitime2 > cut_value2)

    x_values = tree[branch_x].array()
    y_values = tree[branch_y].array()

    x_values = x_values[coitimecut1 & coitimecut2 & scondition1 & scondition2 & hcondition1 & hcondition2]
    y_values = y_values[coitimecut1 & coitimecut2 & scondition1 & scondition2 & hcondition1 & hcondition2]
      
    x_values = x_values.to_numpy()
    y_values = y_values.to_numpy()

    # Find the vertices of the diamond (assuming the diamond is small and well-defined)
    # Here, we assume the diamond is the convex hull of the points in the first file
    points = np.vstack((x_values, y_values)).T
    from scipy.spatial import ConvexHull
    hull = ConvexHull(points)
    vertices = points[hull.vertices]  # Get the vertices of the convex hull
'''
x_min = vertices[np.argmin(vertices[:,0])]
x_max = vertices[np.argmax(vertices[:,0])]
y_min = vertices[np.argmin(vertices[:,1])]
y_max = vertices[np.argmax(vertices[:,1])]
'''
vertices = np.array([[0.39779305,2.18847622],[0.33438134,2.23125977], [0.44894876,2.20543771], [0.51742541,2.16145995], [0.39779305,2.1884762]])

# Print the vertices for verification
print("Diamond vertices:")
#for vertex in vertices:
 #   print(vertex)
print(vertices)

# Initialize lists to store selected events
selected_events = []
full_events_file1 = []
full_events_file2 = []
full_events_file3 = []

# Loop over the other two files and apply the diamond selection
for file_path in file_paths[0:]:
    with uproot.open(file_path) as file:
        tree = file[tree_name]

        x_values = tree[branch_x].array()
        y_values = tree[branch_y].array()
    
        x_values = x_values[coitimecut1 & coitimecut2 & scondition1 & scondition2 & hcondition1 & hcondition2]
        y_values = y_values[coitimecut1 & coitimecut2 & scondition1 & scondition2 & hcondition1 & hcondition2]

        x_values = x_values.to_numpy()
        y_values = y_values.to_numpy()

        # Loop over events and apply the diamond selection
        for x, y in zip(x_values, y_values):
            if is_in_diamond(x, y, vertices):
                selected_events.append((x, y))
                
with uproot.open(file_paths[0]) as file:
    tree = file[tree_name]
    x_values = tree[branch_x].array()
    y_values = tree[branch_y].array()
    
    x_values = x_values[coitimecut1 & coitimecut2 & scondition1 & scondition2 & hcondition1 & hcondition2]
    y_values = y_values[coitimecut1 & coitimecut2 & scondition1 & scondition2 & hcondition1 & hcondition2]

    x_values = x_values.to_numpy()
    y_values = y_values.to_numpy()
    
    # Loop over events and apply the diamond selection
    for x, y in zip(x_values, y_values):
        full_events_file1.append((x, y))

with uproot.open(file_paths[1]) as file:
    tree = file[tree_name]
    x_values = tree[branch_x].array()
    y_values = tree[branch_y].array()

    x_values = x_values[coitimecut1 & coitimecut2 & scondition1 & scondition2 & hcondition1 & hcondition2]
    y_values = y_values[coitimecut1 & coitimecut2 & scondition1 & scondition2 & hcondition1 & hcondition2]

    x_values = x_values.to_numpy()
    y_values = y_values.to_numpy()
    
    # Loop over events and apply the diamond selection
    for x, y in zip(x_values, y_values):
        full_events_file2.append((x, y))

with uproot.open(file_paths[2]) as file:
    tree = file[tree_name]
    x_values = tree[branch_x].array()
    y_values = tree[branch_y].array()

    x_values = x_values[coitimecut1 & coitimecut2 & scondition1 & scondition2 & hcondition1 & hcondition2]
    y_values = y_values[coitimecut1 & coitimecut2 & scondition1 & scondition2 & hcondition1 & hcondition2]
    
    x_values = x_values.to_numpy()
    y_values = y_values.to_numpy()
    
    # Loop over events and apply the diamond selection
    for x, y in zip(x_values, y_values):
        full_events_file3.append((x, y))
        
# Convert selected events to a numpy array for easier manipulation
selected_events = np.array(selected_events)
full_events_file1 = np.array(full_events_file1)
full_events_file2 = np.array(full_events_file2)
full_events_file3 = np.array(full_events_file3)

print("Shape of selected_events:", selected_events.shape)

# Ensure selected_events is 2D
if selected_events.ndim == 1:
    #If selected_events is 1D, reshape it to (N, 2)
    selected_events = selected_events.reshape(-1, 2)
plt.figure(figsize=(5, 4))
# Plot full events from file3
plt.scatter(full_events_file3[:, 0], full_events_file3[:, 1],s=0.1, color='red',  alpha=0.5)
# Plot full events from file2
plt.scatter(full_events_file2[:, 0], full_events_file2[:, 1],s=0.1, color='green', alpha=0.5)
# Plot full events from file1
plt.scatter(full_events_file1[:, 0], full_events_file1[:, 1], s=0.1, color='blue', alpha=0.5)
# Plot the selected events
plt.scatter(selected_events[:, 0], selected_events[:, 1],s=0.1, color='purple', label="Selected Diamond")
plt.plot(vertices[:, 0], vertices[:, 1], 'r--', label="Diamond Boundary")
plt.xlabel(branch_x)
plt.ylabel(branch_y)
plt.legend(markerscale=20)
plt.xlabel(r"$Q^2$ ($GeV^2$)")
plt.ylabel(r"$W$ (GeV)")
plt.title("$Q^2$=0.425")
plt.savefig('Diamond_Plot_Cut.png')
plt.show()

# Print the number of selected events
print(f"Number of selected events: {len(selected_events)}")
