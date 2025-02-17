from psd_tools import PSDImage
import os
import sys

def process_license_psd(psd_path):
    try:
        print(f"Attempting to open PSD file at: {psd_path}")
        
        # Check if file exists
        if not os.path.exists(psd_path):
            print(f"Error: PSD file not found at {psd_path}")
            return
            
        # Open the PSD file
        psd = PSDImage.open(psd_path)
        print("Successfully opened PSD file")
        
        # Export normal PNG
        print("Exporting normal PNG...")
        psd.composite().save('Mach1-LicenseSummary.png')
        print("Exported Mach1-LicenseSummary.png")
        
        # # Find and enable white background layer, then export
        # print("Looking for white_bg layer...")
        # white_bg_found = False
        # for layer in psd.descendants():
        #     print(f"Found layer: {layer.name}")
        #     if layer.name.lower() == 'white_bg':
        #         print("Found white_bg layer, making visible and exporting...")
        #         white_bg_found = True
                
        #         # Store original visibility state
        #         original_visibility = layer.visible
                
        #         try:
        #             # Make visible and save PSD
        #             layer.visible = True
        #             psd.save('Mach1-LicenseSummary.psd')
                    
        #             # Reload PSD to ensure visibility change is applied
        #             psd = PSDImage.open('Mach1-LicenseSummary.psd')
                    
        #             # Export PNG with white background
        #             psd.composite().save('Mach1-LicenseSummary-wbg.png')
        #             print("Exported Mach1-LicenseSummary-wbg.png")
                    
        #         finally:
        #             # Restore original visibility and save PSD
        #             layer.visible = original_visibility
        #             psd.save('Mach1-LicenseSummary.psd')
        #             psd = PSDImage.open('Mach1-LicenseSummary.psd')
        #             print("Restored original PSD state")
        #         break
        
        # if not white_bg_found:
        #     print("Warning: No 'white_bg' layer found in PSD")
        
        # Extract text and maintain formatting
        print("Extracting text content...")
        text_content = []
        sections = {
            'title': [],
            'can': [],
            'cannot': [],
            'must': []
        }
        current_section = 'title'
        
        def extract_text_from_layer(layer, indent=0):
            if hasattr(layer, 'text') and layer.text:
                # Clean up text: normalize spaces, remove line breaks, and trim
                text = ' '.join(layer.text.replace('\r', ' ').replace('\n', ' ').split()).strip()
                if not text:
                    return
                
                # Determine section based on text content
                nonlocal current_section
                if text == 'CAN':
                    current_section = 'can'
                    print(f"Switching to section: {current_section}")
                elif text == 'CANNOT':
                    current_section = 'cannot'
                    print(f"Switching to section: {current_section}")
                elif text == 'MUST':
                    current_section = 'must'
                    print(f"Switching to section: {current_section}")
                else:
                    # Skip the license disclaimer text
                    if text.startswith('The license for') or text.startswith('Mach1 Spatial SDK Open Use License. This summary'):
                        return
                        
                    if text.startswith('•'):
                        # Convert bullet point to dash and clean up any non-breaking spaces
                        cleaned_text = text[1:].strip().replace('\xa0', ' ')
                        text = f" - {cleaned_text}"
                        sections[current_section].append(text)
                    elif not any(text.startswith(header) for header in ['MACH1 SPATIAL SDK', 'License Version', 'OPEN USE']):
                        sections[current_section].append(text)
            
            if hasattr(layer, 'layers'):
                for sublayer in layer.layers:
                    extract_text_from_layer(sublayer)
        
        # Process all layers
        for layer in psd.descendants():
            extract_text_from_layer(layer)
        
        print("Sections content:")
        for section, content in sections.items():
            print(f"{section}: {content}")
        
        # Combine sections with proper formatting
        formatted_text = []
        formatted_text.append('MACH1 SPATIAL SDK')
        formatted_text.append('License Version 3.0')
        formatted_text.append('MACH1 Spatial SDK Open Use License')
        formatted_text.append('\n------------\n')
        formatted_text.append('Mach1 Spatial SDK License Summary\n')
        formatted_text.append('CAN:')
        formatted_text.extend(sections['can'])
        formatted_text.append('\nCANNOT:')
        formatted_text.extend(sections['cannot'])
        formatted_text.append('\nMUST:')
        formatted_text.extend(sections['must'])
        formatted_text.append('\n------------')
        
        # Clean up any remaining line breaks and extra spaces in the final text
        final_text = []
        for line in formatted_text:
            if line.startswith(' - '):
                # For bullet points, ensure they're on one line
                final_text.append(' - ' + ' '.join(line[3:].split()))
            else:
                final_text.append(line)
        
        # Write text to file
        print("Writing text content to file...")
        with open('Mach1-LicenseSummary.txt', 'w', encoding='utf-8') as f:
            f.write('\n'.join(final_text))
        print("Created Mach1-LicenseSummary.txt")
        
        print("Processing complete!")
        
    except Exception as e:
        print(f"Error processing PSD file: {str(e)}")
        print(f"Error type: {type(e)}")
        import traceback
        traceback.print_exc()

if __name__ == "__main__":
    script_dir = os.path.dirname(os.path.abspath(__file__))
    psd_path = os.path.join(script_dir, 'Mach1-LicenseSummary.psd')
    print(f"Script directory: {script_dir}")
    process_license_psd(psd_path) 