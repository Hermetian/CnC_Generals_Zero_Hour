#!/usr/bin/env python
"""
Script to apply stlport.diff patches to STLport-4.5.3
This script will parse the diff file and apply changes only if they haven't been applied already.
It can search for STLport directories automatically and patch multiple installations.
"""

import os
import re
import sys
import difflib
import argparse
import subprocess
from pathlib import Path
import traceback

def parse_args():
    """Parse command line arguments."""
    parser = argparse.ArgumentParser(description='Apply stlport.diff patch to STLport-4.5.3')
    parser.add_argument('--diff-file', default='stlport.diff',
                        help='Path to the stlport.diff file')
    parser.add_argument('--stlport-dir', 
                        help='Path to the STLport-4.5.3 directory. If not specified, will search automatically.')
    parser.add_argument('--project-root', default='.',
                        help='Path to the project root directory (for auto-search)')
    parser.add_argument('--dry-run', action='store_true',
                        help='Only show what would be changed, without making actual changes')
    parser.add_argument('--verbose', action='store_true',
                        help='Show detailed information about changes')
    # For backward compatibility with direct arguments
    args, unknown_args = parser.parse_known_args()
    if unknown_args and not args.stlport_dir:
        # If we have positional arguments, use the first one as stlport_dir
        args.stlport_dir = unknown_args[0]
        # And if there's a second one, use it as diff_file
        if len(unknown_args) > 1:
            args.diff_file = unknown_args[1]
    return args

def find_stlport_dirs(project_root):
    """Find all STLport-4.5.3 directories in the project."""
    stlport_dirs = []
    
    print(f"Looking for STLport-4.5.3 in {project_root}")
    
    # Expected STLport paths based on README.md
    expected_paths = [
        os.path.join(project_root, 'Generals', 'Code', 'Libraries', 'STLport-4.5.3'),
        os.path.join(project_root, 'GeneralsMD', 'Code', 'Libraries', 'Source', 'STLport-4.5.3'),
        os.path.join(project_root, 'GeneralsMD', 'Code', 'Libraries', 'STLport-4.5.3')
    ]
    
    # Check expected paths first
    for path in expected_paths:
        print(f"Checking expected path: {path}")
        if os.path.exists(path) and os.path.isdir(path):
            print(f"Found STLport-4.5.3 at {path}")
            stlport_dirs.append(path)
    
    # If we didn't find any in the expected locations, search for them
    if not stlport_dirs:
        print("STLport directories not found in expected locations. Searching...")
        for root, dirs, _ in os.walk(project_root):
            if 'STLport-4.5.3' in dirs:
                stlport_dir = os.path.join(root, 'STLport-4.5.3')
                print(f"Found STLport-4.5.3 at {stlport_dir}")
                stlport_dirs.append(stlport_dir)
                
    if not stlport_dirs:
        print("No STLport-4.5.3 directories found.")
    
    return stlport_dirs

def parse_diff_file(diff_file_path):
    """
    Parse the diff file to extract all patches.
    
    The diff file format starts with "--- filename" followed by "+++ filename",
    then the patch hunks.
    
    Returns a dictionary where keys are file paths and values are the patch lines.
    """
    patches = {}
    
    with open(diff_file_path, 'r', encoding='utf-8', errors='ignore') as f:
        lines = f.readlines()
        
        i = 0
        while i < len(lines):
            # Find the start of a patch (--- line)
            if i < len(lines) and lines[i].startswith('--- '):
                # Extract the file path from the --- line
                file_path = lines[i].strip().split('\t')[0][4:]  # Skip "--- " and take before tab if present
                current_patch = [lines[i]]
                
                # Get the +++ line
                i += 1
                if i < len(lines) and lines[i].startswith('+++ '):
                    current_patch.append(lines[i])
                    i += 1
                else:
                    # Invalid diff format, skip this patch
                    i += 1
                    continue
                
                # Collect the patch hunks
                while i < len(lines) and (not lines[i].startswith('--- ')) and i < len(lines):
                    if (lines[i].startswith('@@') or lines[i].startswith(' ') or 
                        lines[i].startswith('+') or lines[i].startswith('-')):
                        current_patch.append(lines[i])
                    i += 1
                
                # Add the patch to the dictionary
                if file_path and current_patch:
                    patches[file_path] = current_patch
                
                # Don't increment i again as we're already at the next "---" line or EOF
                continue
            
            i += 1
    
    return patches

