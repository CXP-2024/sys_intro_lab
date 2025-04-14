#!/usr/bin/env python3
import os
import re
import sys

def process_matrix_files(directory):
    """
    Process all files in the given directory to extract matrix data.
    Look for lines starting with DATA_A_ or DATA_B_, format them properly,
    and write to data/task3.txt in the format expected by task3.
    """
    # Data structures to store matrix A and B rows
    matrix_a = {}
    matrix_b = {}
    
    # Keep track of files processed
    files_processed = 0
    text_files_processed = 0
    
    # Process all files in the directory recursively
    for root, dirs, files in os.walk(directory):
        for filename in files:
            filepath = os.path.join(root, filename)
            files_processed += 1
            
            if files_processed % 10 == 0:
                print(f"Processed {files_processed} files, found {len(matrix_a)} matrix A rows and {len(matrix_b)} matrix B rows")
                
            if os.path.isfile(filepath):
                try:
                    # Try to open the file with UTF-8 encoding first
                    with open(filepath, 'r', encoding='utf-8', errors='ignore') as file:
                        text_files_processed += 1
                        print(f"Reading text file: {filepath}")
                        for line in file:
                            # Check for matrix A data with various formats
                            a_match = re.match(r'^DATA_A_(\d+)[:=,\s]+(.*)', line, re.IGNORECASE)
                            if a_match:
                                row_num = int(a_match.group(1))
                                # Replace both commas and spaces with spaces for consistent format
                                data = re.sub(r'[,\s]+', ' ', a_match.group(2).strip())
                                matrix_a[row_num] = data
                                print(f"Found matrix A row {row_num}")
                                continue
                            
                            # Check for matrix B data with various formats
                            b_match = re.match(r'^DATA_B_(\d+)[:=,\s]+(.*)', line, re.IGNORECASE)
                            if b_match:
                                row_num = int(b_match.group(1))
                                # Replace both commas and spaces with spaces for consistent format
                                data = re.sub(r'[,\s]+', ' ', b_match.group(2).strip())
                                matrix_b[row_num] = data
                                print(f"Found matrix B row {row_num}")
                except UnicodeDecodeError:
                    # If the file is not a text file (binary file), skip it
                    print(f"Skipping binary file: {filepath}")
                    continue
                except Exception as e:
                    print(f"Error processing file {filepath}: {e}")
                    continue
    
    print(f"Processed {files_processed} files total, {text_files_processed} text files")
    print(f"Found {len(matrix_a)} matrix A rows and {len(matrix_b)} matrix B rows")
    
    # If we found matrix data, determine dimensions
    if matrix_a and matrix_b:
        # Matrix A dimensions: rows x columns
        m = len(matrix_a)
        # For matrix A columns (k), count elements in the first row
        k = len(matrix_a[min(matrix_a.keys())].split())
        # Matrix B dimensions: k x n
        n = len(matrix_b[min(matrix_b.keys())].split())
        
        # Format data for task3
        # Use an absolute path to write the output file
        output_path = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'data', 'task3.txt')
        print(f"Writing matrix data to: {output_path}")
        
        with open(output_path, 'w') as output:
            # Just 1 task (task_id = 1)
            output.write('1\n')
            
            # First task (task_id, m, k, n)
            output.write('1 {} {} {}\n'.format(m, k, n))
            
            # Write matrix A rows
            for i in sorted(matrix_a.keys()):
                output.write(matrix_a[i] + '\n')
            
            # Write matrix B rows
            for i in sorted(matrix_b.keys()):
                output.write(matrix_b[i] + '\n')
        
        print(f"Processed matrices: A({m}x{k}) and B({k}x{n})")
        return True
    else:
        print("No matrix data found!")
        if files_processed == 0:
            print("No files were processed! Check if the tar.gz was extracted correctly.")
        return False

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python process_matrix_data.py <directory>")
        sys.exit(1)
    
    directory = sys.argv[1]
    if not os.path.isdir(directory):
        print(f"Error: {directory} is not a valid directory")
        sys.exit(1)
    
    print(f"Searching for matrix data in {directory} and its subdirectories...")
    success = process_matrix_files(directory)
    if not success:
        sys.exit(1)