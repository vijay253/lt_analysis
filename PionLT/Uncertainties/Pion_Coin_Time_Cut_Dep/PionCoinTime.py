import os
from odf.opendocument import OpenDocumentSpreadsheet
from odf.table import Table, TableRow, TableCell
from odf.text import P

# Folders and files
folders = {
    "Standard": "Standard",
    "Loose": "10%Wider",
    "Tight": "10%Narrower"
}

epsilons = ["286", "629", "781"]  # your three epsilon values
file_template = "x_unsep.pl_375_{}"

# Read first column from a file
def read_first_column(file_path):
    values = []
    with open(file_path, "r") as f:
        for line in f:
            line = line.strip()
            if line:  # skip empty lines
                val = line.split()[0]  # first column
                values.append(float(val))
    return values

# Create spreadsheet
doc = OpenDocumentSpreadsheet()
table = Table(name="PionCoinTimeCutChanges")
doc.spreadsheet.addElement(table)

# Prepare headers
headers = []
for eps in epsilons:
    headers.extend([
        f"Unsep X at {eps} Std Coin Time Cuts",
        f"Unsep X at {eps} 10% Loose Coin Time Cuts",
        f"Loose Syst at {eps}",
        f"Unsep X at {eps} 10% Tight Coin Time Cuts",
        f"Tight Syst at {eps}"
    ])

# Write headers as first row
header_row = TableRow()
for h in headers:
    cell = TableCell()
    cell.addElement(P(text=h))
    header_row.addElement(cell)
table.addElement(header_row)

# Read all data
data = {}
for eps in epsilons:
    std_file = os.path.join(folders["Standard"], file_template.format(eps))
    loose_file = os.path.join(folders["Loose"], file_template.format(eps))
    tight_file = os.path.join(folders["Tight"], file_template.format(eps))
    
    std_values = read_first_column(std_file)
    loose_values = read_first_column(loose_file)
    tight_values = read_first_column(tight_file)
    
    diff_loose = [s - l for s, l in zip(std_values, loose_values)]
    diff_tight = [s - t for s, t in zip(std_values, tight_values)]
    
    data[eps] = {
        "std": std_values,
        "loose": loose_values,
        "std_loose": diff_loose,
        "tight": tight_values,
        "std_tight": diff_tight
    }

# Find maximum number of rows
max_rows = max(len(data[eps]["std"]) for eps in epsilons)

# Write data vertically
for i in range(max_rows):
    row = TableRow()
    for eps in epsilons:
        std_col = data[eps]["std"]
        loose_col = data[eps]["loose"]
        diff_loose_col = data[eps]["std_loose"]
        tight_col = data[eps]["tight"]
        diff_tight_col = data[eps]["std_tight"]
        
        row_values = [
            std_col[i] if i < len(std_col) else "",
            loose_col[i] if i < len(loose_col) else "",
            diff_loose_col[i] if i < len(diff_loose_col) else "",
            tight_col[i] if i < len(tight_col) else "",
            diff_tight_col[i] if i < len(diff_tight_col) else ""
        ]
        
        for val in row_values:
            cell = TableCell()
            cell.addElement(P(text=str(val)))
            row.addElement(cell)
    table.addElement(row)

# Save spreadsheet
output_file = "output/PionCoinTimeCut.ods"
doc.save(output_file)
print(f"Spreadsheet saved as {output_file}")