def map_diff_path_to_actual_path(file_path, stlport_dir):
    """
    Maps a file path in the diff to an actual file path in the STLport directory structure.
    
    In the diff file, paths like '/stl/_algobase.h' refer to files at 
    'STLport-4.5.3/stlport/stl/_algobase.h' in the actual directory structure.
    
    Handles the conversion between forward slashes (/) in the diff file and 
    backslashes (\\) in Windows paths.
    """
    # Remove leading slash if present
    if file_path.startswith('/'):
        file_path = file_path[1:]
    
    # Normalize path separators (handle both \ and / consistently)
    file_path = file_path.replace('/', os.sep)
    
    # The primary location to check is under the stlport subdirectory
    stlport_path = os.path.join(stlport_dir, "stlport", file_path)
    if os.path.exists(stlport_path):
        print(f"Found file at: {stlport_path}")
        return stlport_path
    
    # As a fallback, check if the file exists directly under the STLport directory
    full_path = os.path.join(stlport_dir, file_path)
    if os.path.exists(full_path):
        print(f"Found file at: {full_path}")
        return full_path
    
    # For paths with directories like "stl\" or "config\", ensure they're under stlport/
    for subdir in ["stl", "config"]:
        if file_path.startswith(f"{subdir}{os.sep}"):
            path = os.path.join(stlport_dir, "stlport", file_path)
            if os.path.exists(path):
                print(f"Found file at: {path}")
                return path
    
    # If we still can't find it, try again with forward slashes
    # (some files might be referenced with a different separator pattern)
    file_path_fwd = file_path.replace(os.sep, '/')
    stlport_path_fwd = os.path.normpath(os.path.join(stlport_dir, "stlport", file_path_fwd))
    if os.path.exists(stlport_path_fwd):
        print(f"Found file at: {stlport_path_fwd}")
        return stlport_path_fwd
    
    print(f"Tried paths:")
    print(f"  - {stlport_path}")
    print(f"  - {full_path}")
    print(f"  - {stlport_path_fwd}")
    
    # If we couldn't find it, return None
    return None

def apply_patch(file_path, patch_lines, stlport_dir, dry_run=False):
    # Map the diff path to the actual file path
    actual_file_path = map_diff_path_to_actual_path(file_path, stlport_dir)
    
    print(f"Processing {file_path}...")
    
    if actual_file_path is None:
        print(f"Error: File {file_path} does not exist in the STLport directory structure")
        return False
    
    # Extract @@ lines to get line numbers
    hunks = []
    current_hunk = None
    
    for line in patch_lines:
        if line.startswith('@@'):
            # Parse the @@ line to get line numbers
            match = re.search(r'@@ -(\d+),(\d+) \+(\d+),(\d+) @@', line)
            if match:
                old_start, old_count, new_start, new_count = map(int, match.groups())
                current_hunk = {
                    'old_start': old_start,
                    'old_count': old_count,
                    'new_start': new_start,
                    'new_count': new_count,
                    'lines': []
                }
                hunks.append(current_hunk)
        elif current_hunk is not None:
            current_hunk['lines'].append(line)
    
    # Read the file content
    try:
        with open(actual_file_path, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.readlines()
    except FileNotFoundError:
        print(f"Error: File {actual_file_path} not found")
        return False
    except Exception as e:
        print(f"Error reading {actual_file_path}: {e}")
        return False
    
    # Track successful and failed hunks
    successful_hunks = 0
    failed_hunks = 0
    applied_changes = False
    
    # Apply the patches for each hunk
    for hunk_idx, hunk in enumerate(hunks):
        old_start = hunk['old_start'] - 1  # Convert to 0-based indexing
        current_line = old_start
        
        # Create a copy of the content to apply this hunk
        new_content = content.copy()
        modified_content = new_content[:old_start]
        
        # Track if this hunk succeeded
        hunk_succeeded = True
        
        # Try to apply this hunk
        for line in hunk['lines']:
            if line.startswith(' '):  # Context line
                if current_line < len(content) and content[current_line].rstrip() == line[1:].rstrip():
                    modified_content.append(content[current_line])
                    current_line += 1
                else:
                    print(f"Error in hunk {hunk_idx+1}/{len(hunks)}: Context mismatch at line {current_line + 1}")
                    print(f"Expected: '{line[1:].rstrip()}'")
                    print(f"Found: '{content[current_line].rstrip() if current_line < len(content) else 'EOF'}'")
                    hunk_succeeded = False
                    failed_hunks += 1
                    break
            elif line.startswith('-'):  # Remove line
                if current_line < len(content) and content[current_line].rstrip() == line[1:].rstrip():
                    current_line += 1
                else:
                    print(f"Error in hunk {hunk_idx+1}/{len(hunks)}: Cannot remove line at {current_line + 1}")
                    print(f"Expected: '{line[1:].rstrip()}'")
                    print(f"Found: '{content[current_line].rstrip() if current_line < len(content) else 'EOF'}'")
                    hunk_succeeded = False
                    failed_hunks += 1
                    break
            elif line.startswith('+'):  # Add line
                modified_content.append(line[1:])
        
        # If the hunk succeeded, update the content for the next hunk
        if hunk_succeeded:
            successful_hunks += 1
            applied_changes = True
            # Add the rest of the file
            modified_content.extend(content[current_line:])
            content = modified_content
    
    # Only write to the file if at least one hunk was successful
    if applied_changes and not dry_run:
        try:
            with open(actual_file_path, 'w', encoding='utf-8') as f:
                f.writelines(content)
            print(f"Successfully patched {file_path} ({successful_hunks}/{len(hunks)} hunks applied)")
        except Exception as e:
            print(f"Error writing to {actual_file_path}: {e}")
            return False
    elif applied_changes and dry_run:
        print(f"Would patch {file_path} ({successful_hunks}/{len(hunks)} hunks would be applied)")
    else:
        if len(hunks) > 0:
            print(f"No changes applied to {file_path} ({failed_hunks}/{len(hunks)} hunks failed)")
        else:
            print(f"No hunks found in patch for {file_path}")
    
    return applied_changes

def apply_patches_to_stlport_dir(stlport_dir, diff_file_path, dry_run=False, verbose=False):
    """Apply patches to a single STLport directory."""
    print(f"\nApplying patches to {stlport_dir} using diff file {diff_file_path}")
    
    # Verify the diff file exists
    if not os.path.exists(diff_file_path):
        print(f"Error: Diff file {diff_file_path} does not exist")
        return False
    
    # Verify the STLport directory exists
    if not os.path.exists(stlport_dir):
        print(f"Error: STLport directory {stlport_dir} does not exist")
        return False

    # Parse the diff file to get all patches
    patches = parse_diff_file(diff_file_path)
    if not patches:
        print("Error: No patches found in the diff file")
        return False

    print(f"Applying {len(patches)} patches...")

    # Apply the patches
    success_count = 0
    for file_path, patch_lines in patches.items():
        if verbose:
            print(f"Applying patch to {file_path}")
        result = apply_patch(file_path, patch_lines, stlport_dir, dry_run)
        if result:
            success_count += 1

    print(f"Patching complete. {success_count}/{len(patches)} files were patched successfully.")
    return success_count > 0

def main():
    """Main function to apply STLport patches."""
    try:
        # Parse command-line arguments
        args = parse_args()
        
        # Determine STLport directories
        stlport_dirs = []
        if args.stlport_dir:
            # If a specific directory is provided, use it
            stlport_dirs = [args.stlport_dir]
        else:
            # Otherwise, search for STLport directories
            print("STLport directory not specified. Searching automatically...")
            project_root = os.path.abspath(args.project_root)
            stlport_dirs = find_stlport_dirs(project_root)
            
            if not stlport_dirs:
                print("Error: No STLport-4.5.3 directories found in the project")
                sys.exit(1)
        
        # Determine the diff file path for each STLport directory
        all_success = True
        for stlport_dir in stlport_dirs:
            # Default: Check if the diff file is in the STLport directory
            stlport_diff = os.path.join(stlport_dir, 'stlport.diff')
            
            # Use the user-specified diff file if provided, otherwise use the one in STLport dir
            diff_file_path = args.diff_file if args.diff_file != 'stlport.diff' else stlport_diff
            
            # If the user-specified path doesn't exist, fall back to the STLport directory
            if not os.path.exists(diff_file_path) and args.diff_file != 'stlport.diff':
                print(f"Warning: Specified diff file {diff_file_path} not found.")
                print(f"Checking for diff file in STLport directory: {stlport_diff}")
                if os.path.exists(stlport_diff):
                    diff_file_path = stlport_diff
                else:
                    print(f"Error: No diff file found for {stlport_dir}")
                    all_success = False
                    continue
            
            # Apply patches to this STLport directory
            success = apply_patches_to_stlport_dir(
                stlport_dir, 
                diff_file_path, 
                dry_run=args.dry_run, 
                verbose=args.verbose
            )
            
            if not success:
                all_success = False
        
        if all_success:
            print("\nAll STLport directories patched successfully.")
            return 0
        else:
            print("\nSome STLport directories could not be patched. See errors above.")
            return 1
            
    except Exception as e:
        print(f"Error: {e}")
        traceback.print_exc()
        return 1

if __name__ == "__main__":
    sys.exit(main()) 